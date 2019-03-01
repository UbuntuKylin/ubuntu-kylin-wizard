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

#include <gio/gio.h>
#include <gdk/gdk.h>

#include "WizardSettings.h"
#include "RawPixel.h"

namespace
{
Settings *settings_instance = nullptr;

const std::string UNITY_GSETTINGS_SCHEMA = "org.compiz.unityshell";
const std::string UNITY_GSETTINGS_PATH = "/org/compiz/profiles/unity/plugins/unityshell/";
const std::string ICON_SIZE = "icon-size";

const std::string LAUNCHER_SETTINGS = "com.canonical.Unity.Launcher";
//const std::string FAVORITES = "favorites";
const std::string LAUNCHER_POTISION = "launcher-position";

const std::string UBUNTU_UI_SETTINGS = "com.ubuntu.user-interface";
const std::string SCALE_FACTOR = "scale-factor";

const RawPixel DEFAULT_ICON_SIZE  = 48_em;
const RawPixel DEFAULT_ICON_SIZE_DELTA =  6_em;
const RawPixel SIDE_LINE_WIDTH = 1_em;
const RawPixel PANEL_HEIGHT = 24_em;
const RawPixel SPACE_BETWEEN_ICONS = 5_em;
const RawPixel ICON_PADDING = 6_em;

const int DEFAULT_LAUNCHER_SIZE = 64;

const double DEFAULT_DPI = 96.0f;
}

class Settings::Impl
{
public:
    Impl(Settings* owner)
        : parent_(owner)
        , unity_settings_(g_settings_new_with_path(UNITY_GSETTINGS_SCHEMA.c_str(), UNITY_GSETTINGS_PATH.c_str()))
        , ubuntu_ui_settings_(g_settings_new(UBUNTU_UI_SETTINGS.c_str()))
        , launcher_settings_(g_settings_new(LAUNCHER_SETTINGS.c_str()))
        , launcher_size_(DEFAULT_LAUNCHER_SIZE)
        , launcher_position_(LauncherPosition::BOTTOM)
        , cv_(new EMConverter())
    {
        UpdateDPI();
        launcher_position_ = static_cast<LauncherPosition>(g_settings_get_enum(launcher_settings_, LAUNCHER_POTISION.c_str()));

        RawPixel icon_size = g_settings_get_int(unity_settings_, ICON_SIZE.c_str()) + DEFAULT_ICON_SIZE_DELTA;
        icon_size_ = icon_size.CP(cv_);

        RawPixel launcher_size = icon_size + 2 * ICON_PADDING + SIDE_LINE_WIDTH - 2;
        launcher_size_ = launcher_size.CP(cv_) - (1_em).CP(cv_);
        panel_height_ = PANEL_HEIGHT.CP(cv_);
        space_between_icons_ = SPACE_BETWEEN_ICONS.CP(cv_);
    }

    Settings *parent_;
    GSettings *unity_settings_;
    GSettings *launcher_settings_;
    GSettings *ubuntu_ui_settings_;
    LauncherPosition launcher_position_;
    int launcher_size_;
    int panel_height_;
    int icon_size_;
    int space_between_icons_;
    EMConverter::Ptr cv_;


    void UpdateDPI()
    {
      double min_scale = 4.0;
      double max_scale = 0.0;

      GVariant* dict;
      g_settings_get(ubuntu_ui_settings_, SCALE_FACTOR.c_str(), "@a{si}", &dict);

      int dpi = DEFAULT_DPI;

      gchar* monitor_name = gdk_screen_get_monitor_plug_name(gdk_screen_get_default(), 0);
      double ui_scale = 1.0f;
      int value;

      if (g_variant_lookup(dict, monitor_name, "i", &value) && value > 0)
        ui_scale = static_cast<double>(value)/8.0f;

      dpi = DEFAULT_DPI * ui_scale;
      min_scale = std::min(min_scale, ui_scale);
      max_scale = std::max(max_scale, ui_scale);

      cv_->SetDPI(dpi);
    }
};

Settings::Settings()
    : pimpl(new Impl(this))
{
    if (settings_instance)
    {
        g_error("More than one Settings created.");
        return;
    }

    settings_instance = this;
}

Settings::~Settings()
{
    if (settings_instance == this)
        settings_instance = nullptr;
}

Settings& Settings::Instance()
{
    if (!settings_instance)
    {
        g_error("No Settings created yet.");
    }

    return *settings_instance;
}

int Settings::LauncherSize() const
{
    return pimpl->launcher_size_;
}

int Settings::PanelHeight() const
{
    return pimpl->panel_height_;
}

int Settings::IconSize() const
{
    return pimpl->icon_size_;
}

int Settings::SpaceBetweenIcons() const
{
    return pimpl->space_between_icons_;
}

LauncherPosition Settings::GetLauncherPosition() const
{
    return pimpl->launcher_position_;
}

