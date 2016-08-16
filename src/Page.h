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

#ifndef PAGE_H
#define PAGE_H

#include <cairo.h>
#include <gtk/gtk.h>
#include <string>

#include "WizardSettings.h"

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

class Page
{
public:
    Page(std::string title, std::string subtitle, std::string desc, GtkBuilder *builder, int width, int height, std::string ind_pic, std::string thumbnail_pic, int icon_order);

    void DrawDesc(cairo_t *desc_cr);
    void Draw(cairo_t *win_cr);

    bool GetActive();
    void SetActive(bool active);

private:
    void ClipRec(cairo_t *win_cr);
    void DrawPolyline(cairo_t *win_cr);
    void DrawRing(cairo_t *win_cr, int x, int y);

    std::string title_;
    std::string name_;
    std::string description_;
    std::string thumbnail_pic_;
    std::string page_ind_pic_;
    int icon_order_;

    LauncherPosition launcher_position_;
    int launcher_size_;
    int panel_height_;
    int icon_size_;
    int space_between_icons_;

    GtkBuilder *builder_;
    GtkWidget *desc_area_;
    GtkWidget *page_ind_;
    GtkWidget *thumbnail_;

    int width_;
    int height_;

    int rec_x_;
    int rec_y_;
    int rec_width_;
    int rec_height_;

    GdkPoint fixed_spot_;
    GdkPoint focus_spot_;

    bool active_;
};

#endif
