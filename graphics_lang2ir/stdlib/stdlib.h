#ifndef GRLANG_STDLIB_STDLIB_H
#define GRLANG_STDLIB_STDLIB_H

#include "common/config.h"

#include <cstdlib>
#include <cstddef>
#include <iostream>

static constexpr size_t SGL_WIDTH_DEFAULT  = 1080;
static constexpr size_t SGL_HEIGHT_DEFAULT = 720;

extern "C" void __print_(grlang::Int value);
extern "C" grlang::Int __scan_();

extern "C" void __sgl_update_(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT]);

extern "C" int  __sgl_rand_();
extern "C" void __sgl_srand_(int seed);

extern "C" void __sgl_initialize_();
extern "C" void __sgl_close_();

#endif // GRLANG_STDLIB_STDLIB_H
