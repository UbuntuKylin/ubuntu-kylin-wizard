#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QTextStream>

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
        qDebug() << "Don't start in live CD!";
        return 0;
    }

    QApplication a(argc, argv);
    MainWindow w;

    QFile styleFile(":/style/wizard.qss");
    styleFile.open(QFile::ReadOnly);

    QString style(styleFile.readAll());
    a.setStyleSheet(style);

    w.show();

    return a.exec();
}
