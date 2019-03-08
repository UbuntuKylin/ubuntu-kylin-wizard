#ifndef APPINFO_H
#define APPINFO_H

#include <QString>
#include <QSqlDatabase>
#include <QIcon>

class AppInfo
{
public:
    AppInfo(QString app_name);
    QString getName() const;
    QIcon   getIcon() const;
    QString getSummary() const;

private:
    QString fetchSummary();
    bool fileExist(QString file_name);

    static QSqlDatabase db;

    QString name;
    QIcon   icon;
    QString summary;
};

#endif // APPINFO_H
