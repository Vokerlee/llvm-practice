#ifndef SGL_MINIMAL_INCLUDE_SGL_MINIMAL_INCLUDE
#define SGL_MINIMAL_INCLUDE_SGL_MINIMAL_INCLUDE

#define SGL_WIDTH_DEFAULT  1080
#define SGL_HEIGHT_DEFAULT 720

int  sgl_get_pixel(int x, int y);
void sgl_put_pixel(int x, int y, int argb);

void sgl_update(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT]);
void sgl_flush();

int  sgl_rand();
void sgl_srand(int seed);

#endif // SGL_MINIMAL_INCLUDE_SGL_MINIMAL_INCLUDE
