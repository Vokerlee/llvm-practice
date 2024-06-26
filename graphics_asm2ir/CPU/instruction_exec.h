#ifndef GRASM_INSTRUCTION_EXEC_H
#define GRASM_INSTRUCTION_EXEC_H

#include "common/macros.h"
#include "common/config.h"
#include "CPU/instruction.h"

namespace grasm
{

class CPU;

namespace exec
{
// General operations
void exit       (CPU *cpu, const Instruction &instr);

// Register-arithmetic operations
void addi       (CPU *cpu, const Instruction &instr);
void subi       (CPU *cpu, const Instruction &instr);
void muli       (CPU *cpu, const Instruction &instr);
void divi       (CPU *cpu, const Instruction &instr);
void remi       (CPU *cpu, const Instruction &instr);
void andi       (CPU *cpu, const Instruction &instr);
void xori       (CPU *cpu, const Instruction &instr);
void mov        (CPU *cpu, const Instruction &instr);
void mov_imm    (CPU *cpu, const Instruction &instr);

// Conditional operations
void ilt       (CPU *cpu, const Instruction &instr);
void imet      (CPU *cpu, const Instruction &instr);
void eq        (CPU *cpu, const Instruction &instr);
void neq       (CPU *cpu, const Instruction &instr);
void select    (CPU *cpu, const Instruction &instr);

// Graphics-related operations
void srand     (CPU *cpu, const Instruction &instr);
void rand      (CPU *cpu, const Instruction &instr);
void cf        (CPU *cpu, const Instruction &instr);
void gr        (CPU *cpu, const Instruction &instr);
void gpr       (CPU *cpu, const Instruction &instr);
void gp        (CPU *cpu, const Instruction &instr);
void flush     (CPU *cpu, const Instruction &instr);

void rgb       (CPU *cpu, const Instruction &instr);
void red       (CPU *cpu, const Instruction &instr);
void green     (CPU *cpu, const Instruction &instr);
void blue      (CPU *cpu, const Instruction &instr);

void rgb_imm   (CPU *cpu, const Instruction &instr);
void red_imm   (CPU *cpu, const Instruction &instr);
void green_imm (CPU *cpu, const Instruction &instr);
void blue_imm  (CPU *cpu, const Instruction &instr);

// Memory operations
void lwi       (CPU *cpu, const Instruction &instr);
void ldi       (CPU *cpu, const Instruction &instr);
void swi       (CPU *cpu, const Instruction &instr);
void sdi       (CPU *cpu, const Instruction &instr);

// Branch operations
void br        (CPU *cpu, const Instruction &instr);
void brif      (CPU *cpu, const Instruction &instr);

void call      (CPU *cpu, const Instruction &instr);
void ret       (CPU *cpu, const Instruction &instr);

// Stack operations
void push      (CPU *cpu, const Instruction &instr);
void pop       (CPU *cpu, const Instruction &instr);

}; // exec

}; // grasm

#endif // GRASM_INSTRUCTION_EXEC_H
