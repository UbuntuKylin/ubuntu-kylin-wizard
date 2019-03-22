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

#include "config.h"

#include <QGSettings/QGSettings>
#include <QVariant>

#define GSETTINGS_SCHEMA_WIZARD "org.ukui.wizard"
#define KEY_APP_LIST "app-list"

Config::Config(QObject *parent) : QObject(parent)
{
    gsettings = new QGSettings (GSETTINGS_SCHEMA_WIZARD, "", this);

    appList = gsettings->get(KEY_APP_LIST).toStringList();
}

QStringList Config::getAppList()
{
    return appList;
}

Config::~Config()
{
    delete gsettings;
}
