#include "widget.h"
#include "ui_widget.h"
#include <QGeoCoordinate>
#include <QGeoPositionInfoSource>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPermission>



QString BASE_URL = "https://api.caiyunapp.com/v2.6/fuwhPNzXsYXIvMya/";


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground);
    mqtt_w = new mqtt_win;
    this->hide();
    mqtt_w->show();
    connect(mqtt_w,&mqtt_win::started,this,[=]()
            {
        this->show();
        mqtt_w->hide();
    });
    connect(mqtt_w,&mqtt_win::new_message,this,[=](QString msg)
            {
        qDebug()<<msg;
        ui->air_in_the_room->clear();
        ui->air_out_of_the_room->clear();
        QJsonDocument doc = QJsonDocument::fromJson(msg.toLocal8Bit().data());
        QJsonObject air_in = QJsonDocument::fromJson(doc.object()["items"].toObject()["aqi_in_the_room"].toObject()["value"].toString().toLocal8Bit().data()).object();

        for(auto key : air_in.keys())
        {
            if(air_in[key].isDouble())
                ui->air_in_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+QString::number(air_in[key].toDouble()));
            else if(air_in[key].isString())
                ui->air_in_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+air_in[key].toString());
            else
                ui->air_in_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+QString::number(air_in[key].toInt()));
        };

        QJsonObject air_out = QJsonDocument::fromJson(doc.object()["items"].toObject()["aqi_out_of_door"].toObject()["value"].toString().toLocal8Bit().data()).object();

        for(auto key : air_out.keys())
        {
            if(air_out[key].isDouble())
                ui->air_out_of_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+QString::number(air_out[key].toDouble()));
            else if(air_out[key].isString())
                ui->air_out_of_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+air_out[key].toString());
            else if(air_out[key].isObject())
            {
                if(air_out[key].toObject()["chn"].isString())
                    ui->air_out_of_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+air_out[key].toObject()["chn"].toString());
                else
                    ui->air_out_of_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+QString::number(air_out[key].toObject()["chn"].toInt()));
            }
            else
                ui->air_out_of_the_room->append(QString("%1").arg(key, 12, QLatin1Char(' '))+'\t'+QString::number(air_out[key].toInt()));
        };
        qDebug()<<this->win_state<<"\t"<<doc.object()["items"].toObject()["win_state"].toObject()["value"].toInt();
        if (this->win_state == doc.object()["items"].toObject()["win_state"].toObject()["value"].toInt()&&this->fans_state == doc.object()["items"].toObject()["fans_state"].toObject()["value"].toInt())
        {
            ui->ch_win_state->setEnabled(true);
            ui->ch_fans_state->setEnabled(true);
        }

        else
        {
            this->win_state = doc.object()["items"].toObject()["win_state"].toObject()["value"].toInt();
            this->fans_state = doc.object()["items"].toObject()["fans_state"].toObject()["value"].toInt();
            // this->win_state = doc.object()["items"].toObject()["win_state"].toObject()["value"].toInt();
            // ui->ch_win_state->click();
            emit ui->ch_win_state->clicked(true);
            emit ui->ch_fans_state->clicked(true);
        }
        // if ()
        // {

        //     // ui->ch_fans_state->clicked(true);
        // }
        // else
        // {
        //     this->fans_state = doc.object()["items"].toObject()["fans_state"].toObject()["value"].toInt();
        //     // if(this->fans_state == 1)
        //     emit ui->ch_fans_state->clicked(true);
        //     // else(this->fans_state == 0)
        //     //     emit ui->ch_fans_state->clicked(false);
        // }

        QJsonObject aqi_threshold = QJsonDocument::fromJson(doc.object()["items"].toObject()["aqi_threshold"].toObject()["value"].toString().toLocal8Bit().data()).object();
        if(!aqi_threshold.isEmpty())
        {
            qDebug()<<this->aqi_threshold_sys<<"\t"<<aqi_threshold["aqi_threshold_sys"];
            if (this->aqi_threshold_sys == aqi_threshold["aqi_threshold_sys"].toInt())
            {
                ui->set_threshold->setEnabled(true);
            }
            else
            {
                this->aqi_threshold_sys = aqi_threshold["aqi_threshold_sys"].toInt();
                this->co_threshold = aqi_threshold["co_threshold"].toDouble();
                this->boom_threshold = aqi_threshold["boom_threshold"].toDouble();
                this->ch4_threshold = aqi_threshold["ch4_threshold"].toDouble();
                ui->co_threshold->setText(QString::number(this->co_threshold));
                ui->boom_threshold->setText(QString::number(this->boom_threshold));
                ui->ch4_threshold->setText(QString::number(this->ch4_threshold));
            }
        }
        QJsonObject location_set = QJsonDocument::fromJson(doc.object()["items"].toObject()["location_set"].toObject()["value"].toString().toLocal8Bit().data()).object();
        if(!aqi_threshold.isEmpty())
        {
            qDebug()<<this->location_set_sys<<"\t"<<location_set["location_set_sys"];
            if (this->location_set_sys == location_set["location_set_sys"].toInt())
            {
                ui->auto_location->setEnabled(true);
            }
            else
            {
                ui->longitude->setText(QString::number(location_set["longitude"].toDouble()));
                ui->latitude->setText(QString::number(location_set["latitude"].toDouble()));
            }
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

// void Widget::on_ch_win_state_clicked()
// {
//     emit ui->ch_win_state->clicked(true);
// }


void Widget::win_state_ch()
{

    return;
}



void Widget::on_auto_location_clicked()
{
    if(this->location_set_sys == 0)
        this->location_set_sys = 1;
    else if(this->location_set_sys == 1)
        this->location_set_sys = 0;

    // QLocationPermission locationPermission;
    // locationPermission.setAccuracy(QLocationPermission::Precise);
    // if(qApp->checkPermission(locationPermission) != Qt::PermissionStatus::Granted) {
    //     qApp->requestPermission(QLocationPermission{}, this, &Widget::permissionUpdated);
    // } else {
    //     qDebug() << "PERMISSION ALREADY GRANTED";
    // }
    QGeoPositionInfoSource *source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source) {
        source->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
        // qDebug()<<"source";
        connect(source, &QGeoPositionInfoSource::positionUpdated,this,[=](const QGeoPositionInfo &info)
                {
                    qDebug()<<info;
                    ui->longitude->setText(QString::number(info.coordinate().longitude()));
                    ui->latitude->setText(QString::number(info.coordinate().latitude()));
                    QJsonObject message,params,location_set;
                    location_set.insert("location_set_sys",this->location_set_sys);
                    location_set.insert("longitude",info.coordinate().longitude());
                    location_set.insert("latitude",info.coordinate().latitude());
                    params.insert("aqi_out_of_door", "");
                    params.insert("aqi_in_the_room", "");
                    params.insert("win_state",this->win_state);
                    params.insert("fans_state",this->fans_state);
                    params.insert("humidity",0);
                    params.insert("temperature",0);
                    params.insert("openning_time",-1);
                    params.insert("openning_time_fans",-1);
                    params.insert("aqi_threshold","");
                    params.insert("location_set",QString(QJsonDocument(location_set).toJson(QJsonDocument::Compact)));
                    message.insert("params",params);
                    message.insert("version","1.0.0");
                    mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
                    qDebug()<<"info";
                    source->stopUpdates();
                });
        source->requestUpdate();
    }
    else
    {
        qDebug()<<"defeat";
    }
    // ui->auto_location->setEnabled(false);
}

void Widget::update_message()
{

}

void Widget::on_set_open_timer_clicked()
{
    QJsonObject message,params;
    params.insert("aqi_out_of_door", "");
    params.insert("aqi_in_the_room", "");
    params.insert("win_state",this->win_state);
    params.insert("fans_state",this->fans_state);
    params.insert("humidity",0);
    params.insert("temperature",0);
    params.insert("openning_time",ui->openning_time->value());
    params.insert("openning_time_fans",-1);
    params.insert("aqi_threshold","");
    params.insert("location_set","");
    message.insert("params",params);
    message.insert("version","1.0.0");
    mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
}

void Widget::permissionUpdated()
{

}

void Widget::on_set_threshold_clicked()
{
    if(this->aqi_threshold_sys == 0)
        this->aqi_threshold_sys = 1;
    else if(this->aqi_threshold_sys == 1)
        this->aqi_threshold_sys = 0;
    QJsonObject message,params,aqi_threshold;
    aqi_threshold.insert("aqi_threshold_sys",this->aqi_threshold_sys);
    aqi_threshold.insert("co_threshold",ui->co_threshold->text().toFloat());
    aqi_threshold.insert("boom_threshold",ui->boom_threshold->text().toFloat());
    aqi_threshold.insert("ch4_threshold",ui->ch4_threshold->text().toFloat());
    params.insert("aqi_out_of_door", "");
    params.insert("aqi_in_the_room", "");
    params.insert("win_state",this->win_state);
    params.insert("fans_state",this->fans_state);
    params.insert("humidity",0);
    params.insert("temperature",0);
    params.insert("openning_time",-1);
    params.insert("openning_time_fans",-1);
    params.insert("aqi_threshold",QString(QJsonDocument(aqi_threshold).toJson(QJsonDocument::Compact)));
    params.insert("location_set","");
    message.insert("params",params);
    message.insert("version","1.0.0");
    mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
    ui->set_threshold->setEnabled(false);
}

void Widget::on_ch_fans_state_clicked(bool checked)
{
    if(!checked)
    {
        if(this->fans_state == 0)
        {
            this->fans_state = 1;
            ui->fans_icon->setStyleSheet("image: url(:/fans_2.png);background-color: rgba(87, 227, 137,150);border-radius:20px;");
            ui->ch_fans_state->setText("停止通风");
        }
        else
        {
            ui->fans_icon->setStyleSheet("image: url(:/fans_1.png);background-color: rgba(98, 160, 234,150);border-radius:20px;");
            ui->ch_fans_state->setText("开始通风");
            this->fans_state = 0;
        }
        QJsonObject message,params;
        params.insert("aqi_out_of_door", "");
        params.insert("aqi_in_the_room", "");
        params.insert("win_state",this->win_state);
        params.insert("fans_state",this->fans_state);
        params.insert("temperature",0);
        params.insert("openning_time",-1);
        params.insert("openning_time_fans",-1);
        params.insert("aqi_threshold","");
        message.insert("params",params);
        message.insert("version","1.0.0");
        mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
        ui->ch_fans_state->setEnabled(false);

    }
    else
    {
        if(this->fans_state == 0)
        {
            // this->fans_state = 1;
            ui->fans_icon->setStyleSheet("image: url(:/fans_1.png);background-color: rgba(98, 160, 234,150);border-radius:20px;");
            ui->ch_fans_state->setText("开始通风");
        }
        else
        {
            ui->fans_icon->setStyleSheet("image: url(:/fans_2.png);background-color: rgba(87, 227, 137,150);border-radius:20px;");
            ui->ch_fans_state->setText("停止通风");

            // this->ch_fans_state = 0;
        }
        QJsonObject message,params;
        params.insert("aqi_out_of_door", "");
        params.insert("aqi_in_the_room", "");
        params.insert("win_state",this->win_state);
        params.insert("fans_state",this->fans_state);
        params.insert("temperature",0);
        params.insert("openning_time",-1);
        params.insert("openning_time_fans",-1);
        params.insert("aqi_threshold","");
        message.insert("params",params);
        message.insert("version","1.0.0");
        mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
        ui->ch_fans_state->setEnabled(false);

    }

}


void Widget::on_ch_win_state_clicked(bool checked)
{
    if(!checked)
    {
        if(this->win_state == 0)
        {
            this->win_state = 1;
            ui->win_icon->setStyleSheet("image: url(:/win_openning.png);background-color: rgba(87, 227, 137,150);border-radius:20px;");
            ui->ch_win_state->setText("关窗");
        }
        else
        {
            ui->win_icon->setStyleSheet("image: url(:/win_closing.png);background-color: rgba(98, 160, 234,150);border-radius:20px;");
            ui->ch_win_state->setText("开窗");
            this->win_state = 0;
        }
        QJsonObject message,params;
        params.insert("aqi_out_of_door", "");
        params.insert("aqi_in_the_room", "");
        params.insert("win_state",this->win_state);
        params.insert("fans_state",this->fans_state);
        params.insert("temperature",0);
        params.insert("openning_time",-1);
        params.insert("openning_time_fans",-1);
        params.insert("aqi_threshold","");
        message.insert("params",params);
        message.insert("version","1.0.0");
        mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
        ui->ch_win_state->setEnabled(false);

    }
    else
    {
        if(this->win_state == 0)
        {
            ui->win_icon->setStyleSheet("image: url(:/win_closing.png);background-color: rgba(98, 160, 234,150);border-radius:20px;");
            ui->ch_win_state->setText("开窗");
            // this->win_state = 1;

        }
        else
        {
            ui->win_icon->setStyleSheet("image: url(:/win_openning.png);background-color: rgba(87, 227, 137,150);border-radius:20px;");
            ui->ch_win_state->setText("关窗");
            // this->win_state = 0;
        }
        QJsonObject message,params;
        params.insert("aqi_out_of_door", "开窗");
        params.insert("aqi_in_the_room", "开窗");
        params.insert("win_state",this->win_state);
        params.insert("fans_state",this->fans_state);
        params.insert("temperature",0);
        params.insert("openning_time",-1);
        params.insert("openning_time_fans",-1);
        params.insert("aqi_threshold","");
        message.insert("params",params);
        message.insert("version","1.0.0");
        mqtt_w->send_message(QString(QJsonDocument(message).toJson()));
        ui->ch_win_state->setEnabled(false);

    }
}


void Widget::on_set_open_timer_2_clicked()
{
    QJsonObject message,params;
    params.insert("aqi_out_of_door", "");
    params.insert("aqi_in_the_room", "");
    params.insert("win_state",this->win_state);
    params.insert("fans_state",this->fans_state);
    params.insert("humidity",0);
    params.insert("temperature",0);
    params.insert("openning_time",-1);
    params.insert("openning_time_fans",ui->openning_time_2->value());
    params.insert("aqi_threshold","");
    params.insert("location_set","");
    message.insert("params",params);
    message.insert("version","1.0.0");
    mqtt_w->send_message(QString(QJsonDocument(message).toJson()));

}

