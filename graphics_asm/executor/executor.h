#ifndef GRASM_PARSER_H
#define GRASM_PARSER_H

#include "common/macros.h"
#include "CPU/CPU.h"

#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class Executor
{
public:
    NO_COPY_SEMANTIC(Executor);
    NO_MOVE_SEMANTIC(Executor);

    Executor() = default;

    ~Executor() = default;

private:

};

}; // grasm

#endif // GRASM_PARSER_H
