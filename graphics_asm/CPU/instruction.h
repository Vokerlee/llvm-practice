#ifndef GRASM_INSTRUCTION_H
#define GRASM_INSTRUCTION_H

#include "common/macros.h"
#include "common/config.h"
#include "CPU/instruction_id.h"
#include "CPU/CPU.h"

#include <string>

namespace grasm
{

class Instruction
{
public:
    typedef void (*InstrImpl)(CPU *cpu, const Instruction &instr);

    struct Attrs
    {
        Reg imm;
        Reg rs1;
        Reg rs2;
        Reg rs3;
        Reg rd;
    };

public:
    NO_COPY_SEMANTIC(Instruction);
    NO_MOVE_SEMANTIC(Instruction);

    Instruction(InstructionId id, Attrs attributes, InstrImpl impl, std::string mnemonic = "") :
        id_(id),
        attributes_(attributes),
        impl_(impl),
        mnemonic_(mnemonic)
    {}

    Attrs GetAttrs() const
    {
        return attributes_;
    }

    std::string GetMnemonic() const
    {
        return mnemonic_;
    }

    ~Instruction() = default;

private:
    InstructionId id_   {InstructionId::EXIT};
    Attrs         attributes_;
    InstrImpl     impl_ {nullptr};
    std::string   mnemonic_;

};

}; // grasm

#endif // GRASM_INSTRUCTION_H
