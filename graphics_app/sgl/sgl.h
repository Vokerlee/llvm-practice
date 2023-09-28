#ifndef SGL_INCLUDE_SGL_INCLUDE
#define SGL_INCLUDE_SGL_INCLUDE

#define SGL_WIDTH_DEFAULT  1080
#define SGL_HEIGHT_DEFAULT 720

struct sgl_window_info;
typedef struct sgl_window_info sgl_window_info_t;

sgl_window_info_t *sgl_get_window_info();

int sgl_set_draw_func    (sgl_window_info_t *win_info, void (*draw_func)(unsigned char *, int, int));
int sgl_set_window_width (sgl_window_info_t *win_info, int width);
int sgl_set_window_height(sgl_window_info_t *win_info, int height);
int sgl_set_window_size  (sgl_window_info_t *win_info, int width, int height);

int sgl_initialize(sgl_window_info_t *win_info, int argc, char *argv[]);
int sgl_close(sgl_window_info_t *win_info);

#endif // SGL_INCLUDE_SGL_INCLUDE
