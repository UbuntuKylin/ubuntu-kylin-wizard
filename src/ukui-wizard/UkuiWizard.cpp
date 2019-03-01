#include <gtk/gtk.h>

#include "wizard.h"

#define WID(builder, type, s) GTK_##type (gtk_builder_get_object(builder, s))

enum {
    COLUMN_PIXBUF,
    COLUMN_STRING,
    N_COLUMNS
};

static gboolean exit_cb (GtkWidget *widget, GdkEventKey *event, gpointer *data)
{
    gtk_main_quit();
    return FALSE;
}

Wizard::Wizard()
    : builder_(gtk_builder_new_from_file("wizard.ui"))
{
    gint i;

    /* main windown */
    GtkWidget *win_ = WID(builder_, WIDGET, "assistant");
    gtk_window_set_default_size (GTK_WINDOW(win_), 860, 580);
    gtk_window_set_position (GTK_WINDOW(win_), GTK_WIN_POS_CENTER);

    /* The first page */
    GtkWidget *page = gtk_assistant_get_nth_page (GTK_ASSISTANT (win_), 0);
    gtk_assistant_set_page_type (GTK_ASSISTANT (win_), page, GTK_ASSISTANT_PAGE_CUSTOM);

    /* close button */
    GtkWidget *close_button = WID (builder_, WIDGET, "close_button");
    GtkWidget *close_img = gtk_image_new_from_icon_name ("gtk-close", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image (GTK_BUTTON(close_button), close_img);
    g_signal_connect (close_button, "button_press_event",
                      G_CALLBACK(exit_cb), NULL);

    /* title */
    GtkWidget *title = WID (builder_, WIDGET, "title");
//    const char* str = "Here, you can install the third-party software from ubuntu-kylin-software-center";
    const char* str = "在此，您可以安装以下来自“麒麟软件中心”的第三方应用。";
    const char* format = "<span font=\"14.5\">\%s</span>";
    char *markup = g_markup_printf_escaped (format, str);
    gtk_label_set_markup (GTK_LABEL (title), markup);
    g_free (markup);

    /* icon view */
    icon_view_ = WID (builder_, WIDGET, "icon_view");
    fill_icon_view();

    gtk_icon_view_set_item_orientation(GTK_ICON_VIEW(icon_view_), GTK_ORIENTATION_HORIZONTAL);

    gtk_widget_show_all(win_);
}

//char *apps[] = {"wechat", "wps", "qq", "sogoupinyin"};
void
Wizard::fill_icon_view ()
{
    GdkPixbuf *icon;
    GtkTreeIter iter;
    GError *error;
    error = NULL;
    int n = 21;
    GtkListStore *list_store = gtk_list_store_new (2, GDK_TYPE_PIXBUF, G_TYPE_STRING);

    for (int i = 0; i < n; i++) {
        icon = gdk_pixbuf_new_from_file ("wechat.png", &error);
        if (error != NULL) {
            g_print ("error: %s", error->message);
            g_free(error);
        }
        for (i = 0; i < 21; i++) {
            gtk_list_store_append (list_store, &iter);
            gtk_list_store_set (list_store, &iter,
                                COLUMN_PIXBUF, icon,
                                COLUMN_STRING, "hello, this is wps \n and this is another line!",
                                -1);
        }
    }
    gtk_icon_view_set_pixbuf_column (GTK_ICON_VIEW(icon_view_), 0);
    gtk_icon_view_set_markup_column (GTK_ICON_VIEW(icon_view_), 1);
    gtk_icon_view_set_model (GTK_ICON_VIEW(icon_view_), GTK_TREE_MODEL(list_store));
}
