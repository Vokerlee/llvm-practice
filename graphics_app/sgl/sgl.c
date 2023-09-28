#include "sgl.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <unistd.h>

static void activate(GtkApplication *app, gpointer user_data);
static void close_window(GtkWidget *widget, gpointer user_data);
static gboolean idle_callback(gpointer user_data);

struct sgl_window_info
{
    int width;
    int height;

    GtkApplication *app;

    unsigned char *pixels;

    void (*draw_func)(unsigned char *, int, int);
};

sgl_window_info_t *sgl_get_window_info()
{
    return calloc(1, sizeof(sgl_window_info_t));
}

int sgl_set_draw_func(sgl_window_info_t *win_info, void (*draw_func)(unsigned char *, int, int))
{
    if (win_info == NULL)
        return -1;

    win_info->draw_func = draw_func;
}

int sgl_set_window_width(sgl_window_info_t *win_info, int width)
{
    if (win_info == NULL)
        return -1;

    win_info->width = width;

    return 0;
}

int sgl_set_window_height(sgl_window_info_t *win_info, int height)
{
    if (win_info == NULL)
        return -1;

    win_info->height = height;

    return 0;
}

int sgl_set_window_size(sgl_window_info_t *win_info, int width, int height)
{
    if (win_info == NULL)
        return -1;

    win_info->width  = width;
    win_info->height = height;

    return 0;
}

int sgl_initialize(sgl_window_info_t *win_info, int argc, char *argv[])
{
    win_info->pixels = calloc(win_info->width * win_info->height * 4, sizeof(unsigned char));
    if (win_info->pixels == NULL)
        return -1;

    win_info->app = gtk_application_new("graph.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(win_info->app, "activate", G_CALLBACK(activate), win_info);

    return g_application_run(G_APPLICATION(win_info->app), argc, argv);
}

int sgl_close(sgl_window_info_t *win_info)
{
    free(win_info->pixels);
    g_object_unref(win_info->app);

    free(win_info);

    return 0;
}

void draw_callback(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data)
{
    sgl_window_info_t *win_info = (sgl_window_info_t *) data;

    win_info->draw_func(win_info->pixels, width, height);

    cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *) win_info->pixels, CAIRO_FORMAT_ARGB32, width, height, width * 4);

    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_surface_flush(surface);

    cairo_paint(cr);

    cairo_surface_destroy(surface);
}

static gboolean idle_callback(gpointer user_data)
{
    if (GTK_IS_WIDGET(user_data))
    {
        gboolean visible = gtk_widget_get_visible(GTK_WIDGET(user_data));
        if (visible)
            gtk_widget_queue_draw(GTK_WIDGET(user_data));
        else
             return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

static void close_window(GtkWidget *widget, gpointer user_data)
{
    g_source_remove_by_user_data(user_data);
}

static void activate(GtkApplication *app, gpointer data)
{
    sgl_window_info_t *win_info = (sgl_window_info_t *) data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Graphics app");
    gtk_window_set_default_size(GTK_WINDOW(window), win_info->width, win_info->height);

    GtkWidget *frame = gtk_frame_new(NULL);
    gtk_window_set_child(GTK_WINDOW(window), frame);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, win_info->width, win_info->height);
    gtk_frame_set_child(GTK_FRAME(frame), drawing_area);

    g_signal_connect(window, "destroy", G_CALLBACK(close_window), drawing_area);
    g_idle_add(idle_callback, drawing_area);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), draw_callback, win_info, NULL);

    gtk_window_present(GTK_WINDOW(window));
}
