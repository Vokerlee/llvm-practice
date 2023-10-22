#include "sgl/sgl_minimal.h"

static void gameoflife_init(int pixel_buffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT])
{
    int seed = -21731286;
    sgl_srand(seed);

    for (int y = 0; y < SGL_HEIGHT_DEFAULT; y++)
    {
        for (int x = 0; x < SGL_WIDTH_DEFAULT; x++)
            pixel_buffer[y][x] = 0xFF00AA33 * (sgl_rand() % 2);
    }

    sgl_update(pixel_buffer);
}

void gameoflife_start()
{
    int pixel_buffers[2][SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT];
    int buffer_index = 0;

    gameoflife_init(pixel_buffers[1 - buffer_index]);

    int n_steps = 100000;

    for (int step = 0; step < n_steps; ++step)
    {
        for (int i = 0; i < SGL_HEIGHT_DEFAULT; i++)
        {
            for (int j = 0; j < SGL_WIDTH_DEFAULT; j++)
            {
                int n_neighbors = 0;

                for (int y = -1; y <= 1; y++)
                {
                    for (int x = -1; x <= 1; x++)
                    {
                        if (x == 0 && y == 0)
                            continue;

                        int pixel_neighbour = pixel_buffers[1 - buffer_index][(i + y + SGL_HEIGHT_DEFAULT) % SGL_HEIGHT_DEFAULT][(j + x + SGL_WIDTH_DEFAULT) % SGL_WIDTH_DEFAULT];
                        if ((pixel_neighbour & 0xFF000000) != 0)
                            n_neighbors += 1;
                    }
                }

                int pixel = pixel_buffers[1 - buffer_index][i][j];

                if ((pixel & 0xFF000000) != 0 && (n_neighbors < 2 || n_neighbors > 3))
                    pixel_buffers[buffer_index][i][j] = 0x00000000;
                else if ((pixel & 0xFF000000) == 0 && n_neighbors == 3)
                    pixel_buffers[buffer_index][i][j] = 0xFF00AA33;
                else
                    pixel_buffers[buffer_index][i][j] = pixel_buffers[1 - buffer_index][i][j];
            }
        }

        sgl_update(pixel_buffers[buffer_index]);

        buffer_index = (buffer_index + 1) % 2;
    }
}
