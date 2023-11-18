#ifndef GRASM_CPU_H
#define GRASM_CPU_H

#include "common/macros.h"
#include "common/config.h"
#include "sgl/sgl_minimal.hpp"
#include "CPU/instruction.h"

#include <vector>
#include <stack>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class CPU
{
public:
    static constexpr size_t REG_FILE_SIZE = 0x20;

public:
    NO_COPY_SEMANTIC(CPU);
    NO_MOVE_SEMANTIC(CPU);

    CPU() = default;
    ~CPU() = default;

protected:
    typedef int FrameBuffer[SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT];

protected:
    Reg regs_[REG_FILE_SIZE] = {0};
    Reg pc_ {0};
    Reg pc_target_ {0};

    std::stack<Reg> call_stack_;
    std::vector<FrameBuffer> frames_;
};

}; // grasm

#endif // GRASM_CPU_H
