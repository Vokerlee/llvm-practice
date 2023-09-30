#include "sgl.h"

#include <stdlib.h>
#include <stdio.h>

void init_func(unsigned char *pixels_old, unsigned char *pixels_new, int width, int height);
void draw_func(unsigned char *pixels_old, unsigned char *pixels_new, int width, int height);

int main(int argc, char *argv[])
{
    sgl_window_info_t *win_info = sgl_get_window_info();
    sgl_set_window_size(win_info, SGL_WIDTH_DEFAULT, SGL_HEIGHT_DEFAULT);
    sgl_set_init_func(win_info, init_func);
    sgl_set_draw_func(win_info, draw_func);

    int init_status = sgl_initialize(win_info, argc, argv);
    if (init_status != 0)
        return EXIT_FAILURE;

    int close_status = sgl_close(win_info);
    if (close_status != 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
