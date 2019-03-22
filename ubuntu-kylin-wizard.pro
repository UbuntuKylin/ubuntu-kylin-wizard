QT       += core gui dbus sql network
CONFIG += c++11 link_pkgconfig
PKGCONFIG += gsettings-qt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ubuntu-kylin-wizard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/appmodel.cpp \
    src/appinfo.cpp \
    src/config.cpp

HEADERS += \
    src/mainwindow.h \
    src/appmodel.h \
    src/appinfo.h \
    src/config.h

FORMS += \
    src/mainwindow.ui

TRANSLATIONS += \
    po/zh_CN.ts

isEmpty(PREFIX){
    PREFIX = /usr
}

target.path = $${PREFIX}/bin/

RESOURCES += \
    wizard.qrc

desktop.files = data/ubuntu-kylin-wizard.desktop
desktop.path = /etc/xdg/autostart/

gsettings.files = data/org.ukui.wizard.gschema.xml
gsettings.path = /usr/share/glib-2.0/schemas/

INSTALLS += target desktop gsettings
