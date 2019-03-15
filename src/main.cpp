#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QStandardPaths>

bool isLiveCD()
{
    QFile file("/proc/cmdline");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "error";
        return true;
    }
    QTextStream in(&file);
    QString line = in.readAll();
    if (line.contains("casper"))
        return true;
    else
        return false;
}

int main(int argc, char *argv[])
{
    if (isLiveCD()) {
        qDebug() << "Do not start in live CD!";
        return 0;
    }

    Q_INIT_RESOURCE(wizard);

    QApplication a(argc, argv);

    QTranslator wizardTranslator;
    if (wizardTranslator.load(QLocale::system().name(), ":/i18n/po")) {
        a.installTranslator(&wizardTranslator);
    } else {
        qDebug() << "Faild to load " << QLocale::system().name() << ".qm !";
    }

    QFile styleFile(":/style/data/wizard.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    // This should be after i18n
    MainWindow w;
    w.show();

    return a.exec();
}
