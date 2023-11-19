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
        std::string label;

        Reg imm {0};
        Reg rs1 {0};
        Reg rs2 {0};
        Reg rs3 {0};
        Reg rd  {0};
    };

public:
    Instruction(InstructionId id, Attrs attributes, InstrImpl impl, std::string mnemonic = "") :
        id_(id),
        attributes_(attributes),
        impl_(impl),
        mnemonic_(mnemonic)
    {}

    ~Instruction() = default;

    const Attrs &GetAttrs() const
    {
        return attributes_;
    }

    const std::string &GetMnemonic() const
    {
        return mnemonic_;
    }

    InstructionId GetId() const
    {
        return id_;
    }

    void Execute(CPU *cpu)
    {
        impl_(cpu, *this);
    }

    static void *LazyFunctionCreator(const std::string &mnemonic);

private:
    InstructionId id_ {InstructionId::EXIT};
    Attrs         attributes_;
    InstrImpl     impl_ {nullptr};
    std::string   mnemonic_;
};

}; // grasm

#endif // GRASM_INSTRUCTION_H
