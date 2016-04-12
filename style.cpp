#include "style.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string>
#include <memory>
#include <iostream>

const std::string LAUNCHER_SETTINGS = "com.canonical.Unity.Launcher";
const std::string FAVORITES = "favorites";

const std::string UNITY_GSETTINGS_SCHEMA = "org.compiz.unityshell";
const std::string UNITY_GSETTINGS_PATH = "/org/compiz/profiles/unity/plugins/unityshell/";
const std::string ICON_SIZE = "icon-size";

const gint MID_LAUNCHER = -2;
const gint UNDER_PANEL = -3;

const RawPixel DEFAULT_ICON_SIZE  = 48_em;
const RawPixel DEFAULT_ICON_SIZE_DELTA =  6_em;

const gint PANEL_HEIGHT = 24;
const gint LAUNCHER_SIZE = 64;
const gint ARROW_HEIGHT = 80;
const gint ARROW_WIDTH  = 33;

const RawPixel SIDE_LINE_WIDTH = 1_em;

Style::Style()
  : unity_settings_(g_settings_new_with_path(UNITY_GSETTINGS_SCHEMA.c_str(), UNITY_GSETTINGS_PATH.c_str()))
  , launcher_settings_(g_settings_new(LAUNCHER_SETTINGS.c_str()))
{
  GdkWindow *root_win = gdk_get_default_root_window();
  root_width_ = gdk_window_get_width(root_win);
  root_height_ = gdk_window_get_height(root_win);

  icon_size_ = g_settings_get_int(unity_settings_, ICON_SIZE.c_str()) + DEFAULT_ICON_SIZE_DELTA;
  launcher_size_ = icon_size_ + 2 * ICON_PADDING + SIDE_LINE_WIDTH.CP(1) - 2;
  panel_height_= PANEL_HEIGHT;

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

  left_arrow_pos_.x = 0.05 * (root_width_ - ARROW_WIDTH);
  left_arrow_pos_.y = 0.5 * (root_height_ - ARROW_HEIGHT);

  right_arrow_pos_.x = 0.95 * (root_width_ - ARROW_WIDTH);
  right_arrow_pos_.y = 0.5 * (root_height_ - ARROW_HEIGHT);

  base_pos_.x = 0.125 * root_width_;
  base_pos_.y = 0.25 * root_height_;

  title1_pos_.x = 0.5 * root_width_;
  title1_pos_.y = 0.25 * root_height_;

  title2_pos_.x = title1_pos_.x;
  title2_pos_.y = 0.35 * root_height_;

  details_pos_.x = title1_pos_.x;
  details_pos_.y = 0.4 * root_height_;

  close_pos_.x = 0.5 * root_width_;
  close_pos_.y = 0.9 * (root_height_ - launcher_size_);

  page_ind_pos_.x = 0.75 * root_width_;
  page_ind_pos_.y = 0.9 * (root_height_ - launcher_size_);

  spot_pos_.x = 0.45 * root_width_;
  spot_pos_.y = 0.25 * root_height_;

  inflexion_pos_.x = spot_pos_.x;
  inflexion_pos_.y = close_pos_.y + 30;
}

gint Style::get_root_width()
{
  return root_width_;
}

gint Style::get_root_height()
{
  return root_height_;
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
  return icon_size_;
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

    if (value.find("devices") != std::string::npos || value.find("running") != std::string::npos || value.find("expo-icon") != std::string::npos)
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
    icon_pos.x = 0.45 * root_width_;
    icon_pos.y = root_height_ - launcher_size_;
  }
  else if (index == UNDER_PANEL)
  {
    icon_pos.x = root_width_ - 15;
    icon_pos.y = panel_height_;
  }
  else
  {
    icon_pos.x = (index - 1) * (icon_size_ + SPACE_BETWEEN_ICONS) + SPACE_BETWEEN_ICONS + icon_size_ / 2;
    icon_pos.y = root_height_ - launcher_size_;
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

Point Style::get_title1_pos()
{
  return title1_pos_;
}

Point Style::get_title2_pos()
{
  return title2_pos_;
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
