#include "CPU/instruction.h"
#include "CPU/instruction_exec.h"
#include "CPU/CPU.h"

namespace grasm::exec
{

void exit(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

} // grasm::exec
