#ifndef SGL_INCLUDE_SGL_INCLUDE
#define SGL_INCLUDE_SGL_INCLUDE

#include "sgl_rand.h"

#define SGL_WIDTH_DEFAULT  1080
#define SGL_HEIGHT_DEFAULT 720

struct sgl_window_info;
typedef struct sgl_window_info sgl_window_info_t;

sgl_window_info_t *sgl_get_window_info();

int sgl_set_init_func(sgl_window_info_t *win_info, void (*init_func)(unsigned char *, unsigned char *, int, int));
int sgl_set_draw_func(sgl_window_info_t *win_info, void (*draw_func)(unsigned char *, unsigned char *, int, int));

int sgl_set_window_title (sgl_window_info_t *win_info, const char *window_name);
int sgl_set_window_width (sgl_window_info_t *win_info, int width);
int sgl_set_window_height(sgl_window_info_t *win_info, int height);
int sgl_set_window_size  (sgl_window_info_t *win_info, int width, int height);

int sgl_initialize(sgl_window_info_t *win_info, int argc, char *argv[]);
int sgl_close(sgl_window_info_t *win_info);

#endif // SGL_INCLUDE_SGL_INCLUDE
