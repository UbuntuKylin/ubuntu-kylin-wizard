#include "draw.h"

#include "style.h"
#include "blur.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/gdkkeysyms.h>
#include <pango/pango.h>

#include <iostream>

void clip_rec(cairo_t *cr, int x, int y, int width, int height)
{
  cairo_save(cr);
  cairo_rectangle(cr, x, y, width, height);
  cairo_clip(cr);
  cairo_new_path(cr);
  cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
  cairo_restore(cr);
}

static gboolean on_close_pressed(GtkWidget *widget, GdkEventButton *event, gpointer *user_data)
{
  if (event->button == 1)
    gtk_main_quit();
  return FALSE;
}

static gboolean enter_close_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "close_img"), PKGDATADIR"/close_hover.png");
  return FALSE;
}

static gboolean leave_close_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "close_img"), PKGDATADIR"/close.png");
  return FALSE;
}

static gboolean on_arrow_left_pressed(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  if (event->button == 1 && draw->page() != 0)
  {
    draw->pre_page();
    gtk_widget_queue_draw(WID(draw->Builer(), WIDGET, "window"));
  }
  return FALSE;
}

static gboolean enter_left_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "arrow_left_img"), PKGDATADIR"/arrow_left_hover.png");
  return FALSE;
}

static gboolean leave_left_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "arrow_left_img"), PKGDATADIR"/arrow_left.png");
  return FALSE;
}

static gboolean on_arrow_right_pressed(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  if (event->button == 1 && draw->page() != PAGES_NUM - 1)
  {
    draw->next_page();
    gtk_widget_queue_draw(WID(draw->Builer(), WIDGET, "window"));
  }
  return FALSE;
}

static gboolean enter_right_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "arrow_right_img"), PKGDATADIR"/arrow_right_hover.png");
  return FALSE;
}

static gboolean leave_right_box(GtkWidget *widget, GdkEventButton *event, Draw *draw)
{
  gtk_image_set_from_file(WID(draw->Builer(), IMAGE, "arrow_right_img"), PKGDATADIR"/arrow_right.png");
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

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, Draw* draw)
{
  draw->do_drawing(cr);

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

Draw::Draw()
  : builder_(gtk_builder_new_from_file(PKGDATADIR"/wizard.ui"))
  , style_(new Style())
  , page_num_(0)
{
  GError *error = NULL;

  window_ = WID(builder_, WIDGET, "window");
  setup(window_);

  fixed_ = WID(builder_, WIDGET, "fixed");

  root_pixbuf_ = gdk_pixbuf_get_from_window(gdk_get_default_root_window(), 0, 0, style_->get_root_width(), style_->get_root_height());

//  // Get the thumbnail from current screen.
//  GdkPixbuf *image_buf = gdk_pixbuf_new_from_file_at_size(PKGDATADIR"/computer.png", 422, 334, &error);
//  if (!image_buf) {
//    std::cout << "error message: " << error->message << std::endl;
//  }
//  GdkPixbuf *preview = gdk_pixbuf_copy (image_buf);

//  GdkPixbuf *tmp = gdk_pixbuf_scale_simple(root_pixbuf_, 385, 230, GDK_INTERP_NEAREST);

//  if (tmp)
//  {
//    gdk_pixbuf_composite(tmp, preview,
//                         17, 22, 385, 230, 17, 22, 1,1,GDK_INTERP_NEAREST, 255);
//    g_object_unref(tmp);
//  }
//  base_img_ = WID(builder_, WIDGET, "base_img");
//  gtk_image_set_from_pixbuf(GTK_IMAGE(base_img_), preview);
//  gtk_fixed_move(GTK_FIXED(fixed_), base_img_, style_->get_base_pos().x, style_->get_base_pos().y);

  base_img_ = WID(builder_, WIDGET, "base_img");
  GdkPixbuf *base_pixbuf = gdk_pixbuf_new_from_file(PKGDATADIR"/computer.png", &error);
  gtk_image_set_from_pixbuf(GTK_IMAGE(base_img_), base_pixbuf);
  gtk_fixed_move(GTK_FIXED(fixed_), base_img_, style_->get_base_pos().x, style_->get_base_pos().y);

  thumbnail_ = WID(builder_, WIDGET, "thumbnail");
  gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_1.png");
  gtk_fixed_move(GTK_FIXED(fixed_), thumbnail_, style_->get_base_pos().x + 17, style_->get_base_pos().y + 24);

  grid_ = WID(builder_, WIDGET, "grid");
  gtk_fixed_move(GTK_FIXED(fixed_), grid_, style_->get_title1_pos().x, style_->get_title1_pos().y);

  title1_ = WID(builder_, WIDGET, "title1");
  gtk_label_set_text(GTK_LABEL(title1_), "如何快速启动应用程序");

  title2_ = WID(builder_, WIDGET, "title2");
  gtk_label_set_text(GTK_LABEL(title2_), "Launcher");

  details_ = WID(builder_, WIDGET, "details");
  gtk_label_set_text(GTK_LABEL(details_), "Ubuntu特有的快速启动面板，可以方便快捷的打开和切换各种应用。同时可以根据使用习惯自由定制Launcher面板上的应用。");
  gtk_widget_set_size_request(details_, 0.8 * (style_->get_right_arrow_pos().x - style_->get_spot_pos().x), -1);
  gtk_label_set_line_wrap(GTK_LABEL(details_), TRUE);

  PangoFontDescription *fd_1 = pango_font_description_from_string("Serif 30");
  gtk_widget_override_font(title1_, fd_1);
  PangoFontDescription *fd_2 = pango_font_description_from_string("Serif 24");
  gtk_widget_override_font(title2_, fd_2);
  PangoFontDescription *fd_3 = pango_font_description_from_string("Serif 18");
  gtk_widget_override_font(details_, fd_3);

  GdkColor color;
  gdk_color_parse("white", &color);
  gtk_widget_modify_fg(title1_, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_fg(title2_, GTK_STATE_NORMAL, &color);
  gtk_widget_modify_fg(details_, GTK_STATE_NORMAL, &color);

  left_box_ = WID(builder_, WIDGET, "left_box");
  arrow_left_img_ = WID(builder_, WIDGET, "arrow_left_img");
  gtk_image_set_from_file(GTK_IMAGE(arrow_left_img_), PKGDATADIR"/arrow_left.png");
  gtk_fixed_move(GTK_FIXED(fixed_), left_box_, style_->get_left_arrow_pos().x, style_->get_left_arrow_pos().y);

  right_box_ = WID(builder_, WIDGET, "right_box");
  arrow_right_img_ = WID(builder_, WIDGET, "arrow_right_img");
  gtk_image_set_from_file(GTK_IMAGE(arrow_right_img_), PKGDATADIR"/arrow_right.png");
  gtk_fixed_move(GTK_FIXED(fixed_), right_box_, style_->get_right_arrow_pos().x, style_->get_right_arrow_pos().y);

  close_box_ = WID(builder_, WIDGET, "close_box");
  close_img_ = WID(builder_, WIDGET, "close_img");
  gtk_image_set_from_file(GTK_IMAGE(close_img_), PKGDATADIR"/close.png");
  gtk_fixed_move(GTK_FIXED(fixed_), close_box_, style_->get_close_pos().x, style_->get_close_pos().y);

  page_ind_ = WID(builder_, WIDGET, "page_ind");
  gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_1.png");
  gtk_fixed_move(GTK_FIXED(fixed_), page_ind_, style_->get_page_ind_pos().x, style_->get_page_ind_pos().y);

  lightspot1_ = WID(builder_, WIDGET, "lightspot1");
  gtk_image_set_from_file(GTK_IMAGE(lightspot1_), PKGDATADIR"/lightspot.png");
  gtk_fixed_move(GTK_FIXED(fixed_), lightspot1_, style_->get_spot_pos().x - 0.5 * LIGHTSPOT_SIZE, style_->get_spot_pos().y - 0.5 * LIGHTSPOT_SIZE);

  lightspot2_ = WID(builder_, WIDGET, "lightspot2");
  gtk_image_set_from_file(GTK_IMAGE(lightspot2_), PKGDATADIR"/lightspot.png");

  g_signal_connect(G_OBJECT(window_), "draw",
      G_CALLBACK(on_draw_event), this);
  g_signal_connect(G_OBJECT(window_), "key-press-event",
      G_CALLBACK(on_key_press), NULL);

  g_signal_connect(G_OBJECT(close_box_), "button_press_event",
      G_CALLBACK(on_close_pressed), NULL);
  g_signal_connect(G_OBJECT(close_box_), "enter_notify_event",
      G_CALLBACK(enter_close_box), this);
  g_signal_connect(G_OBJECT(close_box_), "leave_notify_event",
      G_CALLBACK(leave_close_box), this);

  g_signal_connect(G_OBJECT(left_box_), "button_press_event",
      G_CALLBACK(on_arrow_left_pressed), this);
  g_signal_connect(G_OBJECT(left_box_), "enter_notify_event",
      G_CALLBACK(enter_left_box), this);
  g_signal_connect(G_OBJECT(left_box_), "leave_notify_event",
      G_CALLBACK(leave_left_box), this);

  g_signal_connect(G_OBJECT(right_box_), "button_press_event",
      G_CALLBACK(on_arrow_right_pressed), this);
  g_signal_connect(G_OBJECT(right_box_), "enter_notify_event",
      G_CALLBACK(enter_right_box), this);
  g_signal_connect(G_OBJECT(right_box_), "leave_notify_event",
      G_CALLBACK(leave_right_box), this);
}

void Draw::Run()
{
  gtk_widget_show_all(window_);
}

GtkBuilder* Draw::Builer()
{
  return builder_;
}

void Draw::do_drawing(cairo_t *cr)
{
  draw_background(cr);
  draw_page(cr);
}

void Draw::draw_background(cairo_t *cr)
{
  cairo_surface_t *bg_sur = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, style_->get_root_width(), style_->get_root_height());
  cairo_t *bg_cr = cairo_create(bg_sur);
  gdk_cairo_set_source_pixbuf(bg_cr, root_pixbuf_, 0, 0);
  cairo_paint(bg_cr);

  blur(bg_sur, 8);
  cairo_set_source_surface(cr, bg_sur, 0, 0);
  cairo_paint(cr);

  cairo_destroy(bg_cr);
  cairo_surface_destroy(bg_sur);
}

void Draw::draw_page(cairo_t *cr)
{
  gint root_height = style_->get_root_height();
  gint root_width = style_->get_root_width();
  gint launcher_size = style_->get_launcher_size();
  gint panel_height = style_->get_panel_height();
  gint icon_size = style_->get_icon_size();

  switch (page_num_) {
  case 0:
    draw_polyline(cr);
    clip_rec(cr, 0, root_height - launcher_size, root_width, launcher_size);
    gtk_label_set_text(GTK_LABEL(title1_), "如何快速启动应用程序");
    gtk_label_set_text(GTK_LABEL(title2_), "Launcher");
    gtk_label_set_text(GTK_LABEL(details_), "Ubuntu特有的快速启动面板，可以方便快捷的打开和切换各种应用。同时可以根据使用习惯自由定制Launcher面板上的应用。");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_1.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_1.png");
    break;
 case 1:
    draw_polyline(cr);
    clip_rec(cr, SPACE_BETWEEN_ICONS.CP(style_->cv_), root_height - launcher_size + ICON_PADDING.CP(style_->cv_), icon_size, icon_size);
    gtk_label_set_text(GTK_LABEL(title1_), "快速的智能搜索");
    gtk_label_set_text(GTK_LABEL(title2_), "Dash");
    gtk_label_set_text(GTK_LABEL(details_), "点击此处可以打开Dash界面，Dash可以提供强大的快速智能搜索功能，可以方便快捷的搜索并打开本地和网络的各种资源，包括：应用、文件、音乐、视频等。");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_2.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_2.png");
    break;
  case 2:
    draw_polyline(cr);
    clip_rec(cr, style_->icon_pos_[page_num_].x - 0.5 * icon_size, root_height - launcher_size + ICON_PADDING.CP(style_->cv_), icon_size, icon_size);
    gtk_label_set_text(GTK_LABEL(title1_), "浏览并管理您的文件");
    gtk_label_set_text(GTK_LABEL(title2_), "文件管理器");
    gtk_label_set_text(GTK_LABEL(details_), "可以通过此处打开文件管理器，它可以方便的浏览和管理系统中的各种数据。");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_3.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_3.png");
    break;
  case 3:
    draw_polyline(cr);
    clip_rec(cr, style_->icon_pos_[page_num_].x - 0.5 * icon_size, root_height - launcher_size + ICON_PADDING.CP(style_->cv_), icon_size, icon_size);
    gtk_label_set_text(GTK_LABEL(title1_), "查看和修改系统设置");
    gtk_label_set_text(GTK_LABEL(title2_), "优客助手");
    gtk_label_set_text(GTK_LABEL(details_), "优麒麟为用户打造的系统管理和配置工具，具备强大的系统信息展示、一键垃圾清理和系统定制美化等功能");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_4.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_4.png");
    break;
  case 4:
    draw_polyline(cr);
    clip_rec(cr, style_->icon_pos_[page_num_].x - 0.5 * icon_size, root_height - launcher_size + ICON_PADDING.CP(style_->cv_), icon_size, icon_size);
    gtk_label_set_text(GTK_LABEL(title1_), "如何合理的管理系统配置");
    gtk_label_set_text(GTK_LABEL(title2_), "控制面版");
    gtk_label_set_text(GTK_LABEL(details_), "允许用户查看并操作基本的系统设置。");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_5.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_5.png");
    break;
  case 5:
    draw_polyline(cr);
    clip_rec(cr, 0, 0, root_width, panel_height);
    gtk_label_set_text(GTK_LABEL(title1_), "查看系统基本状态");
    gtk_label_set_text(GTK_LABEL(title2_), "Indicator");
    gtk_label_set_text(GTK_LABEL(details_), "可以在此区域方便快捷的查看和配置系统声音、网络、时间和消息等。");
    gtk_image_set_from_file(GTK_IMAGE(page_ind_), PKGDATADIR"/step_6.png");
    gtk_image_set_from_file(GTK_IMAGE(thumbnail_), PKGDATADIR"/thumbnail_6.png");
    break;
  default:
    break;
  }
}

void Draw::draw_polyline(cairo_t *cr)
{
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 8.0);
  cairo_set_line_width(cr, 2);

  Point p;
  p.x = style_->icon_pos_[page_num_].x;
  p.y = style_->icon_pos_[page_num_].y;

  gtk_fixed_move(GTK_FIXED(fixed_), lightspot2_,
                 p.x - 0.5 * LIGHTSPOT_SIZE, p.y - 0.5 * LIGHTSPOT_SIZE);
  cairo_move_to(cr, p.x, p.y);

  gint inflexion_y = 0;
  if (page_num_ != 5)
    inflexion_y = style_->get_inflexion_pos().y;
  else
    inflexion_y = (style_->get_spot_pos().y + style_->get_panel_height()) / 2;

  cairo_line_to(cr, p.x, inflexion_y);
  cairo_line_to(cr, style_->get_inflexion_pos().x, inflexion_y);
  cairo_line_to(cr, style_->get_spot_pos().x, style_->get_spot_pos().y);
  cairo_stroke(cr);
}

gint Draw::page()
{
  return page_num_;
}

void Draw::next_page()
{
  if (page_num_ != PAGES_NUM - 1)
    page_num_++;
}

void Draw::pre_page()
{
  if (page_num_ != 0)
    page_num_--;
}
