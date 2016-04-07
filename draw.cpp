#include "draw.h"

#include "style.h"
#include "blur.h"

#include <cairo.h>
#include <iostream>

void do_drawing(cairo_t *cr, Info *info)
{
  Point icon_pos = get_icon_position("Nautilus");
  if (icon_pos.x == 0)
    std::cout << "Not found" << std::endl;
  draw_background(cr, info);
  draw_page(cr, info);
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
  cairo_set_line_width(cr, 4);
  cairo_move_to(cr, icon_pos.x, icon_pos.y);
  cairo_line_to(cr, icon_pos.x, icon_pos.y - 600);
  cairo_stroke(cr);

//   // blur and blend overlay onto initial image-surface
//   cairo_surface_set_device_scale(surface, w_scale, h_scale);
//   blur(blurred_cr, blurSize);
//   cairo_set_source_surface(cr, surface, 0.0, 0.0);
//   cairo_paint_with_alpha(cr, opacity);

   // clean up
//   cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
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
    cairo_save(cr);
    cairo_rectangle(cr, 0, 768 - 56, 1366, 56);
    cairo_clip(cr);
    cairo_new_path(cr);
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_restore(cr);

    gtk_image_set_from_file(WID(info->builder, IMAGE, "page_ind"), PKGDATADIR"/step_1.png");
    break;
 case 1:
    cairo_save(cr);
    cairo_rectangle(cr, 5, 768 - 56, 48 + 6, 48 + 6);
    cairo_clip(cr);
    cairo_new_path(cr);
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_restore(cr);

    gtk_image_set_from_file(WID(info->builder, IMAGE, "page_ind"), PKGDATADIR"/step_2.png");
    break;
  default:
    break;
  }
}
