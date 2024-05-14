#include <WiFi.h>
#include <Arduino.h>

const char *ssid = "xiaopin";
const char *password = "QQ715640167";


WiFiUDP udp;
unsigned int port = 6060;
const char *station_ip = "192.168.200.251";

uint8_t speed_pin = 4;
uint8_t direction_pin = 16;
uint8_t pwm_pin = 17;

uint8_t fans_state = 0;

// 定义缓冲区
char buff[255];

void wifi_connect()
{
	WiFi.mode(WIFI_STA);
	WiFi.setSleep(false); // 关闭STA模式下wifi休眠，提高响应速度
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(200);
		Serial.print(".");
	}
	Serial.print("Connected, IP Address: ");
	Serial.println(WiFi.localIP());
}

void setup()
{
	Serial.begin(115200);
	analogWriteFrequency(10000);
	analogWrite(pwm_pin, 0);
	wifi_connect();
	pinMode(direction_pin,OUTPUT);
	pinMode(speed_pin,INPUT);
	pinMode(pwm_pin,OUTPUT);
	digitalWrite(direction_pin, HIGH);
	udp.begin(port);
}

String get_message()
{
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
	return message;
}

void loop()
{
	String message = get_message();
	if (message.equals(""))
		delay(1000);
	else if (message.equals("start out"))
	{
		fans_state = 1;
		Serial.println("正转");
		digitalWrite(direction_pin, HIGH);
		analogWrite(pwm_pin, 100);
		// udp.beginPacket(station_ip, 6060);
		// udp.print("fans on");
		// udp.endPacket();
		delay(1000);
	}
	else if (message.equals("start in"))
	{
		fans_state = -1;
		Serial.println("反转");
		digitalWrite(direction_pin, LOW);
		analogWrite(pwm_pin, 100);
		delay(1000);
	}
	else if (message.equals("stop"))
	{
		fans_state = 0;
		Serial.println("停止");
		// digitalWrite(direction_pin,LOW);
		analogWrite(pwm_pin, 0);
		// udp.beginPacket(station_ip, 6060);
		// udp.print("fans off");
		// udp.endPacket();
		delay(1000);
	}
	else if(message.equals("state"))
	{
		udp.beginPacket(station_ip, 6060);
		if(fans_state == 1)
			udp.print("fans on");
		else if(fans_state == 0)
			udp.print("fans off");
		udp.endPacket();
		delay(1000);
	}
	else
	delay(1000);
}