#include "draw.h"

#include "style.h"
#include "blur.h"

#include <cairo.h>
#include <iostream>

void clip_rec(cairo_t *cr, int x, int y, int width, int height)
{
  cairo_save(cr);
  cairo_rectangle(cr, x, y, width, height);
  cairo_clip(cr);
  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
  cairo_restore(cr);
}

void do_drawing(cairo_t *cr, Info *info)
{
  draw_background(cr, info);
  draw_page(cr, info);
}

void draw_background(cairo_t *cr, Info *info)
{
  GdkWindow *root_win = gdk_get_default_root_window();
  gint width = gdk_window_get_width(root_win);
  gint height = gdk_window_get_height(root_win);
  if (!info->root_pixbuf)
  {
    info->root_pixbuf = gdk_pixbuf_get_from_window(root_win, 0, 0, width, height);
  }

  cairo_surface_t *bg_sur = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *bg_cr = cairo_create(bg_sur);
  gdk_cairo_set_source_pixbuf(bg_cr, info->root_pixbuf, 0, 0);
  cairo_paint(bg_cr);

  blur(bg_sur, 8);
  cairo_set_source_surface(cr, bg_sur, 0, 0);
  cairo_paint(cr);

  cairo_destroy(bg_cr);
  cairo_surface_destroy(bg_sur);
}

void draw_page(cairo_t *cr, Info *info)
{
  switch (info->index) {
  case 0:
    clip_rec(cr, 0, 768 - LAUNCHER_SIZE, 1366, LAUNCHER_SIZE);
    draw_polyline(cr, info, 0);
    gtk_image_set_from_file(WID(info->builder, IMAGE, "page_ind"), PKGDATADIR"/step_1.png");
    break;
 case 1:
    clip_rec(cr, SPACE_BETWEEN_ICONS, 768 - LAUNCHER_SIZE + ICON_PADDING, 48 + DEFAULT_ICON_SIZE_DELTA, 48 + DEFAULT_ICON_SIZE_DELTA);
    draw_polyline(cr, info, 1);
    gtk_image_set_from_file(WID(info->builder, IMAGE, "page_ind"), PKGDATADIR"/step_2.png");
    break;
  case 2:
    clip_rec(cr, 2 * SPACE_BETWEEN_ICONS + 48 + DEFAULT_ICON_SIZE_DELTA, 768 - LAUNCHER_SIZE + ICON_PADDING, 48 + DEFAULT_ICON_SIZE_DELTA, 48 + DEFAULT_ICON_SIZE_DELTA);
    draw_polyline(cr, info, 2);
    gtk_image_set_from_file(WID(info->builder, IMAGE, "page_ind"), PKGDATADIR"/step_3.png");
      break;
  case 3:
    break;
  case 4:
//    clip_rec(cr, );
    break;
  case 5:
    break;
  default:
    break;
  }
}

void draw_polyline(cairo_t *cr, Info *info, int index)
{
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 8.0);
  cairo_set_line_width(cr, 2);

  Point p;
  switch (index) {
  case 0:
    p.x = get_spot2_pos().x;
    p.y = 768 - 56;
    break;
  case 1:
    p.x = SPACE_BETWEEN_ICONS + 0.5 * (48 + DEFAULT_ICON_SIZE_DELTA);
    p.y = 768 - 56;
    break;
  case 2:
    p.x = get_spot1_pos("Nautilus").x;
    p.y = get_spot1_pos("Nautilus").y;
    break;
  case 3:
    break;
  case 4:
    p.x = get_spot1_pos("unity-control-center").x;
    p.y = get_spot1_pos("unity-control-center").y;
    break;
  default:
    break;
  }
  gtk_fixed_move(WID(info->builder, FIXED, "fixed"), WID(info->builder, WIDGET, "lightspot1"),
                 p.x - 0.5 * LIGHTSPOT_SIZE, p.y - 0.5 * LIGHTSPOT_SIZE);
  cairo_move_to(cr, p.x, p.y);
  cairo_line_to(cr, p.x, get_inflexion_pos().y);
  cairo_line_to(cr, get_inflexion_pos().x, get_inflexion_pos().y);
  cairo_line_to(cr, get_spot2_pos().x, get_spot2_pos().y);
  cairo_stroke(cr);
}
