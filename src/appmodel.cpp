// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/*
 * Copyright (C) 2019, National University of Defense Technology(NUDT) & Kylin Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: handsome_feng <jianfengli@ubuntukylin.com>
 */

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
        return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;// Qt::ItemIsSelectable;
}
bool AppModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            itemCheckStateMap[index.row() * 2 + index.column()] = Qt::Checked;
        } else {
            itemCheckStateMap[index.row() * 2 + index.column()] = Qt::Unchecked;
        }
    }
    return true;
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
//    case Qt::BackgroundRole:
//        if (row == 1 && col == 1)  //change background only for cell(1,2)
//            return QBrush(Qt::red);
//        break;
//    case Qt::TextAlignmentRole:
//        if (row == 2 && col == 0) //change text alignment only for cell(1,1)
//            return Qt::AlignRight + Qt::AlignVCenter;
//        break;
    case Qt::CheckStateRole: {
        if (itemCheckStateMap.contains(i))
                return itemCheckStateMap[i] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
        return Qt::Unchecked;
    }
    }
    return QVariant();
}
