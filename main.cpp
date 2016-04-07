#include "blur.h"
#include "draw.h"
#include "style.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <pango/pango.h>

#include <iostream>


#define PAGES_NUM 2

const gint PANEL_HEIGHT = 24;
const gint LAUNCHER_SIZE = 64;

static gboolean on_close_pressed(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
  if (event->button == 1)
    gtk_main_quit();
  return FALSE;
}

static gboolean enter_close_box(GtkWidget *widget, GdkEventButton *event, Info *info)
{
  gtk_image_set_from_file(WID(info->builder, IMAGE, "close_img"), PKGDATADIR"/close_hover.png");
  return FALSE;
}

static gboolean leave_close_box(GtkWidget *widget, GdkEventButton *event, Info *info)
{
  gtk_image_set_from_file(WID(info->builder, IMAGE, "close_img"), PKGDATADIR"/close.png");
  return FALSE;
}

static gboolean on_arrow_left_pressed(GtkWidget *widget, GdkEventButton *event, Info *info)
{
  if (event->button == 1 && info->index != 0)
  {
    info->index--;
    gtk_widget_queue_draw(WID(info->builder, WIDGET, "window"));
  }
  return FALSE;
}

static gboolean enter_left_box(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
//  gtk_image_set_from_file(GTK_IMAGE(widget), PKGDATADIR"/");
  return FALSE;
}

static gboolean leave_left_box(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
//  gtk_image_set_from_file(GTK_IMAGE(widget), PKGDATADIR"/");
  return FALSE;
}

static gboolean on_arrow_right_pressed(GtkWidget *widget, GdkEventButton *event, Info *info)
{
  if (event->button == 1 && info->index != PAGES_NUM - 1)
  {
    info->index++;
    gtk_widget_queue_draw(WID(info->builder, WIDGET, "window"));
  }
  return FALSE;
}

static gboolean enter_right_box(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
//  gtk_image_set_from_file(GTK_IMAGE(widget), PKGDATADIR"/");
  return FALSE;
}

static gboolean leave_right_box(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
//  gtk_image_set_from_file(GTK_IMAGE(widget), PKGDATADIR"/");
  return FALSE;
}

static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
  switch(event->keyval)
  {
  case GDK_KEY_Escape:
    gtk_main_quit();
    break;
  default:
    break;
  }
}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, Info *info)
{      
  do_drawing(cr, info);

  return FALSE;
}

static void setup(GtkWidget *win)
{        
  gtk_widget_set_app_paintable(win, TRUE);
  gtk_window_set_type_hint(GTK_WINDOW(win), GDK_WINDOW_TYPE_HINT_DOCK);
  gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

  GdkScreen *screen = gdk_screen_get_default();
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
  gint width = gdk_screen_get_width(screen);
  gint height = gdk_screen_get_height(screen);
  gtk_window_set_default_size(GTK_WINDOW(win), width, height);

  if (visual != NULL && gdk_screen_is_composited(screen)) {
      gtk_widget_set_visual(win, visual);
  }
}

int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *left_box;
  GtkWidget *right_box;
  GtkWidget *close_box;
  GtkWidget *arrow_left_img;
  GtkWidget *arrow_right_img;
  GtkWidget *base_img;
  GtkWidget *close_img;
  GtkWidget *page_ind;
  GtkWidget *title1;
  GtkWidget *title2;
  GtkWidget *details;
  GtkWidget *lightspot1;
  GtkWidget *lightspot2;
  GError *error = NULL;

  Info info;

  gtk_init(&argc, &argv);

  info.builder = gtk_builder_new();
  gtk_builder_add_from_file(info.builder, PKGDATADIR"/wizard.ui", &error);
  if (!info.builder)
  {
    std::cout << "error: " << error->message << std::endl;
  }
  window = WID(info.builder, WIDGET, "window");
  
  setup(window);

  fixed = WID(info.builder, WIDGET, "fixed");
  
  GdkPixbuf *image_buf = gdk_pixbuf_new_from_file_at_size(PKGDATADIR"/computer.png", 422, 334, &error);
  if (!image_buf) {
    std::cout << "error message: " << error->message << std::endl;
    return -1;
  }
  GdkPixbuf *preview = gdk_pixbuf_copy (image_buf);

  GdkWindow *root_win = gdk_get_default_root_window();
  gint width = gdk_window_get_width(root_win);
  gint height = gdk_window_get_height(root_win);
  info.root_pixbuf = gdk_pixbuf_get_from_window(root_win, 0, 0, width, height);
  GdkPixbuf *tmp = gdk_pixbuf_scale_simple(info.root_pixbuf, 385, 230, GDK_INTERP_NEAREST);

  if (tmp)
  {
    gdk_pixbuf_composite(tmp, preview,
                         17, 22, 385, 230, 17, 22, 1,1,GDK_INTERP_NEAREST, 255);
    g_object_unref(tmp);
  }
  base_img = WID(info.builder, WIDGET, "base_img");
  gtk_image_set_from_pixbuf(GTK_IMAGE(base_img), preview);
  gtk_fixed_move(GTK_FIXED(fixed), base_img, get_base_pos().x, get_base_pos().y);

  title1 = WID(info.builder, WIDGET, "title1");
  gtk_label_set_text(GTK_LABEL(title1), "如何快速启动应用程序");
  gtk_fixed_move(GTK_FIXED(fixed), title1, get_title1_pos().x, get_title1_pos().y);

  title2 = WID(info.builder, WIDGET, "title2");
  gtk_label_set_text(GTK_LABEL(title2), "Launcher");
  gtk_fixed_move(GTK_FIXED(fixed), title2, get_title2_pos().x, get_title2_pos().y);

  details = WID(info.builder, WIDGET, "details");
  gtk_label_set_text(GTK_LABEL(details), "Ubuntu特有的快速启动面板，可以方便快捷的打开和切换各种应用。同时可以根据使用习惯自由定制Launcher面板上的应用。");
  gtk_widget_set_size_request(details, 500, 50);
  gtk_label_set_line_wrap(GTK_LABEL(details), TRUE);
  gtk_fixed_move(GTK_FIXED(fixed), details, get_details_pos().x, get_details_pos().y);

  PangoFontDescription *fd = pango_font_description_from_string("Serif 20");
  gtk_widget_override_font(title1, fd);

  GdkColor color;
  gdk_color_parse("white", &color);
  gtk_widget_modify_fg(title1, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_fg(title2, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_fg(details, GTK_STATE_NORMAL, &color);

  left_box = WID(info.builder, WIDGET, "left_box");
  arrow_left_img = WID(info.builder, WIDGET, "arrow_left_img");
  gtk_image_set_from_file(GTK_IMAGE(arrow_left_img), PKGDATADIR"/arrow_previous.svg");
  gtk_fixed_move(GTK_FIXED(fixed), left_box, get_left_arrow_pos().x, get_left_arrow_pos().y);

  right_box = WID(info.builder, WIDGET, "right_box");
  arrow_right_img = WID(info.builder, WIDGET, "arrow_right_img");
  gtk_image_set_from_file(GTK_IMAGE(arrow_right_img), PKGDATADIR"/arrow_next.svg");
  gtk_fixed_move(GTK_FIXED(fixed), right_box, get_right_arrow_pos().x, get_right_arrow_pos().y);

  close_box = WID(info.builder, WIDGET, "close_box");
  close_img = WID(info.builder, WIDGET, "close_img");
  gtk_image_set_from_file(GTK_IMAGE(close_img), PKGDATADIR"/close.png");
  gtk_fixed_move(GTK_FIXED(fixed), close_box, get_close_pos().x, get_close_pos().y);

  page_ind = WID(info.builder, WIDGET, "page_ind");
  gtk_image_set_from_file(GTK_IMAGE(page_ind), PKGDATADIR"/step_1.png");
  gtk_fixed_move(GTK_FIXED(fixed), page_ind, get_page_ind_pos().x, get_page_ind_pos().y);

  lightspot1 = WID(info.builder, WIDGET, "lightspot1");
  gtk_image_set_from_file(GTK_IMAGE(lightspot1), PKGDATADIR"/lightspot.png");
  gtk_fixed_move(GTK_FIXED(fixed), lightspot1, get_spot1_pos("Nautilus").x, get_spot1_pos("Nautilus").y);

  lightspot2 = WID(info.builder, WIDGET, "lightspot2");
  gtk_image_set_from_file(GTK_IMAGE(lightspot2), PKGDATADIR"/lightspot.png");
  gtk_fixed_move(GTK_FIXED(fixed), lightspot2, get_spot2_pos().x, get_spot2_pos().y);

  g_signal_connect(G_OBJECT(window), "draw",
      G_CALLBACK(on_draw_event), &info);
  g_signal_connect(G_OBJECT(window), "key-press-event",
      G_CALLBACK(on_key_press), NULL);

  g_signal_connect(G_OBJECT(close_box), "button_press_event",
      G_CALLBACK(on_close_pressed), NULL);
  g_signal_connect(G_OBJECT(close_box), "enter_notify_event",
      G_CALLBACK(enter_close_box), &info);
  g_signal_connect(G_OBJECT(close_box), "leave_notify_event",
      G_CALLBACK(leave_close_box), &info);

  g_signal_connect(G_OBJECT(left_box), "button_press_event",
      G_CALLBACK(on_arrow_left_pressed), &info);
  g_signal_connect(G_OBJECT(left_box), "enter_notify_event",
      G_CALLBACK(enter_left_box), NULL);
  g_signal_connect(G_OBJECT(left_box), "leave_notify_event",
      G_CALLBACK(leave_left_box), NULL);

  g_signal_connect(G_OBJECT(right_box), "button_press_event",
      G_CALLBACK(on_arrow_right_pressed), &info);
  g_signal_connect(G_OBJECT(right_box), "enter_notify_event",
      G_CALLBACK(enter_right_box), NULL);
  g_signal_connect(G_OBJECT(right_box), "leave_notify_event",
      G_CALLBACK(leave_right_box), NULL);

//  gtk_widget_set_events(window, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK);

  gtk_widget_show_all(window);

  gtk_main();

//  g_object_unref(G_OBJECT(info.builder));

  return 0;
}
