#include "stdlib/stdlib.h"

#include <cstdlib>
#include <iostream>

extern "C" void __print_(grlang::Int value)
{
    std::cout << value << std::endl;
}

extern "C" grlang::Int __scan_()
{
    grlang::Int res;

    std::cin >> res;
    if (!std::cin) {
        std::cerr << "Problem while reading stdin in scan(). Exiting with failure...\n";
        std::exit(EXIT_FAILURE);
    }

    return res;
}
