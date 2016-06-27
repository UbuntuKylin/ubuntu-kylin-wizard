// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
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

#include "style.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string>
#include <memory>
#include <fstream>

const std::string LAUNCHER_SETTINGS = "com.canonical.Unity.Launcher";
const std::string FAVORITES = "favorites";

const std::string UBUNTU_UI_SETTINGS = "com.ubuntu.user-interface";
const std::string SCALE_FACTOR = "scale-factor";

const std::string UNITY_GSETTINGS_SCHEMA = "org.compiz.unityshell";
const std::string UNITY_GSETTINGS_PATH = "/org/compiz/profiles/unity/plugins/unityshell/";
const std::string ICON_SIZE = "icon-size";

const std::string BACKGROUND_SETTINGS = "org.gnome.desktop.background";
const std::string PICTURE_URI = "picture-uri";

const gint MID_LAUNCHER = -2;
const gint UNDER_PANEL = -3;

const RawPixel DEFAULT_ICON_SIZE  = 48_em;
const RawPixel DEFAULT_ICON_SIZE_DELTA =  6_em;
const RawPixel SIDE_LINE_WIDTH = 1_em;
const RawPixel PANEL_HEIGHT = 24_em;

const gint ARROW_HEIGHT = 80;
const gint ARROW_WIDTH  = 33;
const gint PAGE_IND_HEIGTH = 22;
const gint PAGE_IND_WIDTH = 253;

const double DEFAULT_DPI = 96.0f;

void Style::UpdateDPI()
{
  GSettings *ubuntu_ui_settings = g_settings_new(UBUNTU_UI_SETTINGS.c_str());
  double min_scale = 4.0;
  double max_scale = 0.0;

  GVariant* dict;
  g_settings_get(ubuntu_ui_settings, SCALE_FACTOR.c_str(), "@a{si}", &dict);

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

Style::Style()
  : unity_settings_(g_settings_new_with_path(UNITY_GSETTINGS_SCHEMA.c_str(), UNITY_GSETTINGS_PATH.c_str()))
  , launcher_settings_(g_settings_new(LAUNCHER_SETTINGS.c_str()))
  , background_settings_(g_settings_new(BACKGROUND_SETTINGS.c_str()))
  , cv_(new EMConverter())
{
  UpdateDPI();

  GdkScreen *screen = gdk_screen_get_default();
  screen_width_ = gdk_screen_get_width(screen);
  screen_height_ = gdk_screen_get_height(screen);

  gint primary_monitor = gdk_screen_get_primary_monitor(screen);
  GdkRectangle geo;
  gdk_screen_get_monitor_geometry(screen, primary_monitor, &geo);
  pri_monitor_width_ = geo.width;
  pri_monitor_height_ = geo.height;

  background_url_ = g_settings_get_string(background_settings_, PICTURE_URI.c_str());

  icon_size_ = g_settings_get_int(unity_settings_, ICON_SIZE.c_str()) + DEFAULT_ICON_SIZE_DELTA;
  RawPixel launcher_size = icon_size_ + 2 * ICON_PADDING + SIDE_LINE_WIDTH - 2;
  launcher_size_ = launcher_size.CP(cv_) - (1_em).CP(cv_);
  panel_height_= PANEL_HEIGHT.CP(cv_);

  left_arrow_pos_.x = 0.05 * (pri_monitor_width_ - ARROW_WIDTH);
  left_arrow_pos_.y = 0.5 * (pri_monitor_height_ - ARROW_HEIGHT);

  right_arrow_pos_.x = 0.95 * (pri_monitor_width_ - ARROW_WIDTH);
  right_arrow_pos_.y = 0.5 * (pri_monitor_height_ - ARROW_HEIGHT);

  base_pos_.x = 0.125 * pri_monitor_width_;
  base_pos_.y = 0.25 * pri_monitor_height_;

  title_pos_.x = 0.5 * pri_monitor_width_;
  title_pos_.y = 0.25 * pri_monitor_height_;

  subtitle_pos_.x = title_pos_.x;
  subtitle_pos_.y = 0.35 * pri_monitor_height_;

  details_pos_.x = title_pos_.x;
  details_pos_.y = 0.4 * pri_monitor_height_;

  close_pos_.x = 0.5 * pri_monitor_width_;
  close_pos_.y = 0.9 * (pri_monitor_height_ - launcher_size_);

  page_ind_pos_.x = 0.75 * pri_monitor_width_;
  page_ind_pos_.y = close_pos_.y + (CLOSE_BUTTON_HEIGHT - PAGE_IND_HEIGTH) / 2;

  spot_pos_.x = (base_pos_.x + 422 + title_pos_.x) / 2;
  spot_pos_.y = 0.25 * pri_monitor_height_;

  inflexion_pos_.x = spot_pos_.x;
  inflexion_pos_.y = close_pos_.y + CLOSE_BUTTON_HEIGHT + 5;

  icon_order_[0] = MID_LAUNCHER;
  icon_order_[1] = 1;
  // +1 because of dash is not in the favorites list.
  icon_order_[2] = get_icon_order("Nautilus") + 1;
  icon_order_[3] = get_icon_order("youker-assistant") + 1;
  icon_order_[4] = get_icon_order("unity-control-center") + 1;
  icon_order_[5] = UNDER_PANEL;

  Point p;
  for (int i = 0; i < PAGES_NUM; i++)
  {
    p = cal_icon_position(icon_order_[i]);
    icon_pos_[i].x = p.x;
    icon_pos_[i].y = p.y;
  }
}

gint Style::get_screen_width()
{
  return screen_width_;
}

gint Style::get_screen_height()
{
  return screen_height_;
}

gint Style::get_pri_monitor_width()
{
  return pri_monitor_width_;
}

gint Style::get_pri_monitor_height()
{
  return pri_monitor_height_;
}

std::string Style::get_background_url()
{
  return background_url_.substr(6);
}

gint Style::get_launcher_size()
{
  return launcher_size_;
}

gint Style::get_panel_height()
{
  return panel_height_;
}

gint Style::get_icon_size()
{
  return icon_size_.CP(cv_);
}

gint Style::get_icon_order(std::string icon_name)
{
  gint n = 0;
  gint adjust = 0;
  gboolean found = false;

  std::unique_ptr<gchar*[], void(*)(gchar**)> strings(g_settings_get_strv(launcher_settings_, FAVORITES.c_str()), g_strfreev);

  for (int i = 0; strings[i]; ++i)
  {
    std::string value = strings[i];

    if (value.find("devices") != std::string::npos || value.find("running") != std::string::npos ||
        value.find("expo-icon") != std::string::npos || value.find("ubiquity") != std::string::npos)
      adjust--;

    if (!value.empty() && value.find(icon_name) != std::string::npos)
    {
      found = true;
      n = i + 1;
      break;
    }
  }

  if (!found)
  {
    n = -1;
    adjust = 0;
  }
  return n + adjust;
}

Point Style::get_icon_position(int index)
{
  return icon_pos_[index];
}

Point Style::cal_icon_position(int index)
{
  Point icon_pos;

  if (index == MID_LAUNCHER)
  {
    icon_pos.x = spot_pos_.x;
    icon_pos.y = pri_monitor_height_ - launcher_size_;
  }
  else if (index == UNDER_PANEL)
  {
    icon_pos.x = pri_monitor_width_ - 100;
    icon_pos.y = panel_height_;
  }
  else
  {
    icon_pos.x = (index - 1) * (icon_size_.CP(cv_) + SPACE_BETWEEN_ICONS.CP(cv_)) + SPACE_BETWEEN_ICONS.CP(cv_) + 0.5 * icon_size_.CP(cv_);
    icon_pos.y = pri_monitor_height_ - launcher_size_;
  }

  return icon_pos;
}

Point Style::get_left_arrow_pos()
{
  return left_arrow_pos_;
}

Point Style::get_right_arrow_pos()
{
  return right_arrow_pos_;
}

Point Style::get_base_pos()
{
  return base_pos_;
}

Point Style::get_title_pos()
{
  return title_pos_;
}

Point Style::get_subtitle_pos()
{
  return subtitle_pos_;
}

Point Style::get_details_pos()
{
  return details_pos_;
}

Point Style::get_close_pos()
{
  return close_pos_;
}

Point Style::get_page_ind_pos()
{
  return page_ind_pos_;
}

Point Style::get_spot_pos()
{
  return spot_pos_;
}

Point Style::get_inflexion_pos()
{
  return inflexion_pos_;
}
