#include "stdlib/stdlib.h"

#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <time.h>

const size_t FRAME_TICKS = 3;

static SDL_Renderer *RENDERER = NULL;
static SDL_Window   *WINDOW   = NULL;
static SDL_Texture  *TEXTURE  = NULL;
static Uint32        N_TICKS  = 0;

extern "C" void __sgl_initialize_(int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    WINDOW = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (WINDOW == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return;
    }

    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
    if (RENDERER == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return;
    }

    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (TEXTURE == NULL) {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
    SDL_RenderClear(RENDERER);

    srand(time(NULL));
}

extern "C" void __sgl_close_()
{
    SDL_Event event;

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyTexture(TEXTURE);
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}

static void __sgl_flush_()
{
    SDL_RenderPresent(RENDERER);

    SDL_PumpEvents();
    assert(SDL_TRUE != SDL_HasEvent(SDL_QUIT) && "User-requested quit");

    Uint32 cur_ticks = SDL_GetTicks() - N_TICKS;
    if (cur_ticks < FRAME_TICKS) {
        SDL_Delay(FRAME_TICKS - cur_ticks);
    }
}

extern "C" void __sgl_update_(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT])
{
    SDL_UpdateTexture(TEXTURE, NULL, pixel_buffer, SGL_WIDTH_DEFAULT * sizeof(int));
    SDL_RenderCopy(RENDERER, TEXTURE, NULL, NULL);

    __sgl_flush_();
}

extern "C" int __sgl_rand_()
{
    return rand();
}

extern "C" void __sgl_srand_(int seed)
{
    srand(seed);
}
