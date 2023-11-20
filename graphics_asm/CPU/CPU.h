#ifndef GRASM_CPU_H
#define GRASM_CPU_H

#include "common/macros.h"
#include "common/config.h"
#include "sgl/sgl.h"

#include <cassert>
#include <vector>
#include <stack>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class CPU
{
public:
    friend class Executor;

    static constexpr size_t REG_FILE_SIZE = 0x20;

    typedef int FrameBuffer[SGL_WIDTH_DEFAULT];

public:
    NO_COPY_SEMANTIC(CPU);
    NO_MOVE_SEMANTIC(CPU);

    CPU() = default;
    ~CPU()
    {
        for (auto &frame_ptr : frames_)
        {
            delete [] frame_ptr;
        }
    }

    Reg GetReg(size_t reg_idx) const
    {
        assert(reg_idx < REG_FILE_SIZE);
        return regs_[reg_idx];
    }

    void SetReg(size_t reg_idx, Reg reg)
    {
        assert(reg_idx < REG_FILE_SIZE);
        regs_[reg_idx] = reg;
    }

    void PushReg(Reg reg)
    {
        stack_.push(reg);
    }

    Reg PopReg()
    {
        Reg value = stack_.top();
        stack_.pop();

        return value;
    }

    Reg GetPC() const
    {
        return pc_;
    }

    void SetPC(Reg pc)
    {
        pc_ = pc;
    }

    Reg GetPCTarget() const
    {
        return pc_target_;
    }

    void SetPCTarget(Reg pc_target)
    {
        pc_target_ = pc_target;
    }

    void MakeIdle()
    {
        is_idle = true;
    }

    void DumpRegs() const
    {
        llvm::outs() << "Registers' dump:\n";
        for (size_t i = 0; i < REG_FILE_SIZE; ++i)
            llvm::outs() << "[Reg #" << i << "]: " << regs_[i] << "\n";
    }

    FrameBuffer *CreateFrame()
    {
        FrameBuffer *frame_ptr = new int [SGL_HEIGHT_DEFAULT][SGL_WIDTH_DEFAULT];
        frames_.push_back(frame_ptr);
        return frame_ptr;
    }

protected:
    Reg regs_[REG_FILE_SIZE] = {0};
    Reg pc_ {0};
    Reg pc_target_ {0};

    bool is_idle {true};

    std::stack<Reg> stack_;
    std::vector<FrameBuffer *> frames_;
};

}; // grasm

#endif // GRASM_CPU_H
