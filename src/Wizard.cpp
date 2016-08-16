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

#include "Wizard.h"
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

bool IsInstalled(std::string name)
{
    if (g_find_program_in_path(name.c_str()))
        return true;
    else
        return false;
}

static bool draw(GtkWidget *widget, cairo_t *cr, Wizard* w)
{
    w->DoDraw(cr);
    return FALSE;
}

static bool quit(GtkWidget *widget, GdkEventKey *event, Wizard *w)
{
    w->Quit();
    return FALSE;
}

static bool on_key_pressed(GtkWidget *widget, GdkEventKey *event, Wizard *w)
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

static bool left_arrow_pressed(GtkWidget *widget, GdkEventButton *event, Wizard *w)
{
  if (event->button == 1)
  {
    w->PrePage();
  }
  return FALSE;
}

static bool right_arrow_pressed(GtkWidget *widget, GdkEventButton *event, Wizard *w)
{
  if (event->button == 1)
  {
    w->NextPage();
  }
  return FALSE;
}

static bool window_focus_changed(GtkWidget *widget, GParamSpec *psepc)
{
    // TODO: didn't work here...
    //gtk_window_present(GTK_WINDOW(widget));
    return FALSE;
}

Wizard::Wizard()
    : page_index_(0)
    , pre_index_(0)
    , builder_(gtk_builder_new_from_file(PKGDATADIR"/wizard.ui"))
{   
    Settings settings;
    win_ = WID(builder_, WIDGET, "window");
    grid_ = WID(builder_, WIDGET, "grid");
    fixed_ = WID(builder_, WIDGET, "fixed");
    GtkWidget *right_box_ = WID(builder_, WIDGET, "right_box");
    GtkWidget *left_box_ = WID(builder_, WIDGET, "left_box");

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

    const std::string *thumbnail_name;
    gtk_widget_set_size_request(GTK_WIDGET(win_), screen_width_, screen_height_);
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

    GtkWidget *close_button = WID(builder_, WIDGET, "close_button");

    g_signal_connect(G_OBJECT(close_button), "button_press_event",
        G_CALLBACK(quit), this);

    g_signal_connect(G_OBJECT(left_box_), "button_press_event",
        G_CALLBACK(left_arrow_pressed), this);

    g_signal_connect(G_OBJECT(right_box_), "button_press_event",
        G_CALLBACK(right_arrow_pressed), this);

    g_signal_connect(G_OBJECT(win_), "key-press-event",
        G_CALLBACK(on_key_pressed), this);

    g_signal_connect(G_OBJECT(win_), "notify::has-toplevel-focus",
        G_CALLBACK(window_focus_changed), NULL);

    base_window_ = new BaseWindow(builder_, screen_width_, screen_height_);

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

    // Deal with multiple-screen
    int screen_num = gdk_screen_get_n_monitors(screen);
    if (screen_num > 1)
        DrawOtherMonitor(screen_num);
}

Wizard::~Wizard()
{
    size_t len = pages_.size();
    for (size_t i=0; i < len; i++)
    {
        delete pages_[i];
    }
}

void Wizard::DoDraw(cairo_t *cr)
{
    base_window_->Show(cr);
    pages_[page_index_]->SetActive(true);
    pages_[page_index_]->Draw(cr);
}

void Wizard::Show()
{
    g_signal_connect(G_OBJECT(win_), "draw",
        G_CALLBACK(draw), this);

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
        pages_[page_index_]->SetActive(false);
        page_index_--;
        gtk_widget_queue_draw(win_);
    }
}

void Wizard::NextPage()
{
    if (page_index_ + 1 < pages_.size())
    {
        pages_[page_index_]->SetActive(false);
        page_index_++;
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
