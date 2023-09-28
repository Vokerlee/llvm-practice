#include "sgl.h"

#include <stdlib.h>
#include <stdio.h>

void draw_func(unsigned char *pixels, int width, int height)
{
    static int l = 0;
    l++;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 4;

            pixels[index]     = l;  // Red
            pixels[index + 1] = l;  // Green
            pixels[index + 2] = 0;    // Blue
            pixels[index + 3] = 255;  // Fully opaque
        }
    }
}

int main(int argc, char *argv[])
{
    sgl_window_info_t *win_info = sgl_get_window_info();
    sgl_set_window_size(win_info, SGL_WIDTH_DEFAULT, SGL_HEIGHT_DEFAULT);
    sgl_set_draw_func(win_info, draw_func);

    int init_status = sgl_initialize(win_info, argc, argv);
    if (init_status != 0)
        return EXIT_FAILURE;

    int close_status = sgl_close(win_info);
    if (close_status != 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
