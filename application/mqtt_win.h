#ifndef MQTT_WIN_H
#define MQTT_WIN_H

#include <QWidget>
#include <QTcpSocket>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

namespace Ui {
class mqtt_win;
}

class mqtt_win : public QWidget
{
    Q_OBJECT

public:
    explicit mqtt_win(QWidget *parent = nullptr);
    ~mqtt_win();
    QMqttSubscription *subscription;

    void send_message(QString message);

signals:
    void started();
    void new_message(QString msg);

private slots:
    void on_buttonConnect_clicked();
    void on_topic_set_clicked();
    void save_config();
    void load_config(QString file_name="./config.json");

    // void on_pushButton_clicked();

    // void update_message();
    void on_startbutton_clicked();


    void on_load_config_clicked();

private:
    Ui::mqtt_win *ui;
    QMqttClient *m_client;
    QString subscribe_topic;
    QString publish_topic;

};

#endif // MQTT_WIN_H
