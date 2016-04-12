#ifndef DRAW_H
#define DRAW_H

#include "config.h"

#include <cairo.h>
#include <gtk/gtk.h>

#include "style.h"

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

class Draw
{
public:
  Draw();
  ~Draw();

  GtkBuilder* Builer();
  void Run();
  void do_drawing(cairo_t *cr);
  void draw_background(cairo_t *cr);
  void draw_page(cairo_t *cr);
  void draw_polyline(cairo_t *cr);
  gint page();
  void next_page();
  void pre_page();

private:
  Style *style_;
  GtkBuilder *builder_;
  GdkPixbuf *root_pixbuf_;
  int page_num_;

  GtkWidget *window_;
  GtkWidget *fixed_;
  GtkWidget *left_box_;
  GtkWidget *right_box_;
  GtkWidget *close_box_;
  GtkWidget *arrow_left_img_;
  GtkWidget *arrow_right_img_;
  GtkWidget *base_img_;
  GtkWidget *thumbnail_;
  GtkWidget *close_img_;
  GtkWidget *page_ind_;
  GtkWidget *grid_;
  GtkWidget *title1_;
  GtkWidget *title2_;
  GtkWidget *details_;
  GtkWidget *lightspot1_;
  GtkWidget *lightspot2_;
};

#endif
