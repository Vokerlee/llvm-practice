#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: invalid amount of arguments (2 required)" << std::endl;
        return EXIT_FAILURE;
    }

    (void) argv;

    return EXIT_SUCCESS;
}
