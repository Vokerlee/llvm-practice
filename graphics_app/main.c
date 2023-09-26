// #include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>

#define WIDTH_DEFAULT  1000
#define HEIGHT_DEFAULT 700
#define BYTES_PER_R8G8B8 3

static void fill_row(GByteArray *array, guint8 value, int row_size)
{
    for (guint i = 0; i < row_size; i++)
    {
        g_byte_array_append(array, &value, 1); // R
        g_byte_array_append(array, &value, 1); // G
        g_byte_array_append(array, &value, 1); // B
    }
}

static void add_pixel_picture(GtkPicture *picture)
{
    GByteArray *pixels = g_byte_array_new();

    for (guint i = 0; i <= HEIGHT_DEFAULT; i++)
        fill_row(pixels, 128, WIDTH_DEFAULT);

    gsize height = pixels->len / (WIDTH_DEFAULT * BYTES_PER_R8G8B8);
    g_autoptr(GBytes) bytes = g_byte_array_free_to_bytes(pixels);

    GdkTexture *texture = gdk_memory_texture_new(WIDTH_DEFAULT, height,
                                        GDK_MEMORY_R8G8B8,
                                        bytes,
                                        WIDTH_DEFAULT * BYTES_PER_R8G8B8);
    gtk_picture_set_paintable(picture, GDK_PAINTABLE(texture));
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Graphics app");
    gtk_window_set_default_size(GTK_WINDOW(window), WIDTH_DEFAULT, HEIGHT_DEFAULT);

    GtkWidget *picture = gtk_picture_new();
    gtk_widget_add_css_class(picture, "frame");
    g_object_set(picture,
                "margin-start",  0,
                "margin-end",    0,
                "margin-top",    0,
                "margin-bottom", 0,
                NULL);

    gtk_window_set_child(GTK_WINDOW(window), picture);
    add_pixel_picture(GTK_PICTURE(picture));

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char *argv[])
{
    GtkApplication *app = gtk_application_new("graph.app", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
