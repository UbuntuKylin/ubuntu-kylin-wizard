// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
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

#include <iostream>
#include <glib/gi18n.h>

#include "blur.h"
#include "BaseWindow.h"
#include "config.h"

static bool enter_left_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_left_hover.png");
  return FALSE;
}

static bool leave_left_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_left.png");
  return FALSE;
}

static bool enter_right_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_right_hover.png");
  return FALSE;
}

static bool leave_right_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_right.png");
  return FALSE;
}

BaseWindow::BaseWindow(GtkBuilder *builder, int width, int height)
    : builder_(builder)
    , width_(width)
    , height_(height)
{
    GError *error = NULL;
    base_img_ = WID(builder_, WIDGET, "base_img");
    thumbnail_ = WID(builder_, WIDGET, "thumbnail");

    left_box_ = WID(builder_, WIDGET, "left_box");
    arrow_left_img_ = WID(builder_, WIDGET, "arrow_left_img");

    right_box_ = WID(builder, WIDGET, "right_box");
    arrow_right_img_ = WID(builder_, WIDGET, "arrow_right_img");

    close_button_ = WID(builder_, WIDGET, "close_button");
    page_ind_ = WID(builder_, WIDGET, "page_ind");

    GdkPixbuf *base_pixbuf = gdk_pixbuf_new_from_file(PKGDATADIR"/computer.png", &error);
    gtk_image_set_from_pixbuf(GTK_IMAGE(base_img_), base_pixbuf);
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_left_1.png");
    gtk_image_set_from_file(GTK_IMAGE(arrow_left_img_), PKGDATADIR"/arrow_left.png");
    gtk_image_set_from_file(GTK_IMAGE(arrow_right_img_), PKGDATADIR"/arrow_right.png");
    gtk_button_set_label(GTK_BUTTON(close_button_), _("Login"));

    root_pixbuf_ = gdk_pixbuf_get_from_window(gdk_get_default_root_window(), 0, 0, width_, height_);

    g_signal_connect(G_OBJECT(left_box_), "enter_notify_event",
        G_CALLBACK(enter_left_box), arrow_left_img_);
    g_signal_connect(G_OBJECT(left_box_), "leave_notify_event",
        G_CALLBACK(leave_left_box), arrow_left_img_);

    g_signal_connect(G_OBJECT(right_box_), "enter_notify_event",
        G_CALLBACK(enter_right_box), arrow_right_img_);
    g_signal_connect(G_OBJECT(right_box_), "leave_notify_event",
        G_CALLBACK(leave_right_box), arrow_right_img_);
}

void BaseWindow::Show(cairo_t *cr)
{
    DrawBackground(cr);
}

void BaseWindow::DrawBackground(cairo_t *cr)
{
    cairo_surface_t *bg_sur = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width_, height_);
    cairo_t *bg_cr = cairo_create(bg_sur);
    gdk_cairo_set_source_pixbuf(bg_cr, root_pixbuf_, 0, 0);
    cairo_paint(bg_cr);

    blur(bg_sur, 8);
    cairo_set_source_surface(cr, bg_sur, 0, 0);
    cairo_paint(cr);

    cairo_destroy(bg_cr);
    cairo_surface_destroy(bg_sur);
}
