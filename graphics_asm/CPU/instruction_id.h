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
    ADDI    = 0x1,  // addi    rd rs1 rs2
    SUBI    = 0x2,  // subi    rd rs1 rs2
    MULI    = 0x3,  // muli    rd rs1 rs2
    DIVI    = 0x4,  // divi    rd rs1 rs2
    REMI    = 0x5,  // remi    rd rs1 rs2
    ANDI    = 0x6,  // andi    rd rs1 rs2
    XORI    = 0x7,  // xori    rd rs1 rs2
    MOV     = 0x8,  // mov     rd rs
    MOVIMM  = 0x9,  // movimm  rd number

    // Conditional operations
    ILT     = 0x10, // ilt     rd rs1 rs2
    IMET    = 0x11, // imet    rd rs1 rs2
    EQ      = 0x12, // eq      rd rs1 rs2
    NEQ     = 0x13, // neq     rd rs1 rs2
    SELECT  = 0x14, // select  rd rs1 rs2, rcond

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
    LWI     = 0x30, // lwi     rd rs
    LDI     = 0x31, // ldi     rd rs
    SWI     = 0x32, // swi     rd rs
    SDI     = 0x33, // sdi     rd rs

    // Branch operations
    BR      = 0x40, // br      rs
    BRIF    = 0x41, // brif    rcond rs1 rs2
    CALL    = 0x42,
};

}; // grasm

#endif // GRASM_INSTRUCTION_ID_H
