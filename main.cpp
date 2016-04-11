#include "blur.h"
#include "draw.h"
#include "style.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <iostream>

int main (int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  GdkScreen *screen = gdk_screen_get_default();
  GdkDisplay *display = gdk_screen_get_display(screen);
  GList* stack =  gdk_screen_get_window_stack(screen);
  for( GList* iter = g_list_last(stack); iter; iter = iter->prev)
  {
    GdkWindow* window = static_cast<GdkWindow*>(iter->data);
    XID xid = GDK_WINDOW_XID(window);
    char *name;
    XFetchName(GDK_DISPLAY_XDISPLAY(display), xid, &name);
    std::cout << name << std::endl;
  }
  g_list_foreach(stack, (GFunc)g_object_unref, NULL);
  g_list_free(stack);

  sleep(5);
  Draw *draw = new Draw();
  draw->Run();

  gtk_main();

  return 0;
}
