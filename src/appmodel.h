#ifndef APPMODEL_H
#define APPMODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>

#include "appinfo.h"

class AppModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AppModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QList<AppInfo> app_list;
    QMap<int, Qt::CheckState> itemCheckStateMap;
};

#endif // APPMODEL_H
