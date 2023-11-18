#ifndef GRASM_INSTRUCTION_ID_H
#define GRASM_INSTRUCTION_ID_H

#include "common/macros.h"
#include "common/config.h"

namespace grasm
{

enum class InstructionId
{
    // General operations
    EXIT    = 0x0,  // exit

    // Register-arithmetic operations
    ADD     = 0x1,  // add     rd rs1 rs2
    SUB     = 0x2,  // sub     rd rs1 rs2
    DIV     = 0x3,  // div     rd rs1 rs2
    REM     = 0x4,  // rem     rd rs1 rs2
    AND     = 0x5,  // and     rd rs1 rs2
    XOR     = 0x6,  // xor     rd rs1 rs2
    MOV     = 0x7,  // mov     rd rs
    MOVI    = 0x8,  // and     rd number

    // Conditional operations
    ILT     = 0x10, // ilt     rd rs1 rs2
    IMET    = 0x11, // imet    rd rs1 rs2
    EQ      = 0x12, // eq      rd rs1 rs2
    NEQ     = 0x13, // neq     rd rs1 rs2
    ISELECT = 0x14, // iselect rd rs1 rs2, rcond

    // Graphics-related operations
    SRAND   = 0x20, // srand   rs
    RAND    = 0x21, // rand    rd
    CF      = 0x22, // cf      rd
    GR      = 0x23, // gr      rd rs1 rs2
    GPR     = 0x24, // gpr     rd rs1 rs2
    GP      = 0x25, // gp      rd rs1 rs2 rs3
    FLUSH   = 0x26, // flush   rs

    RGB     = 0x27, // rgb     rd red green blue
    RED     = 0x28, // red     rd rs
    GREEN   = 0x29, // green   rd rs
    BLUE    = 0x2a, // blue    rd rs

    // Memory operations
    LW      = 0x30, // lw      rd rs
    LD      = 0x31, // ld      rd rs
    SW      = 0x32, // sw      rd rs
    SD      = 0x33, // sd      rd rs

    // Branch operations
    BR      = 0x40, // br      rs
    BRIF    = 0x41, // brif    rcond rs1 rs2
    CALL    = 0x42,
};

}; // grasm

#endif // GRASM_INSTRUCTION_ID_H
