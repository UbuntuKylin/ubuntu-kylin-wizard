#ifndef STYLE_H
#define STYLE_H

#include <glib.h>
#include <gio/gio.h>
#include <string>

#include "rawpixel.h"

const gint PAGES_NUM = 6;
const gint LIGHTSPOT_SIZE = 21;
const RawPixel SPACE_BETWEEN_ICONS     =  5_em;
const RawPixel ICON_PADDING            =  6_em;

typedef struct
{
  gint x;
  gint y;
} Point;

class Style
{
public:
  Style();
  ~Style();

  gint get_root_width();
  gint get_root_height();
  gint get_launcher_size();
  gint get_panel_height();
  gint get_icon_size();
  Point get_icon_position(int index);
  Point get_left_arrow_pos();
  Point get_right_arrow_pos();
  Point get_base_pos();
  Point get_title1_pos();
  Point get_title2_pos();
  Point get_details_pos();
  Point get_close_pos();
  Point get_page_ind_pos();
  Point get_spot_pos();
  Point get_inflexion_pos();

  Point icon_pos_[PAGES_NUM];

private:
  gint get_icon_order(std::string icon_name);
  Point cal_icon_position(int index);
  gint root_width_;
  gint root_height_;
  gint launcher_size_;
  gint panel_height_;
  gint icon_size_;
  gint icon_order_[PAGES_NUM];

  Point left_arrow_pos_;
  Point right_arrow_pos_;
  Point base_pos_;
  Point title1_pos_;
  Point title2_pos_;
  Point details_pos_;
  Point close_pos_;
  Point page_ind_pos_;
  Point spot_pos_;
  Point inflexion_pos_;

  GSettings *unity_settings_;
  GSettings *launcher_settings_;

};

#endif
