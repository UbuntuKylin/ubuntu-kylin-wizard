#include <QFont>
#include <QBrush>
#include <QIcon>
#include <QDebug>
#include "appmodel.h"

AppModel::AppModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int AppModel::rowCount(const QModelIndex & /*parent*/) const
{
    return 10;
}

int AppModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
//    qDebug() << QString("row %1, col %2, role %3")
//                .arg(row).arg(col).arg(role);

    switch (role) {
    case Qt::DisplayRole:
        return QString("Row%1, Column%2")
                .arg(row + 1)
                .arg(col +1);
    case Qt::DecorationRole:
        return QIcon(":/wechat.png");
    case Qt::FontRole:
        if (row == 0 && col == 0) { //change font only for cell(0,0)
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:
        if (row == 1 && col == 1)  //change background only for cell(1,2)
            return QBrush(Qt::red);
        break;
    case Qt::TextAlignmentRole:
        if (row == 2 && col == 0) //change text alignment only for cell(1,1)
            return Qt::AlignRight + Qt::AlignVCenter;
        break;
    case Qt::CheckStateRole:
        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
            return Qt::Checked;
        break;
    }
    return QVariant();
}
