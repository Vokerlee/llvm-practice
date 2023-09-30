#include "sgl_rand.h"

void init_func(unsigned char *pixels_old, unsigned char *pixels_new, int width, int height)
{
    int seed = -21731286;
    sgl_srand(seed);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 4;

            pixels_new[index]     = 128;
            pixels_new[index + 1] = 0;
            pixels_new[index + 2] = 128;
            pixels_new[index + 3] = 255 * (sgl_rand() % 2);
        }
    }
}

void draw_func(unsigned char *pixels_old, unsigned char *pixels_new, int width, int height)
{
    static int n_generations = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = (i * width + j) * 4;
            int n_neighbors = 0;

            for (int y = -1; y <= 1; y++)
            {
                for (int x = -1; x <= 1; x++)
                {
                    if (x == 0 && y == 0)
                        continue;

                    int nindex = (((i + y + height) % height) * width + ((j + x + width) % width)) * 4;

                    if (nindex >= 0 && nindex < (width * height * 4 - 1))
                        if (pixels_old[nindex + 3] != 0)
                            n_neighbors += 1;
                }
            }

            if (pixels_old[index + 3] == 255 && (n_neighbors < 2 || n_neighbors > 3))
                pixels_new[index + 3] = 0;
            else if (pixels_old[index + 3] == 0 && n_neighbors == 3)
                pixels_new[index + 3] = 255;
            else
                pixels_new[index + 3] = pixels_old[index + 3];
        }
    }
}