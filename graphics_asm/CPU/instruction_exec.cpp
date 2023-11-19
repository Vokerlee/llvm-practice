#include "CPU/instruction.h"
#include "CPU/instruction_exec.h"
#include "CPU/CPU.h"
#include "sgl/sgl.h"

namespace grasm::exec
{

void exit(CPU *cpu, const Instruction &instr)
{
    (void) instr;
    cpu->MakeIdle();
}

void addi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) + cpu->GetReg(attrs.rs2));
}

void subi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) - cpu->GetReg(attrs.rs2));
}

void muli(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) * cpu->GetReg(attrs.rs2));
}

void divi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) / cpu->GetReg(attrs.rs2));
}

void remi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) % cpu->GetReg(attrs.rs2));
}
void andi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) & cpu->GetReg(attrs.rs2));
}

void xori(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) ^ cpu->GetReg(attrs.rs2));
}

void mov(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1));
}

void movimm(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, attrs.imm);
}

void ilt(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) < cpu->GetReg(attrs.rs2));
}

void imet(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) >= cpu->GetReg(attrs.rs2));
}

void eq(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) == cpu->GetReg(attrs.rs2));
}

void neq(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1) != cpu->GetReg(attrs.rs2));
}

void select(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    if (cpu->GetReg(attrs.rs3))
        cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs1));
    else
        cpu->SetReg(attrs.rd, cpu->GetReg(attrs.rs2));
}

void srand(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    sgl_srand(cpu->GetReg(attrs.rs1));
}

void rand(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, sgl_rand());
}

void cf(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void gr(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void gpr(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void gp(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void flush(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void rgb(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void red(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void green(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void blue(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;
}

void lwi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, *((int *) cpu->GetReg(attrs.rs1)));
}

void ldi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    cpu->SetReg(attrs.rd, *((Reg *) cpu->GetReg(attrs.rs1)));
}

void swi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    *((int *) cpu->GetReg(attrs.rd)) = cpu->GetReg(attrs.rs1);
}

void sdi(CPU *cpu, const Instruction &instr)
{
    auto attrs = instr.GetAttrs();
    *((Reg *) cpu->GetReg(attrs.rd)) = cpu->GetReg(attrs.rs1);
}

void br(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;

    llvm::errs() << "Error: instruction \"br\" is not reachable: its implemenated is in LLVM IR generation\n";
}

void brif(CPU *cpu, const Instruction &instr)
{
    (void) cpu;
    (void) instr;

    llvm::errs() << "Error: instruction \"brif\" is not reachable: its implemenated is in LLVM IR generation\n";
}

} // grasm::exec

namespace grasm
{

void *Instruction::LazyFunctionCreator(const std::string &mnemonic)
{
    if (mnemonic == "exit")
        return reinterpret_cast<void *>(exec::exit);
    if (mnemonic == "addi")
        return reinterpret_cast<void *>(exec::addi);
    if (mnemonic == "subi")
        return reinterpret_cast<void *>(exec::subi);
    if (mnemonic == "divi")
        return reinterpret_cast<void *>(exec::divi);
    if (mnemonic == "remi")
        return reinterpret_cast<void *>(exec::remi);
    if (mnemonic == "andi")
        return reinterpret_cast<void *>(exec::andi);
    if (mnemonic == "xori")
        return reinterpret_cast<void *>(exec::xori);
    if (mnemonic == "mov")
        return reinterpret_cast<void *>(exec::mov);
    if (mnemonic == "movimm")
        return reinterpret_cast<void *>(exec::movimm);
    if (mnemonic == "ilt")
        return reinterpret_cast<void *>(exec::ilt);
    if (mnemonic == "imet")
        return reinterpret_cast<void *>(exec::imet);
    if (mnemonic == "eq")
        return reinterpret_cast<void *>(exec::eq);
    if (mnemonic == "neq")
        return reinterpret_cast<void *>(exec::neq);
    if (mnemonic == "select")
        return reinterpret_cast<void *>(exec::select);
    if (mnemonic == "srand")
        return reinterpret_cast<void *>(exec::srand);
    if (mnemonic == "rand")
        return reinterpret_cast<void *>(exec::rand);
    if (mnemonic == "cf")
        return reinterpret_cast<void *>(exec::cf);
    if (mnemonic == "gr")
        return reinterpret_cast<void *>(exec::gr);
    if (mnemonic == "gpr")
        return reinterpret_cast<void *>(exec::gpr);
    if (mnemonic == "gp")
        return reinterpret_cast<void *>(exec::gp);
    if (mnemonic == "flush")
        return reinterpret_cast<void *>(exec::flush);
    if (mnemonic == "rgb")
        return reinterpret_cast<void *>(exec::rgb);
    if (mnemonic == "red")
        return reinterpret_cast<void *>(exec::red);
    if (mnemonic == "green")
        return reinterpret_cast<void *>(exec::green);
    if (mnemonic == "blue")
        return reinterpret_cast<void *>(exec::blue);
    if (mnemonic == "lwi")
        return reinterpret_cast<void *>(exec::lwi);
    if (mnemonic == "ldi")
        return reinterpret_cast<void *>(exec::ldi);
    if (mnemonic == "swi")
        return reinterpret_cast<void *>(exec::swi);
    if (mnemonic == "sdi")
        return reinterpret_cast<void *>(exec::sdi);
    if (mnemonic == "br")
        return reinterpret_cast<void *>(exec::br);
    if (mnemonic == "brif")
        return reinterpret_cast<void *>(exec::brif);

    return nullptr;
}

} // grasm
