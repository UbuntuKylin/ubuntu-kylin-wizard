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

#include "config.h"
#include "Page.h"

Page::Page(std::string title, std::string subtitle, std::string desc, GtkBuilder *builder, int width, int height, std::string ind_pic, std::string thumbnail_pic, int icon_order)
    : name_(subtitle)
    , title_(title)
    , description_(desc)
    , width_(width)
    , height_(height)
    , builder_(builder)
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
            trans_rec_.x = 0;
            trans_rec_.y = height_ - launcher_size_;
            trans_rec_.width = width_;
            trans_rec_.height = launcher_size_;

            focus_spot_.x = fixed_spot_.x;
            focus_spot_.y = trans_rec_.y;
        }
        else
        {
            trans_rec_.x = 0;
            trans_rec_.y = panel_height_;
            trans_rec_.width = launcher_size_;
            trans_rec_.height = height_;

            focus_spot_.x = trans_rec_.width;
            focus_spot_.y = fixed_spot_.y;
        }
    }
    else if (name_ == "Indicator")
    {
        trans_rec_.x = 0;
        trans_rec_.y = 0;
        trans_rec_.width = width_;
        trans_rec_.height = panel_height_;

        focus_spot_.x = trans_rec_.width - 100;
        focus_spot_.y = trans_rec_.height;
    }
    else if (launcher_position_ == LauncherPosition::BOTTOM)
    {
        trans_rec_.x = icon_order_ * (icon_size_ + space_between_icons_) + space_between_icons_;
        trans_rec_.y = height_ - launcher_size_ + space_between_icons_;
        trans_rec_.width = trans_rec_.height = icon_size_;

        focus_spot_.x = trans_rec_.x + 0.5 * trans_rec_.width;
        focus_spot_.y = trans_rec_.y;
    }
    else
    {
        trans_rec_.x = space_between_icons_;
        trans_rec_.y = panel_height_ + icon_order_ * (icon_size_ + space_between_icons_) + space_between_icons_;
        trans_rec_.width = trans_rec_.height = icon_size_;

        focus_spot_.x = launcher_size_;
        focus_spot_.y = trans_rec_.y + 0.5 * trans_rec_.height;
    }
    SetPolylinePath();
}

std::string Page::GetTitle()
{
    return title_;
}

std::string Page::GetName()
{
    return name_;
}

std::string Page::GetDescription()
{
    return description_;
}

std::string Page::GetThumbnail()
{
    return thumbnail_pic_;
}

std::string Page::GetPageInd()
{
    return page_ind_pic_;
}

GdkRectangle* Page::GetTransparentRect()
{
    return &trans_rec_;
}

std::vector<GdkPoint>& Page::GetPolylinePath()
{
    return polyline_path_;
}

void Page::SetPolylinePath()
{
    polyline_path_.clear();
    polyline_path_.push_back(fixed_spot_);

    if (name_ == "Launcher")
    {
        polyline_path_.push_back(focus_spot_);
    }
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
            inflexion_1.x = fixed_spot_.x;
            inflexion_1.y = fixed_spot_.y + (height_ - launcher_size_ - fixed_spot_.y) * 0.95;

            inflexion_2.x = focus_spot_.x;
            inflexion_2.y = inflexion_1.y;
        }
        else
        {
            inflexion_1.x = (fixed_spot_.x - launcher_size_) * 0.15;
            inflexion_1.y = fixed_spot_.y;

            inflexion_2.x = inflexion_1.x;
            inflexion_2.y = focus_spot_.y;
        }
        polyline_path_.push_back(inflexion_1);
        polyline_path_.push_back(inflexion_2);
        polyline_path_.push_back(focus_spot_);
    }
}
