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

#ifndef WIZARD_H
#define WIZARD_H

#include <vector>
#include <glib/gi18n.h>

#include "config.h"
#include "Page.h"

const int PAGE_NUM = 6;

const std::string page_ind_name[PAGE_NUM] = {PKGDATADIR"/step_1.png", PKGDATADIR"/step_2.png", PKGDATADIR"/step_3.png", PKGDATADIR"/step_4.png", PKGDATADIR"/step_5.png", PKGDATADIR"/step_6.png"};
const std::string left_thumbnail_name[PAGE_NUM] = {PKGDATADIR"/thumbnail_left_1.png", PKGDATADIR"/thumbnail_left_2.png", PKGDATADIR"/thumbnail_left_3.png", PKGDATADIR"/thumbnail_left_4.png", PKGDATADIR"/thumbnail_left_5.png", PKGDATADIR"/thumbnail_left_6.png"};
const std::string bottom_thumbnail_name[PAGE_NUM] = {PKGDATADIR"/thumbnail_bottom_1.png", PKGDATADIR"/thumbnail_bottom_2.png", PKGDATADIR"/thumbnail_bottom_3.png", PKGDATADIR"/thumbnail_bottom_4.png", PKGDATADIR"/thumbnail_bottom_5.png", PKGDATADIR"/thumbnail_bottom_6.png"};

class Wizard
{
public:
    Wizard();
    ~Wizard();
    void Show();
    void DoDraw(cairo_t *cr);
    void PrePage();
    void NextPage();
    void Quit();
    void DrawOtherMonitor(int screen_num);
    void DrawDesc(cairo_t *cr);

private:
    int IsValid(std::string name);
    void InitWidgets();
    void DrawBackground(cairo_t *cr);
    void DrawPolyline(cairo_t *cr);
    void DrawRing(cairo_t *cr, int x, int y);
    void ClipRec(cairo_t *cr);

    GtkBuilder *builder_;
    std::vector<Page*> pages_;
    GdkPixbuf *root_pixbuf_;

    GtkWidget *grid_;
    GtkWidget *win_;
    GtkWidget *fixed_;
    GtkWidget *desc_area_;
    GtkWidget *right_box_;
    GtkWidget *left_box_;
    GtkWidget *base_img_;
    GtkWidget *thumbnail_;
    GtkWidget *arrow_left_img_;
    GtkWidget *arrow_right_img_;
    GtkWidget *close_button_;
    GtkWidget *page_ind_;

    int page_index_;

    int screen_width_;
    int screen_height_;
    int primary_monitor_width_;
    int primary_monitor_height_;
};

#endif
