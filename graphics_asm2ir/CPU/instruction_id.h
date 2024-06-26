#ifndef GRASM_INSTRUCTION_ID_H
#define GRASM_INSTRUCTION_ID_H

#include "common/macros.h"
#include "common/config.h"

namespace grasm
{

enum class InstructionId
{
    // Pseudo instructions (helpers)
    LABEL     = 0xff,
    FUNC      = 0xfe,
    FUNC_END  = 0xfd,

    // General operations
    EXIT      = 0x0,  // exit

    // Register-arithmetic operations
    ADDI      = 0x1,  // addi    rd rs1 rs2
    SUBI      = 0x2,  // subi    rd rs1 rs2
    MULI      = 0x3,  // muli    rd rs1 rs2
    DIVI      = 0x4,  // divi    rd rs1 rs2
    REMI      = 0x5,  // remi    rd rs1 rs2
    ANDI      = 0x6,  // andi    rd rs1 rs2
    XORI      = 0x7,  // xori    rd rs1 rs2
    MOV       = 0x8,  // mov     rd rs
    MOV_IMM   = 0x9,  // mov_imm rd imm

    // Conditional operations
    ILT       = 0x10, // ilt     rd rs1 rs2
    IMET      = 0x11, // imet    rd rs1 rs2
    EQ        = 0x12, // eq      rd rs1 rs2
    NEQ       = 0x13, // neq     rd rs1 rs2
    SELECT    = 0x14, // select  rd rs1 rs2, rcond

    // Graphics-related operations
    SRAND     = 0x20, // srand   rs
    RAND      = 0x21, // rand    rd
    CF        = 0x22, // cf      rd
    GR        = 0x23, // gr      rd rs1 rs2
    GPR       = 0x24, // gpr     rd rs1 rs2
    GP        = 0x25, // gp      rd rs1 rs2 rs3
    FLUSH     = 0x26, // flush   rs

    RGB       = 0x27, // rgb     rd red green blue
    RED       = 0x28, // red     rd rs
    GREEN     = 0x29, // green   rd rs
    BLUE      = 0x2a, // blue    rd rs

    RGB_IMM   = 0x2b, // rgb     rd red_imm green_imm blue_imm
    RED_IMM   = 0x2c, // red     rd imm
    GREEN_IMM = 0x2d, // green   rd imm
    BLUE_IMM  = 0x2e, // blue    rd imm

    // Memory operations
    LWI       = 0x30, // lwi     rd rs
    LDI       = 0x31, // ldi     rd rs
    SWI       = 0x32, // swi     rd rs
    SDI       = 0x33, // sdi     rd rs

    // Branch operations
    BR        = 0x40, // br      label
    BRIF      = 0x41, // brif    rcond label1 label2

    CALL      = 0x42, // call    label
    RET       = 0x43, // ret

    // Stack operations
    PUSH      = 0x44, // push    rs
    POP       = 0x45, // pop     rd
};

}; // grasm

#endif // GRASM_INSTRUCTION_ID_H
