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
