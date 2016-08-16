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

#include <pango/pangocairo.h>
#include <math.h>
#include <glib/gi18n.h>

#include "config.h"
#include "Page.h"

static bool draw_desc(GtkWidget *widget, cairo_t *cr, Page* p)
{
    if (p->GetActive())
        p->DrawDesc(cr);
    return FALSE;
}

Page::Page(std::string title, std::string subtitle, std::string desc, GtkBuilder *builder, int width, int height, std::string ind_pic, std::string thumbnail_pic, int icon_order)
    : name_(subtitle)
    , title_(title)
    , description_(desc)
    , width_(width)
    , height_(height)
    , builder_(builder)
    , active_(false)
    , page_ind_pic_(ind_pic)
    , thumbnail_pic_(thumbnail_pic)
    , icon_order_(icon_order)
    , launcher_position_(Settings::Instance().GetLauncherPosition())
    , launcher_size_(Settings::Instance().LauncherSize())
    , panel_height_(Settings::Instance().PanelHeight())
    , icon_size_(Settings::Instance().IconSize())
    , space_between_icons_(Settings::Instance().SpaceBetweenIcons())
{
    if (launcher_position_ == LauncherPosition::BOTTOM)
    {
        fixed_spot_.x = 0.5 * width_;
        fixed_spot_.y = 0.2 * height_;
    }
    else
    {
        fixed_spot_.x = 0.5 * (width_ + launcher_size_);
        fixed_spot_.y = 0.2 * height_;
    }

    if (name_ == "Launcher")
    {
        if (launcher_position_ == LauncherPosition::BOTTOM)
        {
            rec_x_ = 0;
            rec_y_ = height_ - launcher_size_;
            rec_width_ = width_;
            rec_height_ = launcher_size_;

            focus_spot_.x = fixed_spot_.x;
            focus_spot_.y = rec_y_;
        }
        else
        {
            rec_x_ = 0;
            rec_y_ = panel_height_;
            rec_width_ = launcher_size_;
            rec_height_ = height_;

            focus_spot_.x = rec_width_;
            focus_spot_.y = fixed_spot_.y;
        }
    }
    else if (name_ == "Indicator")
    {
        rec_x_ = 0;
        rec_y_ = 0;
        rec_width_ = width_;
        rec_height_ = panel_height_;

        focus_spot_.x = rec_width_ - 100;
        focus_spot_.y = rec_height_;
    }
    else if (launcher_position_ == LauncherPosition::BOTTOM)
    {
        rec_x_ = icon_order_ * (icon_size_ + space_between_icons_) + space_between_icons_;
        rec_y_ = height_ - launcher_size_ + space_between_icons_;
        rec_width_ = rec_height_ = icon_size_;

        focus_spot_.x = rec_x_ + 0.5 * rec_width_;
        focus_spot_.y = rec_y_;
    }
    else
    {
        rec_x_ = space_between_icons_;
        rec_y_ = panel_height_ + icon_order_ * (icon_size_ + space_between_icons_) + space_between_icons_;
        rec_width_ = rec_height_ = icon_size_;

        focus_spot_.x = launcher_size_;
        focus_spot_.y = rec_y_ + 0.5 * rec_height_;
    }

    page_ind_ = WID(builder_, WIDGET, "page_ind");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_1.png");

    thumbnail_ = WID(builder_, WIDGET, "thumbnail");

    desc_area_ = WID(builder_, WIDGET, "desc_area");

    g_signal_connect(G_OBJECT(desc_area_), "draw",
        G_CALLBACK(draw_desc), this);
}

bool Page::GetActive()
{
    return active_;
}

void Page::SetActive(bool active)
{
    active_ = active;
}

void Page::DrawDesc(cairo_t *cr)
{
    int area_width = gtk_widget_get_allocated_width(desc_area_);
    int area_height = gtk_widget_get_allocated_height(desc_area_);

    PangoLayout *layout;
    PangoFontDescription *desc;
    layout = pango_cairo_create_layout(cr);
    desc = pango_font_description_from_string("Ubuntu 30");
    pango_layout_set_font_description(layout, desc);

    pango_layout_set_width(layout, 0.7 * area_width * PANGO_SCALE);
    pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_move_to(cr, area_width * 0.25, area_height * 0.2);
    pango_layout_set_text (layout, _(title_.c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    desc = pango_font_description_from_string("Ubuntu 24");
    pango_layout_set_font_description(layout, desc);
    cairo_move_to(cr, area_width * 0.25, area_height * 0.3);
    pango_layout_set_text(layout, _(name_.c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    desc = pango_font_description_from_string("Ubuntu 16");
    pango_layout_set_font_description(layout, desc);
    cairo_move_to(cr, area_width * 0.25, area_height * 0.4);
    pango_layout_set_text(layout, _(description_.c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    pango_font_description_free(desc);
    g_object_unref(layout);
}

void Page::Draw(cairo_t *win_cr)
{
    ClipRec(win_cr);
    DrawPolyline(win_cr);
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), page_ind_pic_.c_str());
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), thumbnail_pic_.c_str());
}

void Page::ClipRec(cairo_t *win_cr)
{
    cairo_save(win_cr);
    cairo_rectangle(win_cr, rec_x_, rec_y_, rec_width_, rec_height_);
    cairo_clip(win_cr);
    cairo_new_path(win_cr);
    cairo_set_source_rgba(win_cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator(win_cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(win_cr);
    cairo_restore(win_cr);
}

void Page::DrawPolyline(cairo_t *win_cr)
{
    cairo_set_source_rgba(win_cr, 1.0, 1.0, 1.0, 0.9);
    cairo_set_line_width(win_cr, 2);

    cairo_move_to(win_cr, fixed_spot_.x, fixed_spot_.y);

    if (name_ == "Launcher")
        cairo_line_to(win_cr, focus_spot_.x, focus_spot_.y);
    else
    {
        GdkPoint inflexion_1, inflexion_2;

        if (name_ == "Indicator")
        {
           inflexion_1.x = fixed_spot_.x;
           inflexion_1.y = (panel_height_ + fixed_spot_.y) * 0.5;

           inflexion_2.x = focus_spot_.x;
           inflexion_2.y = inflexion_1.y;
        }
        else if (launcher_position_ == LauncherPosition::BOTTOM)
        {
            GtkWidget *close_button = WID(builder_, WIDGET, "close_button");
            GtkAllocation button_rect;
            gtk_widget_get_allocation(close_button, &button_rect);

            inflexion_1.x = fixed_spot_.x;
            inflexion_1.y = (height_ - launcher_size_ + button_rect.y + button_rect.height) * 0.5;

            inflexion_2.x = focus_spot_.x;
            inflexion_2.y = inflexion_1.y;
        }
        else
        {
            GtkWidget *left_box = WID(builder_, WIDGET, "left_box");
            GtkAllocation left_box_rect;
            gtk_widget_get_allocation(left_box, &left_box_rect);

            inflexion_1.x = left_box_rect.x + 2 * left_box_rect.width;
            inflexion_1.y = fixed_spot_.y;

            inflexion_2.x = inflexion_1.x;
            inflexion_2.y = focus_spot_.y;
        }

        cairo_line_to(win_cr, inflexion_1.x, inflexion_1.y);
        cairo_line_to(win_cr, inflexion_2.x, inflexion_2.y);
        cairo_line_to(win_cr, focus_spot_.x, focus_spot_.y);
    }
    cairo_stroke(win_cr);

    DrawRing(win_cr, fixed_spot_.x, fixed_spot_.y);
    DrawRing(win_cr, focus_spot_.x, focus_spot_.y);
}

void Page::DrawRing(cairo_t *win_cr, int x, int y)
{
  cairo_set_source_rgba(win_cr, 1., 1., 1., 0.1);
  cairo_arc(win_cr, x, y, 10, 0., 2 * M_PI);
  cairo_fill(win_cr);

  cairo_set_source_rgba(win_cr, 1., 1., 1., 0.4);
  cairo_arc(win_cr, x, y, 7, 0., 2 * M_PI);
  cairo_fill(win_cr);

  cairo_set_source_rgba(win_cr, 1., 1., 1., 0.9);
  cairo_arc(win_cr, x, y, 4, 0., 2 * M_PI);
  cairo_fill(win_cr);
}
