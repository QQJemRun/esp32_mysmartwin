#include "mqtt_win.h"
#include "ui_mqtt_win.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

mqtt_win::mqtt_win(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mqtt_win)
{
    ui->setupUi(this);
    ui->startbutton->setEnabled(false);
    m_client = new QMqttClient(this);
    m_client->setHostname(ui->lineEditHost->text());
    m_client->setPort(static_cast<quint16>(ui->spinBoxPort->value()));

    connect(ui->lineEditHost, &QLineEdit::textChanged, m_client, &QMqttClient::setHostname);
    connect(ui->spinBoxPort, QOverload<int>::of(&QSpinBox::valueChanged), m_client, &QMqttClient::setPort);
    connect(ui->lineEditUser, &QLineEdit::textChanged, m_client, &QMqttClient::setUsername);
    connect(ui->lineEditPassword, &QLineEdit::textChanged, m_client, &QMqttClient::setPassword);
    connect(ui->lineEditClientid, &QLineEdit::textChanged, m_client, &QMqttClient::setClientId);
    connect(ui->subscribe_topic,&QLineEdit::textChanged,this,[=]()
            {
        this->subscribe_topic = ui->subscribe_topic->text();
    });
    connect(ui->publish_topic,&QLineEdit::textChanged,this,[=]()
            {
        this->publish_topic = ui->publish_topic->text();
    });
    load_config();
}

mqtt_win::~mqtt_win()
{
    delete ui;
}

void mqtt_win::on_buttonConnect_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->lineEditHost->setEnabled(false);
        ui->spinBoxPort->setEnabled(false);
        ui->lineEditUser->setEnabled(false);
        ui->lineEditPassword->setEnabled(false);
        ui->lineEditClientid->setEnabled(false);
        ui->buttonConnect->setText(tr("Disconnect"));
        ui->startbutton->setEnabled(true);
        m_client->connectToHost();
        this->save_config();
    } else {
        ui->lineEditHost->setEnabled(true);
        ui->spinBoxPort->setEnabled(true);
        ui->lineEditUser->setEnabled(true);
        ui->lineEditPassword->setEnabled(true);
        ui->lineEditClientid->setEnabled(true);
        ui->startbutton->setEnabled(false);
        ui->buttonConnect->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
}





void mqtt_win::on_topic_set_clicked()
{
    this->publish_topic = ui->publish_topic->text();
    this->subscribe_topic = ui->subscribe_topic->text();
}




void mqtt_win::send_message(QString message)
{
    if (m_client->publish(this->publish_topic, message.toUtf8(),static_cast<quint8>(0),false) == -1)
    {
        QMessageBox::warning(this,"error","Could not publish message");
    }
}

void mqtt_win::save_config()
{
    QJsonObject mqtt_base_config,config,mqtt_topic_config;
    mqtt_base_config.insert("clientid", ui->lineEditClientid->text());
    mqtt_base_config.insert("username", ui->lineEditUser->text());
    mqtt_base_config.insert("password",ui->lineEditPassword->text());
    mqtt_base_config.insert("hostname",ui->lineEditHost->text());
    mqtt_base_config.insert("port",ui->spinBoxPort->value());
    mqtt_topic_config.insert("subscribe_topic",ui->subscribe_topic->text());
    mqtt_topic_config.insert("publish_topic",ui->publish_topic->text());
    config.insert("base_config",mqtt_base_config);
    config.insert("topic_config",mqtt_topic_config);
    QJsonDocument doc;
    doc.setObject(config);

    QFile file("./config.json");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "can't open error!";
        return;
    }
    QTextStream stream(&file);
    stream << doc.toJson();
    file.close();
}

void mqtt_win::load_config(QString file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can't open error!";
        return;
    }

    QTextStream stream(&file);
    QString str = stream.readAll();
    file.close();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError && !doc.isNull()) {
        qDebug() << "Json格式错误！" << jsonError.error;
        return;
    }
    QJsonObject config = doc.object();
    ui->lineEditClientid->setText(config["base_config"].toObject()["clientid"].toString());
    ui->lineEditUser->setText(config["base_config"].toObject()["username"].toString());
    ui->lineEditPassword->setText(config["base_config"].toObject()["password"].toString());
    ui->lineEditHost->setText(config["base_config"].toObject()["hostname"].toString());
    ui->spinBoxPort->setValue(config["base_config"].toObject()["port"].toInt());

    ui->subscribe_topic->setText(config["topic_config"].toObject()["subscribe_topic"].toString());
    ui->publish_topic->setText(config["topic_config"].toObject()["publish_topic"].toString());
}


void mqtt_win::on_startbutton_clicked()
{
    subscription = m_client->subscribe(ui->subscribe_topic->text(), static_cast<quint8>(0));
    if (!subscription)
    {
        QMessageBox::warning(this,"error","Could not subscribe. Is there a valid connection?");
    }
    connect(subscription,&QMqttSubscription::messageReceived,this,[=](const QMqttMessage &msg)
            {
        emit this->new_message(QString(msg.payload()));
    });
    emit this->started();
}


void mqtt_win::on_load_config_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("open a file."),
        "./",
        tr("config.json"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the video!");
    }
    else {
        this->load_config(fileName);
    }
}

