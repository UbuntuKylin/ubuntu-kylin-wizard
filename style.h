#ifndef STYLE_H
#define STYLE_H

#include <glib.h>
#include <string>

#include "rawpixel.h"

typedef struct
{
  gint x;
  gint y;
} Point;

const RawPixel DEFAULT_ICON_SIZE_DELTA =  6_em;
const RawPixel SPACE_BETWEEN_ICONS     =  5_em;
const RawPixel ICON_PADDING            =  6_em;

const gint PANEL_HEIGHT = 24;
const gint LAUNCHER_SIZE = 64;
const gint LIGHTSPOT_SIZE = 21;

//class Style
//{
//public:
//  Style();
//  ~Style();

//private:
//  gint icon_size_;
//  gint
//};

gint  get_icon_size();
Point get_icon_position(std::string icon_name);
Point get_left_arrow_pos();
Point get_right_arrow_pos();
Point get_base_pos();
Point get_title1_pos();
Point get_title2_pos();
Point get_details_pos();
Point get_close_pos();
Point get_page_ind_pos();
Point get_spot1_pos(std::string icon_name);
Point get_spot2_pos();
Point get_inflexion_pos();

#endif
