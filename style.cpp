#include "style.h"

#include <gio/gio.h>
#include <string>
#include <memory>
#include <iostream>

const std::string LAUNCHER_SETTINGS = "com.canonical.Unity.Launcher";
const std::string FAVORITES = "favorites";

const std::string UNITY_GSETTINGS_SCHEMA = "org.compiz.unityshell";
const std::string UNITY_GSETTINGS_PATH = "/org/compiz/profiles/unity/plugins/unityshell/";
const std::string ICON_SIZE = "icon-size";

const RawPixel DEFAULT_ICON_SIZE  = 48_em;

const RawPixel SIDE_LINE_WIDTH = 1_em;

const gint root_width = 1366;
const gint root_height = 768;

gint get_icon_size()
{
  GSettings *unity_settings = g_settings_new_with_path(UNITY_GSETTINGS_SCHEMA.c_str(), UNITY_GSETTINGS_PATH.c_str());
  gint icon_size = g_settings_get_int(unity_settings, ICON_SIZE.c_str());
  icon_size += DEFAULT_ICON_SIZE_DELTA;
  return icon_size;
}

Point get_icon_position(std::string icon_name)
{
  Point icon_pos;
  icon_pos.x = 0;
  icon_pos.y = 0;
  gint n = 0;
  gboolean found = false;
  GSettings *launcher_settings = g_settings_new(LAUNCHER_SETTINGS.c_str());

  std::unique_ptr<gchar*[], void(*)(gchar**)> strings(g_settings_get_strv(launcher_settings, FAVORITES.c_str()), g_strfreev);

  for (int i = 0; strings[i]; ++i)
  {
    std::string value = strings[i];

    if (!value.empty() && value.find(icon_name) != std::string::npos)
    {
      found = true;
      n = i + 1;
      break;
    }
  }

  if (!found)
    return icon_pos;

  gint icon_size = get_icon_size();
  n++;
  icon_pos.x = (n - 1) * (icon_size + SPACE_BETWEEN_ICONS) + SPACE_BETWEEN_ICONS + icon_size / 2;
  icon_pos.y = 768 - 56;

  return icon_pos;
}

Point get_left_arrow_pos()
{
  Point left_pos;
  left_pos.x = 0.05 * root_width;
  left_pos.y = 0.5 * root_height;

  return left_pos;
}

Point get_right_arrow_pos()
{
  Point right_pos;
  right_pos.x = 0.95 * root_width;
  right_pos.y = 0.5 * root_height;

  return right_pos;
}

Point get_base_pos()
{
  Point base_pos;
  base_pos.x = 0.125 * root_width;
  base_pos.y = 0.25 * root_height;

  return base_pos;
}

Point get_title1_pos()
{
  Point title1_pos;
  title1_pos.x = 0.5 * root_width;
  title1_pos.y = 0.25 * root_height;

  return title1_pos;
}

Point get_title2_pos()
{
  Point title2_pos;
  title2_pos.x = 0.5 * root_width;
  title2_pos.y = 0.35 * root_height;

  return title2_pos;
}

Point get_details_pos()
{
  Point details_pos;
  details_pos.x = 0.5 * root_width;
  details_pos.y = 0.4 * root_height;

  return details_pos;
}

Point get_close_pos()
{
  Point close_pos;
  close_pos.x = 0.5 * root_width;
  close_pos.y = 0.75 * root_height;

  return close_pos;
}

Point get_page_ind_pos()
{
  Point page_ind_pos;
  page_ind_pos.x = 0.75 * root_width;
  page_ind_pos.y = 0.75 * root_height;

  return page_ind_pos;
}

Point get_spot1_pos(std::string icon_name)
{
  Point spot1_pos;
  spot1_pos = get_icon_position(icon_name);

  return spot1_pos;
}

Point get_spot2_pos()
{
  Point spot2_pos;
  spot2_pos.x = 0.45 * root_width;
  spot2_pos.y = 0.25 * root_height;

  return spot2_pos;
}

Point get_inflexion_pos()
{
  Point inflexion_pos;
  inflexion_pos.x = get_spot2_pos().x;
  inflexion_pos.y = get_close_pos().y + 50;

  return inflexion_pos;
}
