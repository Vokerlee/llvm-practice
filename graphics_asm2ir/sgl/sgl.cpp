#include "sgl/sgl.h"

#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <time.h>

const size_t FRAME_TICKS = 3;

static SDL_Renderer *RENDERER = NULL;
static SDL_Window *WINDOW = NULL;
static SDL_Texture *TEXTURE = NULL;
static Uint32 N_TICKS = 0;

int sgl_initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    WINDOW = SDL_CreateWindow("Game of life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SGL_WIDTH_DEFAULT, SGL_HEIGHT_DEFAULT, 0);
    if (WINDOW == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }

    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
    if (RENDERER == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    TEXTURE = SDL_CreateTexture(RENDERER, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SGL_WIDTH_DEFAULT, SGL_HEIGHT_DEFAULT);
    if (TEXTURE == NULL)
    {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0);
    SDL_RenderClear(RENDERER);

    srand(time(NULL));

    return 0;
}

void sgl_close()
{
    SDL_Event event;

    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyTexture(TEXTURE);
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}

void sgl_flush()
{
    SDL_RenderPresent(RENDERER);

    SDL_PumpEvents();
    assert(SDL_TRUE != SDL_HasEvent(SDL_QUIT) && "User-requested quit");

    Uint32 cur_ticks = SDL_GetTicks() - N_TICKS;
    if (cur_ticks < FRAME_TICKS)
    {
        SDL_Delay(FRAME_TICKS - cur_ticks);
    }
}

void sgl_update(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT])
{
    SDL_UpdateTexture(TEXTURE, NULL, pixel_buffer, SGL_WIDTH_DEFAULT * sizeof(int));
    SDL_RenderCopy(RENDERER, TEXTURE, NULL, NULL);

    sgl_flush();
}

int sgl_rand()
{
    return rand();
}

void sgl_srand(int seed)
{
    srand(seed);
}
