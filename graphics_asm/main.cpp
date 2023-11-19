#include "executor/executor.h"
#include "CPU/CPU.h"

#include <cstdlib>
#include <llvm/Support/raw_ostream.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        llvm::errs() << "Error: invalid amount of arguments (2 required)\n";
        return EXIT_FAILURE;
    }

    grasm::CPU cpu;
    grasm::Executor exec(&cpu);
    bool status = exec.ParseAsmFile(argv[1]);
    if (status == false)
        return EXIT_FAILURE;

    status = exec.Execute();
    if (status == false)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
