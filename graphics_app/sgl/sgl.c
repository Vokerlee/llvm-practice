#include "sgl.h"

#include <cairo.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdlib.h>

static void activate(GtkApplication *app, gpointer user_data);
static void close_window(GtkWidget *widget, gpointer user_data);
static gboolean idle_callback(gpointer user_data);

struct sgl_window_info
{
    int width;
    int height;

    GtkApplication *app;
    const char *window_title;

    unsigned char *pixels_old;
    unsigned char *pixels_new;

    int initialized;

    void (*init_func)(unsigned char *, unsigned char *, int, int);
    void (*draw_func)(unsigned char *, unsigned char *, int, int);
};

sgl_window_info_t *sgl_get_window_info()
{
    return calloc(1, sizeof(sgl_window_info_t));
}

int sgl_set_window_title(sgl_window_info_t *win_info, const char *window_title)
{
    if (win_info == NULL)
        return -1;

    win_info->window_title = window_title;
}

int sgl_set_draw_func(sgl_window_info_t *win_info, void (*draw_func)(unsigned char *, unsigned char *, int, int))
{
    if (win_info == NULL)
        return -1;

    win_info->draw_func = draw_func;
}

int sgl_set_init_func(sgl_window_info_t *win_info, void (*init_func)(unsigned char *, unsigned char *, int, int))
{
    if (win_info == NULL)
        return -1;

    win_info->init_func = init_func;
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
    win_info->pixels_old = calloc(win_info->width * win_info->height * 4, sizeof(unsigned char));
    if (win_info->pixels_old == NULL)
        return -1;

    win_info->pixels_new = calloc(win_info->width * win_info->height * 4, sizeof(unsigned char));
    if (win_info->pixels_new == NULL)
        return -1;

    win_info->initialized = 0;

    win_info->app = gtk_application_new("graph.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(win_info->app, "activate", G_CALLBACK(activate), win_info);

    return g_application_run(G_APPLICATION(win_info->app), argc, argv);
}

int sgl_close(sgl_window_info_t *win_info)
{
    free(win_info->pixels_old);
    free(win_info->pixels_new);
    g_object_unref(win_info->app);

    free(win_info);

    return 0;
}

void draw_callback(GtkDrawingArea *drawing_area, cairo_t *cr, int width, int height, gpointer data)
{
    sgl_window_info_t *win_info = (sgl_window_info_t *) data;

    if (win_info->initialized != 0 && win_info->draw_func != NULL)
        win_info->draw_func(win_info->pixels_old, win_info->pixels_new, width, height);
    else if (win_info->init_func != NULL)
    {
        win_info->init_func(win_info->pixels_old, win_info->pixels_new, width, height);
        win_info->initialized = 1;
    }

    cairo_surface_t *surface = cairo_image_surface_create_for_data((unsigned char *) win_info->pixels_new,
                                                                   CAIRO_FORMAT_ARGB32, width, height, width * 4);

    memcpy(win_info->pixels_old, win_info->pixels_new, width * height * 4);

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
    if (win_info->window_title != NULL)
        gtk_window_set_title(GTK_WINDOW(window), win_info->window_title);
    else
        gtk_window_set_title(GTK_WINDOW(window), "Application");

    gtk_window_set_default_size(GTK_WINDOW(window), win_info->width, win_info->height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

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

int sgl_rand()
{
    return rand();
}

void sgl_srand(int seed)
{
    srand(seed);
}
