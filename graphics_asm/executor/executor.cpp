#include "executor/executor.h"
#include "lexer/lexer.h"
#include "sgl/sgl.h"

#include <stack>

namespace grasm
{

bool Executor::ParseAsmFile(const char *filename)
{
    Lexer lexer(filename);
    if (!lexer.IsValid())
        return false;

    std::string arg1;
    std::string arg2;
    std::string arg3;
    std::string arg4;

    std::stack<std::string> last_func_names;
    last_func_names.push("main");

    while (lexer.IsValid() == true)
    {
        Instruction::Attrs attrs {};
        std::string mnemonic;

        lexer.GetNext(mnemonic);
        if (!mnemonic.compare(""))
            break;

        // That is just a label or func
        if (!mnemonic.substr(mnemonic.size() - 1, 1).compare(":"))
        {
            if (!mnemonic.substr(mnemonic.size() - 2, 1).compare(":"))
            {
                std::string func_name = mnemonic.substr(0, mnemonic.size() - 2);
                llvm::outs() << "func " << func_name << "\n";

                llvm::FunctionType *void_type = llvm::FunctionType::get(builder_.getVoidTy(), false);
                func_map_[func_name] =
                    llvm::Function::Create(void_type, llvm::Function::ExternalLinkage, func_name, module_);

                bb_map_["__" + func_name] = llvm::BasicBlock::Create(context_, "__" + func_name, func_map_[func_name]);

                attrs.label = func_name;
                instrs_.push_back(Instruction(InstructionId::FUNC, attrs, nullptr, func_name));
                last_func_names.push(func_name);

                continue;
            }

            std::string label_name = mnemonic.substr(0, mnemonic.size() - 1);
            std::string last_func = last_func_names.top();
            llvm::outs() << "label " << last_func + "_" + label_name << "\n";

            bb_map_[last_func + "_" + label_name] = llvm::BasicBlock::Create(context_, last_func + "_" + label_name, func_map_[last_func]);

            attrs.label = last_func + "_" + label_name;
            instrs_.push_back(Instruction(InstructionId::LABEL, attrs, nullptr, last_func + "_" + label_name));

            continue;
        }
        else if (!mnemonic.substr(0, 2).compare("::"))
        {
            std::string func_name = mnemonic.substr(2, mnemonic.size() - 2);
            llvm::outs() << "func_end " << func_name << "\n";

            attrs.label = func_name;
            instrs_.push_back(Instruction(InstructionId::FUNC_END, attrs, nullptr, func_name));
            last_func_names.pop();
            continue;
        }

        llvm::outs() << mnemonic << "\n";

        if (!mnemonic.compare("exit"))
            instrs_.push_back(Instruction(InstructionId::EXIT, attrs, exec::exit, "exit"));
        else if (!mnemonic.compare("ret"))
            instrs_.push_back(Instruction(InstructionId::RET, attrs, exec::ret, "ret"));
        else if (!mnemonic.compare("addi") || !mnemonic.compare("subi") || !mnemonic.compare("muli") ||
                 !mnemonic.compare("divi") || !mnemonic.compare("remi") || !mnemonic.compare("andi") ||
                 !mnemonic.compare("xori") || !mnemonic.compare("ilt")  || !mnemonic.compare("imet") ||
                 !mnemonic.compare("eq")   || !mnemonic.compare("neq")  || !mnemonic.compare("gr")   ||
                 !mnemonic.compare("gpr"))
        {
            lexer.GetNext(arg1).GetNext(arg2).GetNext(arg3);
            attrs.rd  = stoi(arg1.substr(1));
            attrs.rs1 = stoi(arg2.substr(1));
            attrs.rs2 = stoi(arg3.substr(1));

            if (!mnemonic.compare("addi"))
                instrs_.push_back(Instruction(InstructionId::ADDI, attrs, exec::addi, "addi"));
            else if (!mnemonic.compare("subi"))
                instrs_.push_back(Instruction(InstructionId::SUBI, attrs, exec::subi, "subi"));
            else if (!mnemonic.compare("muli"))
                instrs_.push_back(Instruction(InstructionId::MULI, attrs, exec::muli, "muli"));
            else if (!mnemonic.compare("divi"))
                instrs_.push_back(Instruction(InstructionId::DIVI, attrs, exec::divi, "divi"));
            else if (!mnemonic.compare("remi"))
                instrs_.push_back(Instruction(InstructionId::REMI, attrs, exec::remi, "remi"));
            else if (!mnemonic.compare("andi"))
                instrs_.push_back(Instruction(InstructionId::ANDI, attrs, exec::andi, "andi"));
            else if (!mnemonic.compare("xori"))
                instrs_.push_back(Instruction(InstructionId::XORI, attrs, exec::xori, "xori"));
            else if (!mnemonic.compare("ilt"))
                instrs_.push_back(Instruction(InstructionId::ILT,  attrs, exec::ilt,  "ilt"));
            else if (!mnemonic.compare("imet"))
                instrs_.push_back(Instruction(InstructionId::IMET, attrs, exec::imet, "imet"));
            else if (!mnemonic.compare("eq"))
                instrs_.push_back(Instruction(InstructionId::EQ,   attrs, exec::eq,   "eq"));
            else if (!mnemonic.compare("neq"))
                instrs_.push_back(Instruction(InstructionId::NEQ,  attrs, exec::neq,  "neq"));
            else if (!mnemonic.compare("gr"))
                instrs_.push_back(Instruction(InstructionId::GR,   attrs, exec::gr,   "gr"));
            else if (!mnemonic.compare("gpr"))
                instrs_.push_back(Instruction(InstructionId::GPR,  attrs, exec::gpr,  "gpr"));
        }
        else if (!mnemonic.compare("select") || !mnemonic.compare("gp") || !mnemonic.compare("rgb"))
        {
            lexer.GetNext(arg1).GetNext(arg2).GetNext(arg3).GetNext(arg4);
            attrs.rd  = stoi(arg1.substr(1));
            attrs.rs1 = stoi(arg2.substr(1));
            attrs.rs2 = stoi(arg3.substr(1));
            attrs.rs3 = stoi(arg4.substr(1));

            if (!mnemonic.compare("select"))
                instrs_.push_back(Instruction(InstructionId::SELECT, attrs, exec::select, "select"));
            else if (!mnemonic.compare("gp"))
                instrs_.push_back(Instruction(InstructionId::GP,     attrs, exec::gp,     "gp"));
            else if (!mnemonic.compare("rgb"))
                instrs_.push_back(Instruction(InstructionId::RGB,    attrs, exec::rgb,    "rgb"));
        }
        else if (!mnemonic.compare("brif"))
        {
            std::string last_func = last_func_names.top();
            lexer.GetNext(arg1).GetNext(arg2).GetNext(arg3);
            attrs.rs1 = stoi(arg1.substr(1));
            attrs.label     = last_func + "_" + arg2;
            attrs.label_alt = last_func + "_" + arg3;

            if (!mnemonic.compare("brif"))
                instrs_.push_back(Instruction(InstructionId::BRIF, attrs, exec::brif, "brif"));
        }
        else if (!mnemonic.compare("mov")  || !mnemonic.compare("red") || !mnemonic.compare("green") ||
                 !mnemonic.compare("blue") || !mnemonic.compare("lwi") || !mnemonic.compare("ldi")   ||
                 !mnemonic.compare("swi")  || !mnemonic.compare("sdi"))
        {
            lexer.GetNext(arg1).GetNext(arg2);
            attrs.rd  = stoi(arg1.substr(1));
            attrs.rs1 = stoi(arg2.substr(1));

            if (!mnemonic.compare("mov"))
                instrs_.push_back(Instruction(InstructionId::MOV,   attrs, exec::mov,   "mov"));
            else if (!mnemonic.compare("red"))
                instrs_.push_back(Instruction(InstructionId::RED,   attrs, exec::red,   "red"));
            else if (!mnemonic.compare("green"))
                instrs_.push_back(Instruction(InstructionId::GREEN, attrs, exec::green, "green"));
            else if (!mnemonic.compare("blue"))
                instrs_.push_back(Instruction(InstructionId::BLUE,  attrs, exec::blue,  "blue"));
            else if (!mnemonic.compare("lwi"))
                instrs_.push_back(Instruction(InstructionId::LWI,   attrs, exec::lwi,   "lwi"));
            else if (!mnemonic.compare("ldi"))
                instrs_.push_back(Instruction(InstructionId::LDI,   attrs, exec::ldi,   "ldi"));
            else if (!mnemonic.compare("swi"))
                instrs_.push_back(Instruction(InstructionId::SWI,   attrs, exec::swi,   "swi"));
            else if (!mnemonic.compare("sdi"))
                instrs_.push_back(Instruction(InstructionId::SDI,   attrs, exec::sdi,   "sdi"));
        }
        else if (!mnemonic.compare("mov_imm")   || !mnemonic.compare("red_imm") ||
                 !mnemonic.compare("green_imm") || !mnemonic.compare("blue_imm"))
        {
            lexer.GetNext(arg1).GetNext(arg2);
            attrs.rd  = stoi(arg1.substr(1));
            attrs.imm = stoi(arg2);

            if (!mnemonic.compare("mov_imm"))
                instrs_.push_back(Instruction(InstructionId::MOV_IMM,   attrs, exec::mov_imm,   "mov_imm"));
            else if (!mnemonic.compare("red_imm"))
                instrs_.push_back(Instruction(InstructionId::RED_IMM,   attrs, exec::red_imm,   "red_imm"));
            else if (!mnemonic.compare("green_imm"))
                instrs_.push_back(Instruction(InstructionId::GREEN_IMM, attrs, exec::green_imm, "green_imm"));
            else if (!mnemonic.compare("blue_imm"))
                instrs_.push_back(Instruction(InstructionId::BLUE_IMM,  attrs, exec::blue_imm,  "blue_imm"));
        }
        else if (!mnemonic.compare("rgb_imm"))
        {
            lexer.GetNext(arg1).GetNext(arg2).GetNext(arg3).GetNext(arg4);
            attrs.rd   = stoi(arg1.substr(1));
            attrs.imm  = stoi(arg2);
            attrs.imm2 = stoi(arg3);
            attrs.imm3 = stoi(arg4);

            if (!mnemonic.compare("rgb_imm"))
                instrs_.push_back(Instruction(InstructionId::RGB_IMM, attrs, exec::rgb_imm, "rgb_imm"));
        }
        else if (!mnemonic.compare("srand") || !mnemonic.compare("flush") || !mnemonic.compare("push"))
        {
            lexer.GetNext(arg1);
            attrs.rs1 = stoi(arg1.substr(1));

            if (!mnemonic.compare("srand"))
                instrs_.push_back(Instruction(InstructionId::SRAND, attrs, exec::srand, "srand"));
            else if (!mnemonic.compare("flush"))
                instrs_.push_back(Instruction(InstructionId::FLUSH, attrs, exec::flush, "flush"));
            else if (!mnemonic.compare("push"))
                instrs_.push_back(Instruction(InstructionId::PUSH,  attrs, exec::push,  "push"));
        }
        else if (!mnemonic.compare("br") || !mnemonic.compare("call"))
        {
            lexer.GetNext(arg1);

            if (!mnemonic.compare("br"))
            {
                std::string last_func = last_func_names.top();
                attrs.label = last_func + "_" + arg1;
                instrs_.push_back(Instruction(InstructionId::BR,   attrs, exec::br,   "br"));

            }
            else if (!mnemonic.compare("call"))
            {
                attrs.label = arg1;
                instrs_.push_back(Instruction(InstructionId::CALL, attrs, exec::call, "call"));
            }
        }
        else if (!mnemonic.compare("cf") || !mnemonic.compare("rand") || !mnemonic.compare("pop"))
        {
            lexer.GetNext(arg1);
            attrs.rd = stoi(arg1.substr(1));

            if (!mnemonic.compare("cf"))
                instrs_.push_back(Instruction(InstructionId::CF,   attrs, exec::cf,   "cf"));
            else if (!mnemonic.compare("rand"))
                instrs_.push_back(Instruction(InstructionId::RAND, attrs, exec::rand, "rand"));
            else if (!mnemonic.compare("pop"))
                instrs_.push_back(Instruction(InstructionId::POP,  attrs, exec::pop,  "pop"));
        }
    }

    return true;
}

bool Executor::Execute()
{
    std::string error_msg;
    llvm::raw_string_ostream error_ostream(error_msg);

    llvm::FunctionType *instr_call_type = llvm::FunctionType::get(
        builder_.getVoidTy(),
        llvm::ArrayRef<llvm::Type *>({builder_.getInt8PtrTy(), builder_.getInt8PtrTy()}),
        false);

    llvm::Value *cpu_ptr = builder_.getInt64((uint64_t) cpu_);

    if (instrs_[0].GetId() == InstructionId::LABEL)
        builder_.SetInsertPoint(bb_map_[instrs_[0].GetAttrs().label]);
    else
        builder_.SetInsertPoint(bb_map_["__start"]);

    std::stack<llvm::BasicBlock *> func_bb_stack;
    std::stack<std::string> func_names_stack;
    llvm::BasicBlock *bb_cur = builder_.GetInsertBlock();

    for (auto &instr : instrs_)
    {
        module_->print(llvm::outs(), nullptr);
        llvm::outs() << "================================================\n";

        InstructionId instr_id = instr.GetId();

        switch (instr_id)
        {
            case InstructionId::LABEL:
            {
                llvm::BasicBlock *bb = bb_map_[instr.GetAttrs().label];
                bb_cur = bb;
                builder_.SetInsertPoint(bb);
                break;
            }
            case InstructionId::FUNC:
            {
                func_bb_stack.push(bb_cur);
                llvm::BasicBlock *bb = bb_map_["__" + instr.GetAttrs().label];
                bb_cur = bb;
                builder_.SetInsertPoint(bb);
                break;
            }
            case InstructionId::FUNC_END:
            {
                bb_cur = func_bb_stack.top();
                func_bb_stack.pop();
                builder_.SetInsertPoint(bb_cur);
                break;
            }
            case InstructionId::CALL:
            {
                llvm::Function *func = func_map_[instr.GetAttrs().label];
                builder_.CreateCall(func);
                break;
            }
            case InstructionId::RET:
            {
                builder_.CreateRetVoid();
                break;
            }
            case InstructionId::EXIT:
            {
                builder_.CreateRetVoid();
                break;
            }
            case InstructionId::BR:
            {
                builder_.CreateBr(bb_map_[instr.GetAttrs().label]);
                break;
            }
            case InstructionId::BRIF:
            {
                llvm::Value *val_ptr =
                builder_.CreateConstGEP2_64(reg_file_type_, reg_file_, 0, instr.GetAttrs().rs1);

                llvm::Value *cond = builder_.CreateICmpEQ(
                    builder_.CreateLoad(builder_.getInt64Ty(), val_ptr),
                    llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 0, true));

                builder_.CreateCondBr(cond, bb_map_[instr.GetAttrs().label_alt],
                                            bb_map_[instr.GetAttrs().label]);

                break;
            }
            default:
            {
                llvm::Value *instr_ptr = builder_.getInt64((uint64_t) &instr);
                builder_.CreateCall(module_->getOrInsertFunction(instr.GetMnemonic(), instr_call_type),
                    llvm::ArrayRef<llvm::Value *>({cpu_ptr, instr_ptr}));
            }
        }
    }

    llvm::outs() << "\n#[LLVM IR]:\n";
    module_->print(llvm::outs(), nullptr);

    llvm::verifyFunction(*main_func_);
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::ExecutionEngine *ee =
        llvm::EngineBuilder(std::unique_ptr<llvm::Module>(module_)).setErrorStr(&error_msg).create();
    if (ee == nullptr)
    {
        llvm::errs() << "error occured in llvm::EngineBuilder(...).create(): "
                     << error_msg << "\n";
        return false;
    }

    ee->InstallLazyFunctionCreator(Instruction::LazyFunctionCreator);
    ee->addGlobalMapping(reg_file_, (void *) cpu_->regs_);
    ee->finalizeObject();

    sgl_initialize();

    llvm::outs() << "\n#[Running code]\n";
    llvm::ArrayRef<llvm::GenericValue> noargs;
    ee->runFunction(main_func_, noargs);

    sgl_close();

    cpu_->DumpRegs();

    return true;
}

} // grasm
