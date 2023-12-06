#include "grammar/driver/driver.h"

#include <cstdlib>

int main()
{
    yy::Driver driver(std::cin, std::cout);

    if (!driver.Parse()) {
        std::cerr << "Parsing failed" << std::endl;
        return EXIT_FAILURE;
    }

    driver.CodeGen();
    driver.DumpIR(std::cout);

    return EXIT_SUCCESS;
}