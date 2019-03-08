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

QVariant AppModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    int i = row * 2 + col;
    if (i >= app_list.length())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole: {
        return QString("%1\n%2")
                .arg(app_list[i].getName())
                .arg(app_list[i].getSummary());
    }
    case Qt::DecorationRole: {
        return app_list[i].getIcon();
    }
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
