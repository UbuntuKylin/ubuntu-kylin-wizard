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

#ifndef BASIC_WINDOW_H
#define BASIC_WINDOW_H

#include <gtk/gtk.h>

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

class BaseWindow
{
public:
    BaseWindow(GtkBuilder* builder, int width, int height);
    void Show(cairo_t *cr);
    void DrawBackground(cairo_t *cr);

private:
    GtkBuilder *builder_;

    GtkWidget *base_img_;
    GtkWidget *thumbnail_;
    GtkWidget *left_box_;
    GtkWidget *right_box_;
    GtkWidget *arrow_left_img_;
    GtkWidget *arrow_right_img_;
    GtkWidget *close_button_;
    GtkWidget *page_ind_;

    GdkPixbuf *root_pixbuf_;

    int width_;
    int height_;
};

#endif
