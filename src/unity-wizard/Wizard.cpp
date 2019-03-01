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

#include <gtk/gtk.h>
#include <math.h>

#include "Wizard.h"
#include "blur.h"
#include "config.h"
#include "WizardSettings.h"

const std::string items[PAGE_NUM] = {"launcher", "dash", "Nautilus", "youker-assistant", "unity-control-center", "indicator"};

const std::string title[PAGE_NUM] = {_("Quick access area"),
                        _("Quick intelligent search"),
                        _("File manager"),
                        _("Desktop manager"),
                        _("User & system settings"),
                        _("Check app & system status")
                       };
const std::string subtitle[PAGE_NUM] = {_("Launcher"),
                           _("Dash"),
                           _("Nautilus"),
                           _("Youker assistant"),
                           _("Unity control center"),
                           _("Indicator")
                          };

const std::string details[PAGE_NUM] = {_("Provides you with quick access to applications, workspaces, removable devices and the Recycle Bin."),
                          _("Allows you to search for applications, files, music, and videos, and shows you items that you have used recently."),
                          _("Allows to browse directories, preview files and launch applications associated with them. "),
                          _("A system management and configuration tool we developed for Linux users, could show system information, cleanup system garbage and beautify system."),
                          _("An improved user interface for configuring the desktop and other aspects of the system."),
                          _("A comprehensive set of indicators provide convenient and powerful access to application features and system facilities such as power, sound, messaging, and the current session.")
                         };

static gboolean draw(GtkWidget *widget, cairo_t *cr, Wizard* w)
{
    w->DoDraw(cr);
    return FALSE;
}

static gboolean draw_desc(GtkWidget *widget, cairo_t *cr, Wizard* w)
{
    w->DrawDesc(cr);
    return FALSE;
}

static gboolean quit(GtkWidget *widget, GdkEventKey *event, Wizard *w)
{
    w->Quit();
    return FALSE;
}

static gboolean on_key_pressed(GtkWidget *widget, GdkEventKey *event, Wizard *w)
{
    switch(event->keyval)
    {
    case GDK_KEY_Escape:
        w->Quit();
        break;
    case GDK_KEY_Left:
        w->PrePage();
        break;
    case GDK_KEY_Right:
        w->NextPage();
        break;
    default:
        break;
    }
    return FALSE;
}

static gboolean left_arrow_pressed(GtkWidget *widget, GdkEventButton *event, Wizard *w)
{
  if (event->button == 1)
  {
    w->PrePage();
  }
  return FALSE;
}

static gboolean right_arrow_pressed(GtkWidget *widget, GdkEventButton *event, Wizard *w)
{
  if (event->button == 1)
  {
    w->NextPage();
  }
  return FALSE;
}

static gboolean enter_left_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_left_hover.png");
  return FALSE;
}

static gboolean leave_left_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_left.png");
  return FALSE;
}

static gboolean enter_right_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_right_hover.png");
  return FALSE;
}

static gboolean leave_right_box(GtkWidget *widget, GdkEventButton *event, GtkWidget *img)
{
  gtk_image_set_from_file(GTK_IMAGE(img), PKGDATADIR"/arrow_right.png");
  return FALSE;
}

static gboolean window_focus_changed(GtkWidget *widget, GParamSpec *psepc)
{
    // TODO: didn't work here...
    //gtk_window_present(GTK_WINDOW(widget));
    return FALSE;
}

Wizard::Wizard()
    : page_index_(0)
    , builder_(gtk_builder_new_from_file(PKGDATADIR"/wizard.ui"))
{
    Settings settings;
    InitWidgets();

    GdkScreen *screen = gdk_screen_get_default();
    gtk_widget_set_visual(win_, gdk_screen_get_rgba_visual(screen));
    gtk_widget_set_app_paintable(win_, TRUE);
    gtk_window_set_type_hint(GTK_WINDOW(win_), GDK_WINDOW_TYPE_HINT_DOCK);
    gtk_window_set_keep_above(GTK_WINDOW(win_), TRUE);

    screen_width_ = gdk_screen_get_width(screen);
    screen_height_ = gdk_screen_get_height(screen);

    gint primary_monitor = gdk_screen_get_primary_monitor(screen);
    GdkRectangle geo;
    gdk_screen_get_monitor_geometry(screen, primary_monitor, &geo);
    primary_monitor_width_ = geo.width;
    primary_monitor_height_ = geo.height;

    gtk_widget_set_size_request(GTK_WIDGET(win_), screen_width_, screen_height_);

    const std::string *thumbnail_name;
    int launcher_size = Settings::Instance().LauncherSize();
    if (Settings::Instance().GetLauncherPosition() == LauncherPosition::BOTTOM)
    {
        thumbnail_name = bottom_thumbnail_name;
        gtk_widget_set_size_request(GTK_WIDGET(grid_), primary_monitor_width_, primary_monitor_height_- launcher_size);
    }
    else
    {
        thumbnail_name = left_thumbnail_name;
        gtk_fixed_move(GTK_FIXED(fixed_), grid_, launcher_size, 0);
        gtk_widget_set_size_request(GTK_WIDGET(grid_), primary_monitor_width_ - launcher_size, primary_monitor_height_ );
    }

    int icon_order;
    for (int i=0; i < PAGE_NUM; i++)
    {
        icon_order = IsValid(items[i]);
        if (icon_order != -255)
        {
            Page *p = new Page(title[i], subtitle[i], details[i], builder_, primary_monitor_width_, primary_monitor_height_, page_ind_name[i], thumbnail_name[i], icon_order);
            pages_.push_back(p);
        }
    }

    root_pixbuf_ = gdk_pixbuf_get_from_window(gdk_get_default_root_window(), 0, 0, screen_width_, screen_height_);

    g_signal_connect(G_OBJECT(close_button_), "button_press_event",
        G_CALLBACK(quit), this);

    g_signal_connect(G_OBJECT(left_box_), "button_press_event",
        G_CALLBACK(left_arrow_pressed), this);
    g_signal_connect(G_OBJECT(left_box_), "enter_notify_event",
        G_CALLBACK(enter_left_box), arrow_left_img_);
    g_signal_connect(G_OBJECT(left_box_), "leave_notify_event",
        G_CALLBACK(leave_left_box), arrow_left_img_);

    g_signal_connect(G_OBJECT(right_box_), "enter_notify_event",
        G_CALLBACK(enter_right_box), arrow_right_img_);
    g_signal_connect(G_OBJECT(right_box_), "leave_notify_event",
        G_CALLBACK(leave_right_box), arrow_right_img_);
    g_signal_connect(G_OBJECT(right_box_), "button_press_event",
        G_CALLBACK(right_arrow_pressed), this);

    g_signal_connect(G_OBJECT(win_), "key-press-event",
        G_CALLBACK(on_key_pressed), this);
    g_signal_connect(G_OBJECT(win_), "notify::has-toplevel-focus",
        G_CALLBACK(window_focus_changed), NULL);
    g_signal_connect(G_OBJECT(win_), "draw",
        G_CALLBACK(draw), this);

    g_signal_connect(G_OBJECT(desc_area_), "draw",
        G_CALLBACK(draw_desc), this);

    // Deal with multiple-screen
    int screen_num = gdk_screen_get_n_monitors(screen);
    if (screen_num > 1)
        DrawOtherMonitor(screen_num);
}

Wizard::~Wizard()
{
    size_t len = pages_.size();
    for (size_t i = 0; i < len; i++)
    {
        delete pages_[i];
    }
}

void Wizard::InitWidgets()
{
    GError *error = NULL;
    win_ = WID(builder_, WIDGET, "window");
    grid_ = WID(builder_, WIDGET, "grid");
    fixed_ = WID(builder_, WIDGET, "fixed");
    desc_area_ = WID(builder_, WIDGET, "desc_area");
    right_box_ = WID(builder_, WIDGET, "right_box");
    left_box_ = WID(builder_, WIDGET, "left_box");
    thumbnail_ = WID(builder_, WIDGET, "thumbnail");
    base_img_ = WID(builder_, WIDGET, "base_img");
    arrow_left_img_ = WID(builder_, WIDGET, "arrow_left_img");
    arrow_right_img_ = WID(builder_, WIDGET, "arrow_right_img");
    close_button_ = WID(builder_, WIDGET, "close_button");
    page_ind_ = WID(builder_, WIDGET, "page_ind");

    GdkPixbuf *base_pixbuf = gdk_pixbuf_new_from_file(PKGDATADIR"/computer.png", &error);
    gtk_image_set_from_pixbuf(GTK_IMAGE(base_img_), base_pixbuf);

    if (Settings::Instance().GetLauncherPosition() == LauncherPosition::BOTTOM)
        gtk_image_set_from_file(GTK_IMAGE(thumbnail_), bottom_thumbnail_name[0].c_str());
    else
        gtk_image_set_from_file(GTK_IMAGE(thumbnail_), left_thumbnail_name[0].c_str());
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), page_ind_name[0].c_str());
    gtk_image_set_from_file(GTK_IMAGE(arrow_left_img_), PKGDATADIR"/arrow_left.png");
    gtk_image_set_from_file(GTK_IMAGE(arrow_right_img_), PKGDATADIR"/arrow_right.png");
    gtk_button_set_label(GTK_BUTTON(close_button_), _("Login"));

}

void Wizard::DoDraw(cairo_t *cr)
{
    DrawBackground(cr);
    ClipRec(cr);
    DrawPolyline(cr);
}

void Wizard::Show()
{
    gtk_widget_show_all(win_);
    gtk_window_present(GTK_WINDOW(win_));
}

void Wizard::Quit()
{
    gtk_widget_destroy(win_);
    gtk_main_quit();
}

void Wizard::PrePage()
{
    if (page_index_ > 0)
    {
        page_index_--;
        gtk_image_set_from_file(GTK_IMAGE(thumbnail_), pages_[page_index_]->GetThumbnail().c_str());
        gtk_image_set_from_file(GTK_IMAGE(page_ind_), pages_[page_index_]->GetPageInd().c_str());
        gtk_widget_queue_draw(win_);
    }
}

void Wizard::NextPage()
{
    if (page_index_ + 1 < pages_.size())
    {
        page_index_++;
        gtk_image_set_from_file(GTK_IMAGE(thumbnail_), pages_[page_index_]->GetThumbnail().c_str());
        gtk_image_set_from_file(GTK_IMAGE(page_ind_), pages_[page_index_]->GetPageInd().c_str());
        gtk_widget_queue_draw(win_);
    }
}

int Wizard::IsValid(std::string name)
{
    if (name == "launcher")
        return -1;
    else if (name == "dash")
        return 0;
    else if (name == "indicator")
        return -2;

    int n = 0, adjust = 0;
    bool found = false;

    GSettings *launcher_settings = g_settings_new("com.canonical.Unity.Launcher");
    std::unique_ptr<gchar*[], void(*)(gchar**)> strings(g_settings_get_strv(launcher_settings, "favorites"), g_strfreev);
    for (int i = 0; strings[i]; ++i)
    {
        std::string value = strings[i];

        if (value.find("devices") != std::string::npos || value.find("running") != std::string::npos ||
        value.find("ubiquity") != std::string::npos)
            adjust--;

        if (!value.empty() && value.find(name) != std::string::npos)
        {
            found = true;
            n = i + 1;
            break;
        }
    }

    if (!found || (n + adjust <= 0))
    {
        return -255;
    }

    return n + adjust;
}

void Wizard::DrawOtherMonitor(int screen_num)
{
    int current_screen = 0;
    for (int i = 0; i < screen_num; i++)
    {
        if (i == current_screen)
            continue;

        GdkRectangle geo;
        gdk_screen_get_monitor_geometry(gdk_screen_get_default(), i, &geo);

        GtkWidget *button = gtk_button_new_with_label(_("Login"));
        gtk_widget_set_size_request(button, 115, 35);
        g_signal_connect(G_OBJECT(button), "button_press_event", G_CALLBACK(quit), this);
        gtk_fixed_put(GTK_FIXED(fixed_), button, geo.x + (geo.width - 115)/2, geo.y + (geo.height - 35)/2);
    }
}

void Wizard::DrawBackground(cairo_t *cr)
{
    cairo_surface_t *bg_sur = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, screen_width_, screen_height_);
    cairo_t *bg_cr = cairo_create(bg_sur);
    gdk_cairo_set_source_pixbuf(bg_cr, root_pixbuf_, 0, 0);
    cairo_paint(bg_cr);

    blur(bg_sur, 8);
    cairo_set_source_surface(cr, bg_sur, 0, 0);
    cairo_paint(cr);

    cairo_destroy(bg_cr);
    cairo_surface_destroy(bg_sur);
}

void Wizard::DrawDesc(cairo_t *cr)
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
    cairo_move_to(cr, area_width * 0.25, area_height * 0.15);
    pango_layout_set_text (layout, _(pages_[page_index_]->GetTitle().c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    desc = pango_font_description_from_string("Ubuntu 24");
    pango_layout_set_font_description(layout, desc);
    cairo_move_to(cr, area_width * 0.25, area_height * 0.30);
    pango_layout_set_text(layout, _(pages_[page_index_]->GetName().c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    desc = pango_font_description_from_string("Ubuntu 16");
    pango_layout_set_font_description(layout, desc);
    cairo_move_to(cr, area_width * 0.25, area_height * 0.43);
    pango_layout_set_text(layout, _(pages_[page_index_]->GetDescription().c_str()), -1);
    pango_cairo_show_layout(cr, layout);

    pango_font_description_free(desc);
    g_object_unref(layout);
}

void Wizard::DrawPolyline(cairo_t *cr)
{
    std::vector<GdkPoint> polyline_path = pages_[page_index_]->GetPolylinePath();
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.9);
    cairo_set_line_width(cr, 2);

    cairo_move_to(cr, polyline_path[0].x, polyline_path[0].y);
    size_t num = polyline_path.size();
    for (size_t i = 1; i < num; i++)
    {
         cairo_line_to(cr, polyline_path[i].x, polyline_path[i].y);
    }
    cairo_stroke(cr);

    DrawRing(cr, polyline_path[0].x, polyline_path[0].y);
    DrawRing(cr, polyline_path[num-1].x, polyline_path[num-1].y);
}

void Wizard::DrawRing(cairo_t *cr, int x, int y)
{
    cairo_set_source_rgba(cr, 1., 1., 1., 0.1);
    cairo_arc(cr, x, y, 10, 0., 2 * M_PI);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, 1., 1., 1., 0.4);
    cairo_arc(cr, x, y, 7, 0., 2 * M_PI);
    cairo_fill(cr);

    cairo_set_source_rgba(cr, 1., 1., 1., 0.9);
    cairo_arc(cr, x, y, 4, 0., 2 * M_PI);
    cairo_fill(cr);
}

void Wizard::ClipRec(cairo_t *cr)
{
    GdkRectangle *rec;
    rec = pages_[page_index_]->GetTransparentRect();

    cairo_save(cr);
    cairo_rectangle(cr, rec->x, rec->y, rec->width, rec->height);
    cairo_clip(cr);
    cairo_new_path(cr);
    cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_restore(cr);
}
