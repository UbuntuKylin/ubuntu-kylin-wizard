#ifndef APPMODEL_H
#define APPMODEL_H

#include <QAbstractTableModel>

class AppModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    AppModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // APPMODEL_H
