#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <mqtt_win.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    // void on_ch_win_state_clicked();

    void on_auto_location_clicked();

    void update_message();
    void win_state_ch();

    void on_set_threshold_clicked();

    void on_set_open_timer_clicked();

    // void on_ch_fans_state_clicked();

    void on_ch_fans_state_clicked(bool checked);

    void on_ch_win_state_clicked(bool checked);

    void on_set_open_timer_2_clicked();

private:
    Ui::Widget *ui;

    int win_state = 0;
    int fans_state = 0;
    int aqi_threshold_sys = 1;
    int location_set_sys = 0;
    float co_threshold = 0.0;
    float boom_threshold = 0.0;
    float ch4_threshold = 0.0;

    void permissionUpdated();
    mqtt_win *mqtt_w;


};
#endif // WIDGET_H
