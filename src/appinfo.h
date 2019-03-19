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
