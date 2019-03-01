// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/*
 * Copyright (C) 2015, National University of Defense Technology(NUDT) & Kylin Ltd
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

#ifndef WIZARD_SETTINGS_H
#define WIZARD_SETTINGS_H

#include <memory>
#include <gtk/gtk.h>

#include "EMConverter.h"

enum class LauncherPosition
{
    LEFT = 0,
    BOTTOM
};

class Settings
{
public:
    Settings();
    ~Settings();

    static Settings& Instance();
    EMConverter::Ptr const& em() const;

    int LauncherSize() const;
    int PanelHeight() const;
    int IconSize() const;
    int SpaceBetweenIcons() const;
    LauncherPosition GetLauncherPosition() const;

private:
    class Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif
