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
  void Setup();
  void do_drawing(cairo_t *cr);
  void draw_other(gint num);
  void draw_background(cairo_t *cr);
  void clip_blur_region(cairo_t *cr);
  void draw_page(cairo_t *cr);
  void draw_polyline(cairo_t *cr);
  void draw_ring(cairo_t *cr, gint x, gint y);
  gint page();
  void next_page();
  void pre_page();

private:
  Style *style_;
  GtkBuilder *builder_;
  GdkPixbuf *root_pixbuf_;
  gint primary_screen_;
  gint page_num_;

  GtkWidget *window_;
  GtkWidget *fixed_;
  GtkWidget *background_;
  GtkWidget *left_box_;
  GtkWidget *right_box_;
  GtkWidget *arrow_left_img_;
  GtkWidget *arrow_right_img_;
  GtkWidget *base_img_;
  GtkWidget *thumbnail_;
  GtkWidget *close_button_;
  GtkWidget *page_ind_;
  GtkWidget *grid_;
  GtkWidget *title_;
  GtkWidget *subtitle_;
  GtkWidget *details_;
};

#endif
