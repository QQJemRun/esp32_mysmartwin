QT       += core gui network widgets mqtt
QT += positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mqtt_win.cpp \
    widget.cpp

HEADERS += \
    mqtt_win.h \
    widget.h

FORMS += \
    mqtt_win.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ai_win_res.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD
DISTFILES += \
    AndroidManifest.xml \
    # config.json

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_EXTRA_LIBS =
}
android: include(/home/run/Android/Sdk/android_openssl/openssl.pri)
# android: include(./android_openssl/openssl.pri)
