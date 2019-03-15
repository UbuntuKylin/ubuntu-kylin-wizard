#include <QFont>
#include <QBrush>
#include <QIcon>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "appmodel.h"
#include "appinfo.h"

static QStringList APP_LIST = {"wps-office", "sogoupinyin", "foxitreader"};

AppModel::AppModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    for (int i = 0; i < APP_LIST.size(); ++i) {
        app_list.append(AppInfo(APP_LIST[i]));
    }
}

int AppModel::rowCount(const QModelIndex & /*parent*/) const
{
    return app_list.length() / 2 + 1;
}

int AppModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 2;
}

Qt::ItemFlags AppModel::flags(const QModelIndex &index) const
{
    if (index.row() * 2 + index.column() == app_list.length())
        return Qt::NoItemFlags;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    int i = row * 2 + col;
    if (i >= app_list.length())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole: {
        return QString(tr("%1\n%2"))
                .arg(app_list[i].getName())
                .arg(app_list[i].getSummary());
    }
    case Qt::DecorationRole: {
        return app_list[i].getIcon();
    }
    case Qt::FontRole: {
            QFont boldFont;
            boldFont.setPixelSize(14);
            return boldFont;
    }
    case Qt::BackgroundRole:
        if (row == 1 && col == 1)  //change background only for cell(1,2)
            return QBrush(Qt::red);
        break;
    case Qt::TextAlignmentRole:
        if (row == 2 && col == 0) //change text alignment only for cell(1,1)
            return Qt::AlignRight + Qt::AlignVCenter;
        break;
//    case Qt::CheckStateRole: {
//        return Qt::Unchecked;
//    }
    }
    return QVariant();
}
