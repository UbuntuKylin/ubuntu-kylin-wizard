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
#include <vector>

#include "WizardSettings.h"

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

class Page
{
public:
    Page(std::string title, std::string subtitle, std::string desc, GtkBuilder *builder, int width, int height, std::string ind_pic, std::string thumbnail_pic, int icon_order);

    std::string GetTitle();
    std::string GetName();
    std::string GetDescription();
    std::string GetThumbnail();
    std::string GetPageInd();
    GdkRectangle *GetTransparentRect();
    std::vector<GdkPoint> &GetPolylinePath();

private:
    void SetPolylinePath();

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

    int width_;
    int height_;

    GdkRectangle trans_rec_;
    std::vector<GdkPoint> polyline_path_;

    GdkPoint fixed_spot_;
    GdkPoint focus_spot_;
};

#endif
