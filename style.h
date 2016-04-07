#ifndef STYLE_H
#define STYLE_H

#include <glib.h>
#include <string>

typedef struct
{
  gint x;
  gint y;
} Point;

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

#endif
