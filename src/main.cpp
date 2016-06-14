// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
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

#include "blur.h"
#include "draw.h"
#include "style.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <libintl.h>

#include "config.h"

const std::string UBIQUITY_DESKTOP = "/usr/share/applications/ubiquity.desktop";

gboolean live_mode()
{
  if (g_file_test(UBIQUITY_DESKTOP.c_str(), G_FILE_TEST_EXISTS))
    return true;
  else
    return false;
}

gboolean first_run()
{
  std::string config_dir = g_get_user_config_dir();
  std::string wizard_config = config_dir.append(G_DIR_SEPARATOR_S "unity" G_DIR_SEPARATOR_S);
  if (g_mkdir_with_parents(wizard_config.c_str(), 0700) < 0)
    wizard_config = "";

  if (!wizard_config.empty() && !g_file_test((wizard_config+"first_run.stamp").c_str(), G_FILE_TEST_EXISTS))
  {
    GError *error = NULL;
    g_file_set_contents((wizard_config+"first_run.stamp").c_str(), "", 0, &error);
    if (error != NULL)
    {
      g_printerr("%s. \n", error->message);
      g_error_free(error);
    }
    return true;
  }
  return false;
}

gboolean wait_launcher(GdkScreen *screen, GdkDisplay *display)
{
  GList* stack =  gdk_screen_get_window_stack(screen);

  gboolean launcher_found = false;
  for( GList* iter = g_list_last(stack); iter; iter = iter->prev)
  {
    GdkWindow* window = static_cast<GdkWindow*>(iter->data);
    XID xid = GDK_WINDOW_XID(window);
    char *name;
    XFetchName(GDK_DISPLAY_XDISPLAY(display), xid, &name);
    if (g_strcmp0(name, "unity-launcher") == 0)
    {
      launcher_found = true;
      sleep(2);
      break;
    }
  }
  g_list_foreach(stack, (GFunc)g_object_unref, NULL);
  g_list_free(stack);
  sleep(1);
  return launcher_found;
}

void call_unity_hint()
{
  KeySym key = XK_Super_L;
  GdkDisplay *gdk_display = gdk_display_get_default();
  Display *display = gdk_x11_display_get_xdisplay(gdk_display);
  XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), True, CurrentTime);
  XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), False, 5000);
  XSync(display, False);
}

int main (int argc, char *argv[])
{
  if (live_mode() || !first_run())
    return 0;

  /* i18n init */
  setlocale (LC_ALL, "");
  bindtextdomain(GETTEXT_PACKAGE, LOCALE_DIR);
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  gtk_init(&argc, &argv);

  GdkScreen *screen = gdk_screen_get_default();
  GdkDisplay *display = gdk_screen_get_display(screen);

  gboolean launcher_showed = false;
  while(!launcher_showed)
  {
    launcher_showed = wait_launcher(screen, display);
  }

  Draw *draw = new Draw();
  draw->Run();

  gtk_main();

  sleep(1);
  call_unity_hint();

  return 0;
}