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
const std::string POSITION = "launcher-position";

const std::string UBUNTU_UI_SETTINGS = "com.ubuntu.user-interface";
const std::string SCALE_FACTOR = "scale-factor";

const std::string UNITY_GSETTINGS_SCHEMA = "org.compiz.unityshell";
const std::string UNITY_GSETTINGS_PATH = "/org/compiz/profiles/unity/plugins/unityshell/";
const std::string ICON_SIZE = "icon-size";

const std::string BACKGROUND_SETTINGS = "org.gnome.desktop.background";
const std::string PICTURE_URI = "picture-uri";

const gint LAUNCHER = -2;
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

gboolean get_app_installed(const gchar *app_name)
{
  gchar *path = g_find_program_in_path(app_name);
  if (path)
    return true;
  else
    return false;
}


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

  launcher_position_ = g_settings_get_string(launcher_settings_, POSITION.c_str());

  if (!g_strcmp0(launcher_position_, "Bottom"))
  {
    left_arrow_pos_.x = 0.05 * (pri_monitor_width_ - ARROW_WIDTH);
    left_arrow_pos_.y = 0.5 * (pri_monitor_height_ - ARROW_HEIGHT);

    base_pos_.x = 0.125 * pri_monitor_width_;
    base_pos_.y = 0.25 * pri_monitor_height_;

    title_pos_.x = 0.5 * pri_monitor_width_;
    title_pos_.y = 0.25 * pri_monitor_height_;

    subtitle_pos_.x = title_pos_.x;
    subtitle_pos_.y = 0.33 * pri_monitor_height_;

    details_pos_.x = title_pos_.x;
    details_pos_.y = 0.4 * pri_monitor_height_;

    close_pos_.x = 0.5 * pri_monitor_width_;
    close_pos_.y = 0.9 * (pri_monitor_height_ - launcher_size_);

    spot_pos_.x = (base_pos_.x + 422 + title_pos_.x) / 2;
    spot_pos_.y = 0.25 * pri_monitor_height_;

    inflexion_pos_.x = spot_pos_.x;
    inflexion_pos_.y = close_pos_.y + CLOSE_BUTTON_HEIGHT + 5;
  }
  else
  {
    left_arrow_pos_.x = 0.05 * (pri_monitor_width_ - ARROW_WIDTH) + launcher_size_;
    left_arrow_pos_.y = 0.5 * (pri_monitor_height_ - ARROW_HEIGHT);

    base_pos_.x = 0.55 * pri_monitor_width_;
    base_pos_.y = 0.25 * pri_monitor_height_;

    spot_pos_.x = 0.5 * pri_monitor_width_;
    spot_pos_.y = 0.25 * pri_monitor_height_;

    title_pos_.x = 0.125 * pri_monitor_width_;
    title_pos_.y = spot_pos_.y;

    subtitle_pos_.x = title_pos_.x;
    subtitle_pos_.y = 0.33 * pri_monitor_height_;

    details_pos_.x = title_pos_.x;
    details_pos_.y = 0.4 * pri_monitor_height_;

    close_pos_.x = 0.125 * pri_monitor_width_;
    close_pos_.y = 0.9 * (pri_monitor_height_ - launcher_size_);

    inflexion_pos_.x = (launcher_size_ + title_pos_.x) / 2;
    inflexion_pos_.y = (panel_height_ + base_pos_.y) / 2;  // This is only use for the last page(indicator).
  }

  right_arrow_pos_.x = 0.95 * (pri_monitor_width_ - ARROW_WIDTH);
  right_arrow_pos_.y = 0.5 * (pri_monitor_height_ - ARROW_HEIGHT);

  page_ind_pos_.x = 0.75 * pri_monitor_width_;
  page_ind_pos_.y = close_pos_.y + (CLOSE_BUTTON_HEIGHT - PAGE_IND_HEIGTH) / 2;

  icon_order_[0] = LAUNCHER;
  icon_order_[1] = 0;
  icon_order_[2] = get_icon_order("Nautilus");
  if (get_app_installed("youker-assistant"))
    icon_order_[3] = get_icon_order("youker-assistant");
  else
    icon_order_[3] = get_icon_order("Software");
  icon_order_[4] = get_icon_order("unity-control-center");
  icon_order_[5] = UNDER_PANEL;

  Point p;
  GdkRectangle trans_geo;
  for (int i = 0; i < PAGES_NUM; i++)
  {
    p = cal_focus_point(icon_order_[i]);
    focus_pos_[i].x = p.x;
    focus_pos_[i].y = p.y;

    trans_geo = cal_trans_area(icon_order_[i]);
    trans_area_[i].x = trans_geo.x;
    trans_area_[i].y = trans_geo.y;
    trans_area_[i].width = trans_geo.width;
    trans_area_[i].height = trans_geo.height;
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

gchar* Style::get_launcher_position()
{
  return launcher_position_;
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
        value.find("ubiquity") != std::string::npos)
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
    return -1;
  }
  return n + adjust;
}

Point Style::get_icon_position(int index)
{
  return focus_pos_[index];
}

GdkRectangle Style::cal_trans_area(int index)
{
  GdkRectangle trans_area;
  if (index == LAUNCHER)
  {
    if (!g_strcmp0(launcher_position_, "Bottom"))
    {
      trans_area.x = 0;
      trans_area.y = pri_monitor_height_ - launcher_size_;
      trans_area.width = pri_monitor_width_;
      trans_area.height = launcher_size_;
    }
    else
    {
      trans_area.x = 0;
      trans_area.y = panel_height_;
      trans_area.width = launcher_size_;
      trans_area.height = pri_monitor_height_ - panel_height_;
    }
  }
  else if (index == UNDER_PANEL)
  {
    trans_area.x = 0;
    trans_area.y = 0;
    trans_area.width = pri_monitor_width_;
    trans_area.height = panel_height_;
  }
  else if (!g_strcmp0(launcher_position_, "Bottom"))
  {
    trans_area.x = index * (icon_size_.CP(cv_) + SPACE_BETWEEN_ICONS.CP(cv_)) + SPACE_BETWEEN_ICONS.CP(cv_);
    trans_area.y = pri_monitor_height_ - launcher_size_ + SPACE_BETWEEN_ICONS.CP(cv_);
    trans_area.width = trans_area.height = icon_size_.CP(cv_);
  }
  else
  {
    trans_area.x = SPACE_BETWEEN_ICONS.CP(cv_);
    trans_area.y = panel_height_ + index * (icon_size_.CP(cv_) + SPACE_BETWEEN_ICONS.CP(cv_)) + SPACE_BETWEEN_ICONS.CP(cv_);
    trans_area.width = icon_size_.CP(cv_);
    trans_area.height = icon_size_.CP(cv_);
  }

  return trans_area;
}

Point Style::cal_focus_point(int index)
{
  Point focus;

  if (index == LAUNCHER)
  {
    if (!g_strcmp0(launcher_position_, "Bottom"))
    {
      focus.x = spot_pos_.x;
      focus.y = pri_monitor_height_ - launcher_size_;
    }
    else
    {
      focus.x = launcher_size_;
      focus.y = spot_pos_.y;
    }
  }
  else if (index == UNDER_PANEL)
  {
    focus.x = pri_monitor_width_ - 100;
    focus.y = panel_height_;
  }
  else if (!g_strcmp0(launcher_position_, "Bottom"))
  {
    focus.x = index * (icon_size_.CP(cv_) + SPACE_BETWEEN_ICONS.CP(cv_)) + SPACE_BETWEEN_ICONS.CP(cv_) + 0.5 * icon_size_.CP(cv_);
    focus.y = pri_monitor_height_ - launcher_size_;
  }
  else
  {
    focus.x = launcher_size_;
    focus.y = panel_height_ + index * (icon_size_.CP(cv_) + SPACE_BETWEEN_ICONS.CP(cv_)) + SPACE_BETWEEN_ICONS.CP(cv_) + 0.5 * icon_size_.CP(cv_);
  }

  return focus;
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
