#ifndef GRASM_INSTRUCTION_H
#define GRASM_INSTRUCTION_H

#include "common/macros.h"
#include "common/config.h"

namespace grasm
{

enum class InsnId
{
    // General operations
    EXIT    = 0x0,  // exit

    // Register-arithmetic operations
    ADD     = 0x1,  // add     dst src1 src2
    SUB     = 0x2,  // sub     dst src1 src2
    DIV     = 0x3,  // div     dst src1 src2
    REM     = 0x4,  // rem     dst src1 src2
    AND     = 0x5,  // and     dst src1 src2
    XOR     = 0x6,  // xor     dst src1 src2
    MOV     = 0x7,  // mov     dst src
    MOVI    = 0x8,  // and     dst number

    // Conditional operations
    ILT     = 0x10, // ilt     dst src1 src2
    IMET    = 0x11, // imet    dst src1 src2
    EQ      = 0x12, // eq      dst src1 src2
    NEQ     = 0x13, // neq     dst src1 src2
    ISELECT = 0x14, // iselect dst src1 src2, cond

    // Graphics-related operations
    SRAND   = 0x20, // srand   src
    RAND    = 0x21, // rand    dst
    CF      = 0x22, // cf      dst
    GR      = 0x23, // gr      dst src
    GPR     = 0x24, // gpr     dst src1 src2
    GP      = 0x25, // gp      dst src1 src2
    FLUSH   = 0x26, // flush   src

    RGB     = 0x27, // rgb     dst red green blue
    RED     = 0x28, // red     dst src
    GREEN   = 0x29, // green   dst src
    BLUE    = 0x2a, // blue    dst src

    // Memory operations
    LW      = 0x30, // lw      dst src
    LD      = 0x31, // ld      dst src
    SW      = 0x32, // sw      dst src
    SD      = 0x33, // sd      dst src

    // Branch operations
    BR      = 0x40, // br      src
    BRIF    = 0x41, // brif    cond src1 src2
};

}; // grasm

#endif // GRASM_INSTRUCTION_H
