#ifndef GRASM_SGL_H
#define GRASM_SGL_H

#include <cstddef>

static constexpr size_t SGL_WIDTH_DEFAULT  = 1080;
static constexpr size_t SGL_HEIGHT_DEFAULT = 720;

void sgl_update(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT]);
void sgl_flush();

int  sgl_rand();
void sgl_srand(int seed);

int  sgl_initialize();
void sgl_close();

#endif // GRASM_SGL_H
