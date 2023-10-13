#include "sgl/sgl.h"

#include <stdlib.h>
#include <stdio.h>

extern void gameoflife_start();

int main(int argc, char *argv[])
{
    sgl_initialize();
    gameoflife_start();
    sgl_close();

    return EXIT_SUCCESS;
}
