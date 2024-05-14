#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_WITHOUT_SSL

#include <Blinker.h>
#include <Arduino.h>
#include <WiFi.h>
#include "TFT_eSPI.h"
#include <ESP_Mail_Client.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>
#include <Ticker.h>
#include <DHT.h>
#include <HTTPClient.h>
#include "AudioTools.h"
#include "AudioLibs/AudioSourceSPIFFS.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include <Servo.h>
#include <WiFiUdp.h>

// smtp邮件服务器配置
const char *SMTP_HOST PROGMEM = "smtp.qq.com";
int SMTP_PORT = esp_mail_smtp_port_465;
const char *AUTHOR_EMAIL PROGMEM = "715640167@qq.com";
const char *AUTHOR_PASSWORD PROGMEM = "tihpmboxgjdabbdg";
const char *RECIPIENT_EMAIL PROGMEM = "715640167@qq.com";

// 阿里云mqtt的配置
const char *mqtt_broker_addr PROGMEM = "iot-06z00a6l3gle6wu.mqtt.iothub.aliyuncs.com";								  // 服务器地址
const uint16_t mqtt_broker_port PROGMEM PROGMEM= 1883;																			  // 服务端口号
const char *mqtt_username PROGMEM = "esp32_dev&ikl8SEBFw6a";															  // 账号（非必须）
const char *mqtt_password PROGMEM = "983317b9fb90945c034e5fa58fdddc35916f4aa2bbdb7fd0507e7dfaa29be4f9";					  // 密码（非必须）
const uint16_t mqtt_client_buff_size PROGMEM= 4096;																			  // 客户端缓存大小（非必须）
const char *mqtt_client_id PROGMEM = "ikl8SEBFw6a.esp32_dev|securemode=2,signmethod=hmacsha256,timestamp=1709755694733|"; // 客户端ID
const char *mqtt_topic_pub PROGMEM = "/sys/ikl8SEBFw6a/esp32_dev/thing/event/property/post";							  // 需要发布到的主题
const char *mqtt_topic_sub PROGMEM = "/sys/ikl8SEBFw6a/esp32_dev/thing/service/property/set";							  // 需要订阅的主题

// 经纬度（主要为了获取天气信息）
float longitude = 116.39;
float latitude = 39.90;
uint8_t location_set_sys = 1;

// 危险气体阈值
float co_threshold = 2.5;
float boom_threshold = 2.0;
float ch4_threshold = 3.0;
uint8_t aqi_threshold_sys = 1;

// 气体检测超限次数记录
uint8_t co_warnning = 0;
uint8_t boom_warnning = 0;
uint8_t ch4_warnning = 0;

uint8_t co_warnning_report = 0;
uint8_t boom_warnning_report = 0;
uint8_t ch4_warnning_report = 0;

JSONVar message_json; 				// 信息模板
JSONVar state_params; 				// 状态模板
uint8_t win_state = 0;	   			// 窗户的状态 开/关
uint8_t fans_state = 0;
uint16_t process_order = 0; 			// 配额计数器

// 天气情况的api
const char *base_aqi_get_url PROGMEM = "http://api.caiyunapp.com/v2.6/fuwhPNzXsYXIvMya/";

// wifi配置
const char *id PROGMEM = "xiaopin";
const char *psw PROGMEM = "QQ715640167";

// const char *sound_server PROGMEM= "192.168.159.240";
const char *fans_ip PROGMEM= "192.168.200.247";

WiFiUDP udp;

const char *blinker_key PROGMEM = "be4686383d74";	// blinker设备密钥
char buttonname[8] PROGMEM = "btn-win";
BlinkerButton Button1(buttonname);

// 硬件传感器管脚的声明
uint8_t led_pin PROGMEM= 22;
uint8_t co_pin PROGMEM= 32;
uint8_t boom_pin PROGMEM= 35;
uint8_t ch4_pin PROGMEM= 34;
uint8_t dht_pin PROGMEM= 23;
uint8_t servo_1_pin PROGMEM= 12;
uint8_t servo_2_pin PROGMEM= 14;
DHT dht(dht_pin, DHT11);
TFT_eSPI tft = TFT_eSPI();
Servo servo_1;
Servo servo_2;

// 播报配置
const char *urls[] PROGMEM= {
	"http://192.168.200.240:8000/start.mp3",
	"http://192.168.200.240:8000/opwin.mp3",
	"http://192.168.200.240:8000/clwin.mp3",
	"http://192.168.200.240:8000/co.mp3",
	"http://192.168.200.240:8000/boom.mp3",
	"http://192.168.200.240:8000/ch4.mp3"
};

const char *tips[] PROGMEM= {
	"今天户外空气很好，记得开窗",
	"今天户外空气不错，可以适度开窗",
	"今天户外空气较差，开窗注意时间哦",
	"今天户外空气很差，请注意开窗时间",
	"今天户外空气很差，请注意开窗时间"
};

const char *warnning[] PROGMEM = {
	"CO 浓度过高，请及时处理（30秒后自动开窗）",
	"CH4 浓度过高，请及时处理（30秒后自动开窗）",
	"可燃气体浓度过高，警惕爆炸（30秒后自动开窗）"
};

URLStream urlStream(id, psw);
AudioSourceURL source(urlStream, urls, "audio/mp3");
I2SStream i2s;
MP3DecoderHelix decoder;
AudioPlayer player;

// 软件服务的声明
SMTPSession smtp;	   		// smtp邮件客户端
Session_Config config; 		// 邮件客户端配置
WiFiClient tcpClient;
PubSubClient mqttClient; 	// mqtt客户端
Ticker openning_timer;
Ticker openning_timer_fans;

// 函数声明
void wifi_connect();												 		// 联网
void smtp_configure();												 		// 配置smtp
void smtp_send_email(String subject, String content);				 		// 发送邮件
void smtpCallback(SMTP_Status status);								 		// 服务器信息解读
void mqtt_configure();												 		// 配置mqtt
void mqtt_callback(char *topic, byte *payload, unsigned int length); 		// mqtt信息反馈
void mqtt_publish_attribute();										 		// mqtt上报属性
void check_state();													 		// 获取各种传感器状态
void get_aqi_out_of_door();											 		// 从网上获取天气信息
void open_win();
void close_win();
void open_fans();
void close_fans();
void miotQuery(int32_t queryCode);											// blinker事件处理
void miotPowerState(const String &state);									// 小爱同学开关状态的获取
void button1_callback(const String &state); 								// blinker开关反馈
void dealJsonString(String &str);
void get_fans_state();
void fans_auto_close();

void printMetaData(MetaDataType type, const char* str, int len){
  Serial.print("==> ");
  Serial.print(toStr(type));
  Serial.print(": ");
  Serial.println(str);
}

void setup()
{
	Serial.begin(115200);
	// wifi_connect();
	AudioLogger::instance().begin(Serial, AudioLogger::Info);

	{// 管脚初始化
		pinMode(led_pin, OUTPUT);
		pinMode(dht_pin, OUTPUT);
		pinMode(co_pin, INPUT);
		pinMode(boom_pin, INPUT);
		pinMode(servo_1_pin, OUTPUT);
		pinMode(servo_2_pin, OUTPUT);
		analogSetPinAttenuation(co_pin, ADC_11db);
		analogSetPinAttenuation(boom_pin, ADC_11db);
		analogSetPinAttenuation(ch4_pin, ADC_11db);
		servo_1.attach(servo_1_pin);
		servo_2.attach(servo_2_pin);
	}
	AudioLogger::instance().begin(Serial, AudioLogger::Info);
	audio_tools::I2SConfig cfg = i2s.defaultConfig();
	cfg.pin_bck = 27;
	cfg.pin_ws = 25;
	cfg.pin_data = 26;
	i2s.begin(cfg);
	player.setDecoder(decoder);
	player.setOutput(i2s);
	player.setAudioSource(source);
	player.setMetadataCallback(printMetaData);
	source.selectStream(0);
	player.setAutoNext(false);
	player.begin();
	while(player.copy()>0);
	player.stop();
	// source.end();
	{// mqtt信息格式
		message_json["version"] = "1.0.0";
		state_params["aqi_out_of_door"] = "{\"description\": \"良\", \"pm10\": 111, \"o3\": 119, \"so2\": 2, \"co\": 0.5, \"aqi\": 81, \"no2\": 22, \"pm25\": 1000}";
		state_params["win_state"] = 0;
		state_params["fans_state"] = 0;
		state_params["humidity"] = 0;
		state_params["aqi_in_the_room"] = "{\"humi\": 34, \"CO\": 0.06124542, \"boom\": 0.0, \"temp\": 56}";
		state_params["openning_time"] = -1;
		state_params["temperature"] = 0;
		state_params["aqi_threshold"] = "{\"aqi_threshold_sys\":0,\"co_threshold\": 0.5, \"boom_threshold\": 2.0, \"ch4_threshold\": 1.5}";
		message_json["params"] = state_params;
	}

	{// blinker初始化
		BLINKER_DEBUG.stream(Serial);
		BLINKER_DEBUG.debugAll();
		// Blinker.begin(blinker_key)
		Blinker.begin(blinker_key, id, psw);
		Button1.attach(button1_callback);
		BlinkerMIOT.attachPowerState(miotPowerState); // 注册回调函数
		BlinkerMIOT.attachQuery(miotQuery);
	}

	{// 屏幕初始化
		tft.init();
		tft.setRotation(2);
		tft.setCursor(0, 0);
		tft.fillScreen(TFT_BLACK);
	}

	{// wifi smtp mqtt配置
		wifi_connect();
		smtp_configure();
		smtp_send_email("Hello", "我是你的小窗户^_^");
		mqtt_configure();
		udp.begin(6060);
	}
}

void loop()
{
	// player.copy();
	// 循环里面计数配额
	process_order++;
	if(process_order % 50 == 0)
		check_state();
	else if(process_order % 70 ==0)
		mqtt_publish_attribute();
	else if(process_order % 11300 == 0)
	{
		get_aqi_out_of_door();
		process_order = 0;
	}
	mqttClient.loop();
	delay(100);
	Blinker.run();
}

void wifi_connect()															// 联网
{
	WiFi.begin(id, psw);
	while (WiFi.status() != WL_CONNECTED)
	{
		digitalWrite(led_pin, HIGH);
		delay(1000);
		Serial.println("正在连接...");
		digitalWrite(led_pin, LOW);
		delay(500);
	}
	Serial.println("连接成功！");
	Serial.print("IP地址:");
	Serial.println(WiFi.localIP());
	digitalWrite(led_pin, HIGH);
}

void smtp_configure()														// 配置smtp
{
	smtp.debug(1);
	smtp.callback(smtpCallback);
	config.server.host_name = SMTP_HOST;
	config.server.port = SMTP_PORT;
	config.login.email = AUTHOR_EMAIL;
	config.login.password = AUTHOR_PASSWORD;
	config.login.user_domain = F("127.0.0.1");
	config.time.ntp_server = F("ntp.aliyun.com,time.nist.gov");
	config.time.gmt_offset = 3;
	config.time.day_light_offset = 0;
}

void smtp_send_email(String subject, String content)						// 发送邮件
{
	SMTP_Message message;
	message.sender.name = F("The Windows");
	message.sender.email = AUTHOR_EMAIL;
	message.subject = subject;
	message.addRecipient(F("Master"), RECIPIENT_EMAIL);
	message.text.content = content;
	message.text.transfer_encoding = "base64";
	message.text.charSet = F("utf-8");
	message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
	message.addHeader(F("Message-ID: <abcde.fghij@gmail.com>"));

	if (!smtp.connect(&config))
	{
		MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
		return;
	}
	if (!smtp.isLoggedIn())
	{
		Serial.println("Not yet logged in.");
	}
	else
	{
		if (smtp.isAuthenticated())
			Serial.println("Successfully logged in.");
		else
			Serial.println("Connected with no Auth.");
	}
	if (!MailClient.sendMail(&smtp, &message))
		MailClient.printf("Error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}

void smtpCallback(SMTP_Status status)										// 服务器信息解读
{
	Serial.println(status.info());
	if (status.success())
	{
		Serial.println("----------------");
		MailClient.printf("Message sent success: %d\n", status.completedCount());
		MailClient.printf("Message sent failed: %d\n", status.failedCount());
		Serial.println("----------------\n");

		for (size_t i = 0; i < smtp.sendingResult.size(); i++)
		{
			SMTP_Result result = smtp.sendingResult.getItem(i);
			MailClient.printf("Message No: %d\n", i + 1);
			MailClient.printf("Status: %s\n", result.completed ? "success" : "failed");
			MailClient.printf("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
			MailClient.printf("Recipient: %s\n", result.recipients.c_str());
			MailClient.printf("Subject: %s\n", result.subject.c_str());
		}
		Serial.println("----------------\n");
		smtp.sendingResult.clear();
	}
}

void mqtt_configure()														// 配置mqtt
{
	mqttClient.setClient(tcpClient);
	mqttClient.setServer(mqtt_broker_addr, mqtt_broker_port);
	mqttClient.setBufferSize(mqtt_client_buff_size);
	mqttClient.setCallback(mqtt_callback);
	mqttClient.setKeepAlive(30);
	if (!mqttClient.connect(mqtt_client_id, mqtt_username, mqtt_password))
	{
		Serial.println("与mqtt server连接失败\n");
	}
	else
	{
		Serial.println("与mqtt server连接成功\n");
		mqttClient.subscribe(mqtt_topic_sub); // 连接成功后可以订阅主题
		// mqtt_publish_ticker.attach(5, mqtt_publish_attribute);
	}
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)			// mqtt信息反馈
{
	Serial.printf("Message arrived in topic %s, length %d\n", topic, length);
	Serial.print("Message:");
	for (int i = 0; i < length; i++)
	{
		Serial.print((char)payload[i]);
	}
	Serial.println("\n----------------END----------------");
	JSONVar message_get = JSON.parse((char *)payload);
	int new_win_state = message_get["items"]["win_state"]["value"];
	int new_fans_state = message_get["items"]["fans_state"]["value"];
	Serial.printf("new_win_state:%d\tnew_fans_state:%d\n",new_win_state,new_fans_state);
	if(new_win_state != win_state)
	{
		if(new_win_state)
			open_win();
		else
			close_win();
	}
	if(new_fans_state != fans_state)
	{
		if(new_fans_state)
			open_fans();
		else
			close_fans();
	}
	Serial.printf("openning_time:%d\n",int(message_get["items"]["openning_time"]["value"]));
	if(int(message_get["items"]["openning_time"]["value"]) != -1)
	{
		Serial.printf("定时开始");
		openning_timer.once(int(message_get["items"]["openning_time"]["value"])*10.0,close_win);
		// openning_timer.attach(int(message_get["items"]["openning_time"]["value"])*60,close_win);
	}
	Serial.printf("openning_time:%d\n",int(message_get["items"]["openning_time"]["value"]));
	if(int(message_get["items"]["openning_time_fans"]["value"]) != -1)
	{
		Serial.printf("定时开始");
		openning_timer_fans.once(int(message_get["items"]["openning_time_fans"]["value"])*10.0,fans_auto_close);
		// openning_timer.attach(int(message_get["items"]["openning_time"]["value"])*60,close_win);
	}
	String temp_aqi_threshold = JSON.stringify(message_get["items"]["aqi_threshold"]["value"]);
	dealJsonString(temp_aqi_threshold);
	JSONVar aqi_threshold = JSON.parse(temp_aqi_threshold);
	temp_aqi_threshold.~String();
	if(aqi_threshold.keys().length()>0)
	{
		Serial.println(aqi_threshold);
		aqi_threshold_sys = JSON.stringify(aqi_threshold["aqi_threshold_sys"]).toInt();
		co_threshold = JSON.stringify(aqi_threshold["co_threshold"]).toFloat();
		boom_threshold = JSON.stringify(aqi_threshold["boom_threshold"]).toFloat();
		ch4_threshold = JSON.stringify(aqi_threshold["ch4_threshold"]).toFloat();
		state_params["aqi_threshold"] = JSON.stringify(aqi_threshold);
		Serial.printf("aqi_threshold_sys:%d\tco_threshold:%f\tboom_threshold:%f\tch4_threshold:%f\n",aqi_threshold_sys,co_threshold,boom_threshold,ch4_threshold);
	}
	delete aqi_threshold;
	String temp_location_set = JSON.stringify(message_get["items"]["location_set"]["value"]);
	dealJsonString(temp_location_set);
	JSONVar location_set = JSON.parse(temp_location_set);
	temp_location_set.~String();
	if(location_set.keys().length()>0)
	{
		location_set_sys = JSON.stringify(location_set["location_set_sys"]).toInt();
		longitude = JSON.stringify(location_set["longitude"]).toFloat();
		latitude = JSON.stringify(location_set["latitude"]).toFloat();
		state_params["location_set"] = JSON.stringify(location_set);
		Serial.println(location_set_sys);
		Serial.printf("location_set_sys:%d\tlongitude:%f\tlatitude:%f\n",location_set_sys,longitude,latitude);
		get_aqi_out_of_door();
	}
	// check_state();
	// mqtt_publish_attribute();
}

void dealJsonString(String &str)
{
	if(*str.begin() == '"')
		str.remove(0,1);
	if(*str.end() == '"')
		str.remove(str.length()-1,1);
	for(int i = 0; i < str.length(); i++)
	{
		if(str[i] == '\\')
		{
			str.remove(i,1);
			i--;
		}
	}
}

void mqtt_publish_attribute()												// mqtt上报属性
{
	// noInterrupts();
	message_json["params"] = state_params;
	mqttClient.publish(mqtt_topic_pub, JSON.stringify(message_json).c_str()); // 连接成功后可以发布消息r
	// interrupts();
}

void check_state()															// 获取各种传感器状态
{
	state_params["temperature"] = int(dht.readTemperature()*100) / 100.0;
	state_params["humidity"] = int(dht.readHumidity() * 100) / 100.0;
	Serial.printf("temp:%f,humi:%f\n",state_params["temperature"],state_params["humidity"]);
	tft.fillScreen(TFT_BLACK);
	tft.setTextSize(2);
	tft.setTextColor(TFT_PURPLE);
	JSONVar aqi_in_the_room;
	aqi_in_the_room["temp"] = JSON.stringify(state_params["temperature"]);
	aqi_in_the_room["humi"] = JSON.stringify(state_params["humidity"]);
	float co_concentration = int(analogRead(co_pin)*3.3/4095*100) / 100.0;
	if (co_concentration > co_threshold)
	{
		if (co_warnning < 7)
			co_warnning++;
		if (co_warnning == 2 && co_warnning_report == 0)
		{
			tft.println(F("too high co concentration"));
			smtp_send_email("Warnning", warnning[0]);
			source.selectStream(3);
			player.play();
			while (player.copy() > 0)
				;
			player.stop();
			co_warnning_report = 1;
		}
		else if (co_warnning == 7)
		{
			if (win_state == 0)
			{
				// co_warnning = 0;
				open_win();
				open_fans();
			}
		}
	}
	else
	{
		if (co_warnning > 0)
		{
			co_warnning--;
			if (co_warnning == 0)
			{
				if (win_state == 1)
				{
					close_win();
					close_fans();
					co_warnning_report = 0;
				}
			}
		}
	}
	float ch4_concentration = int(analogRead(ch4_pin)*3.3/4095*100) / 100.0;
	if (ch4_concentration > ch4_threshold)
	{
		if(ch4_warnning<7)
			ch4_warnning++;
		if (ch4_warnning == 2&&ch4_warnning_report == 0)
		{
			source.selectStream(5);
			player.play();
			while (player.copy() > 0)
				;
			player.stop();
			smtp_send_email("Warnning", warnning[1]);
			ch4_warnning_report = 1;
		}
		else if (ch4_warnning == 7)
		{
			if (win_state == 0)
			{
				// ch4_warnning = 0;
				open_win();
				open_fans();
			}
		}
		tft.println(F("too high co concentration"));
	}
	else
	{
		if (ch4_warnning > 0)
		{
			ch4_warnning--;
			if (ch4_warnning == 0)
			{
				if (win_state == 1)
				{
					close_win();
					close_fans();
					ch4_warnning_report = 0;
				}
			}
		}
	}
	float boom_concentration = int(analogRead(boom_pin)*3.3/4095*100) / 100.0;
	if (boom_concentration > boom_threshold)
	{
		if(boom_warnning<7)
			boom_warnning++;
		if (boom_warnning == 2&&boom_warnning_report == 0)
		{
			source.selectStream(4);
			player.play();
			while (player.copy() > 0)
				;
			player.stop();
			tft.println(F("too high boom concentration"));
			smtp_send_email("Warnning", warnning[2]);
			boom_warnning_report = 1;
		}
		else if (boom_warnning == 7)
		{
			if (win_state == 0)
			{
				// boom_warnning = 0;
				open_win();
				open_fans();
			}
		}
	}
	else
	{
		if (boom_warnning > 0)
		{
			boom_warnning--;
			if (boom_warnning == 0)
			{
				if (win_state == 1)
				{
					close_win();
					close_fans();
					boom_warnning_report = 0;
				}
			}
		}
	}
	aqi_in_the_room["CO"] = co_concentration;
	aqi_in_the_room["boom"] = boom_concentration;
	aqi_in_the_room["ch4"] = ch4_concentration;
	tft.setCursor(5, 5);
	tft.setTextColor(TFT_ORANGE);
	tft.println(F(String("CO:"+String(co_concentration)).begin()));
	tft.setCursor(5, 25);
	tft.setTextColor(TFT_RED);
	tft.println(F(String("boom:"+String(boom_concentration)).begin()));
	tft.setCursor(5, 45);
	tft.setTextColor(TFT_BLUE);
	tft.println(F(String("CH4:"+String(ch4_concentration)).begin()));
	tft.setCursor(5, 65);
	tft.setTextColor(TFT_GREEN);
	tft.println(F(String("temp:"+JSON.stringify(state_params["temperature"])).begin()));
	tft.setCursor(5, 85);
	tft.setTextColor(TFT_PURPLE);
	tft.println(F(String("humi:"+JSON.stringify(state_params["humidity"])+"%").begin()));

	state_params["aqi_in_the_room"] = JSON.stringify(aqi_in_the_room);
	get_fans_state();
}

void get_aqi_out_of_door()													// 从网上获取天气信息
{
	HTTPClient http;		 	// http客户端
	http.begin(base_aqi_get_url+String(longitude)+","+String(latitude)+"/realtime");
	int httpCode = http.GET();
	if(httpCode>0)
	{
		JSONVar aqi_out_of_door = JSON.parse(http.getString())["result"]["realtime"];
		Serial.println(aqi_out_of_door);
		state_params["aqi_out_of_door"] = JSON.stringify(aqi_out_of_door["air_quality"]);
		String  description = JSON.stringify(aqi_out_of_door["air_quality"]["description"]["chn"]);
		if(description == "\"优\"")
		{
			smtp_send_email("tips", tips[0]);
		}
		else if(description.equals("\"良\""))
		{
			smtp_send_email("tips", tips[1]);
		}
		else if(description == "\"轻度污染\"")
		{
			smtp_send_email("tips", tips[2]);
		}
		else if(description == "\"中度污染\"")
		{
			smtp_send_email("tips", tips[3]);
		}
		else if(description == "\"重度污染\"")
		{
			smtp_send_email("tips", tips[4]);
		}
	}
	http.end();
}

void close_win()
{
	source.selectStream(2);
	player.play();
	while(player.copy()>0);
	player.stop();
	win_state = 0;
	state_params["win_state"] = win_state;
	for(int posDegrees = 60; posDegrees <= 167; posDegrees++) {
        servo_1.write(posDegrees-15);
		servo_2.write(223-posDegrees);
        // Serial.println(posDegrees);
        delay(10);
    }
	digitalWrite(led_pin, HIGH);
	// mqtt_publish_attribute();
	// delay(1000);
	// interrupts();
}

void open_win()
{
	source.selectStream(1);
	player.play();
	while(player.copy()>0);
	player.stop();

	win_state = 1;
	state_params["win_state"] = win_state;

	for(int posDegrees = 167; posDegrees >= 60; posDegrees--) {
        servo_1.write(posDegrees-10);
		servo_2.write(220-posDegrees);
        // Serial.println(posDegrees);
        delay(10);
    }
	digitalWrite(led_pin, LOW);
	// mqtt_publish_attribute();
	// delay(1000);
	// interrupts();
}

void get_fans_state()
{
	udp.beginPacket(fans_ip, 6060);
	udp.print("state");
	udp.endPacket();
	delay(1000);
	String message = "";
	int packetSize = udp.parsePacket(); // 获取当前队首数据包长度
	if (packetSize)						// 如果有数据可用
	{
		char buf[packetSize];
		int len = udp.read(buf, packetSize); // 读取当前包数据
		if (len > 0)						 // 为了避免获取的数据后面乱码做的判断
		{
			buf[len] = 0;
		}

		Serial.println();
		Serial.print("Received: ");
		Serial.println(buf);
		Serial.print("From IP: ");
		Serial.println(udp.remoteIP());
		Serial.print("From Port: ");
		Serial.println(udp.remotePort());
		message = String(buf);
	}
	if(message.equals("fans on"))
	{
		fans_state = 1;
		state_params["fans_state"] = 1;
	}
	else if(message.equals("fans off"))
	{
		fans_state = 0;
		state_params["fans_state"] = 0;
	}
}

void close_fans()
{
	fans_state = 0;
	udp.beginPacket(fans_ip, 6060);
	udp.print("stop");
	udp.endPacket();
	delay(1000);
	get_fans_state();
}

void open_fans()
{
	fans_state = 1;
	udp.beginPacket(fans_ip, 6060);
	udp.print("start out");
	udp.endPacket();
	delay(1000);
	get_fans_state();
}

void miotQuery(int32_t queryCode)											// 小爱同学 的回调查询函数
{
	BLINKER_LOG("MIOT Query codes: ", queryCode);
	switch (queryCode)
	{
	case BLINKER_CMD_QUERY_ALL_NUMBER:
		BLINKER_LOG("MIOT Query All");
		BlinkerMIOT.powerState(win_state ? "on" : "off");
		BlinkerMIOT.color(0);
		BlinkerMIOT.mode(0);
		BlinkerMIOT.colorTemp(1000);
		BlinkerMIOT.brightness(1);
		BlinkerMIOT.print();
		break;
	case BLINKER_CMD_QUERY_POWERSTATE_NUMBER:
		BLINKER_LOG("MIOT Query Power State");
		BlinkerMIOT.powerState(win_state ? "on" : "off");
		BlinkerMIOT.print();
		break;
	default:
		BlinkerMIOT.powerState(win_state ? "on" : "off");
		BlinkerMIOT.color(0);
		BlinkerMIOT.mode(0);
		BlinkerMIOT.colorTemp(1000);
		BlinkerMIOT.brightness(1);
		BlinkerMIOT.print();
		break;
	}
}

void miotPowerState(const String &state)									// 小爱同学开关状态的获取
{
	BLINKER_LOG("need set power state: ", state);
	if (state == BLINKER_CMD_ON)
	{
		win_state = 1;
		BlinkerMIOT.powerState("on");
		BlinkerMIOT.print();
		open_win();
	}
	else if (state == BLINKER_CMD_OFF)
	{
		win_state = 0;
		BlinkerMIOT.powerState("off");
		BlinkerMIOT.print();
		close_win();
	}
}

void button1_callback(const String &state)									// blinker开关反馈
{
	BLINKER_LOG("get button state: ", state);
	if (state == BLINKER_CMD_ON)
	{
		open_win();
		Serial.print("开");
	}
	else if (state == BLINKER_CMD_OFF)
	{
		close_win();
		Serial.print("关");
	}
	Blinker.vibrate();
}

void fans_auto_close()
{
	close_fans();
	open_win();
}