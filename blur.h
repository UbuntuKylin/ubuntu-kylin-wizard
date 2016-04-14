// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2015 Canonical Ltd
 *               2015, National University of Defense Technology(NUDT) & Kylin Ltd
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
 * Authored by: Mirco Müller <mirco.mueller@canonical.com
 *              Neil Jagdish Patel <neil.patel@canonical.com>
 */

#ifndef BLUR_H
#define BLUR_H

#include <cairo.h>
#include <gtk/gtk.h>

inline double align(double val, bool odd=true);
void RoundedRect(cairo_t* cr,
                 double   aspect,
                 double   x,
                 double   y,
                 double   cornerRadius,
                 double   width,
                 double   height);

void blurinner(guchar* pixel,
               gint*   zR,
               gint*   zG,
               gint*   zB,
               gint*   zA,
               gint    alpha,
               gint    aprec,
               gint    zprec);

void blurrow(guchar* pixels,
             gint    width,
             gint    height,
             gint    channels,
             gint    line,
             gint    alpha,
             gint    aprec,
             gint    zprec);

void blurcol(guchar* pixels,
             gint    width,
             gint    height,
             gint    channels,
             gint    x,
             gint    alpha,
             gint    aprec,
             gint    zprec);

void expblur(guchar* pixels,
             gint     width,
             gint     height,
             gint     channels,
             gint     radius,
             gint     aprec,
             gint     zprec);

void blur(cairo_surface_t* surface, int size);

#endif
