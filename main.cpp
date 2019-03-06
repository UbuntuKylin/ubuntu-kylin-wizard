#include "mainwindow.h"
#include <QApplication>
#include <QDBusConnection>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCore/QDebug>
#include <QtDBus/QtDBus>

void method ()
{
    QDBusConnection bus = QDBusConnection::systemBus();
    if (!bus.isConnected())
    {
        fprintf(stderr, "Cannot connect to the D-Bus session bus.");
        return;
    }
    QDBusInterface dbus_iface("com.ubuntukylin.softwarecenter", "/",
                              "com.ubuntukylin.softwarecenter", bus);
    if (dbus_iface.isValid()) {
        QDBusReply<QStringList> reply = dbus_iface.call("get_sources", QVariant(true));
        if (reply.isValid()) {
            for (int i = 0; i < reply.value().size(); ++i)
                qDebug() << reply.value().at(i);
        } else {
            fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        }
    } else {
        fprintf (stderr, "%s\n",
                 qPrintable(QDBusConnection::systemBus().lastError().message()));
    }
}

void get_summary (QString app)
{
    assert(!app.isEmpty());
    QString query_string = QString("select summary from application where app_name = '%1'").arg(app);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/usr/share/ubuntu-kylin-software-center/data/uksc.db");
    if (!db.open()) {
        fprintf (stderr, "Open db failed: %s\n", qPrintable(db.lastError().text()));
    }

    QSqlQuery query(db);
    query.prepare(query_string);
    if (!query.exec()) {
        fprintf (stderr, "Exec query failed: %s\n", qPrintable(db.lastError().text()));
    }
    while (query.next()) {
        qDebug() << query.value(0).toString();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    get_summary("wps-office");
    method();
    MainWindow w;
    w.show();

    return a.exec();
}
