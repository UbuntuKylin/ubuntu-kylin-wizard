#ifndef DRAW_H
#define DRAW_H

#include "config.h"

#include <cairo.h>
#include <gtk/gtk.h>

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

typedef struct{
  cairo_t *cr;
  GtkBuilder *builder = NULL;
  GdkPixbuf *root_pixbuf = NULL;
  int index = 0;
} Info;

void do_drawing(cairo_t *, Info *info);
void draw_background(cairo_t *, Info *info);
void draw_page(cairo_t *cr, Info *info);
void draw_polyline(cairo_t *cr, Info *info, int index);

#endif
