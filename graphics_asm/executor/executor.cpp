#include "executor/executor.h"
#include "lexer/lexer.h"
#include "sgl/sgl.h"

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

    while (lexer.IsValid() == true)
    {
        Instruction::Attrs attrs {};
        std::string mnemonic;

        lexer.GetNext(mnemonic);
        if (!mnemonic.compare(""))
            break;

        // That is just a label
        if (!mnemonic.substr(mnemonic.size() - 1, mnemonic.size() - 1).compare(":"))
        {
            std::string label_name = mnemonic.substr(0, mnemonic.size() - 1);
            llvm::outs() << "label " << label_name << "\n";

            bb_map_[label_name] = llvm::BasicBlock::Create(context_, label_name, main_func_);

            attrs.label = label_name;
            instrs_.push_back(Instruction(InstructionId::LABEL, attrs, nullptr, label_name));

            continue;
        }

        llvm::outs() << mnemonic << "\n";

        if (!mnemonic.compare("exit"))
            instrs_.push_back(Instruction(InstructionId::EXIT, attrs, exec::exit, "exit"));
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
            lexer.GetNext(arg1).GetNext(arg2).GetNext(arg3);
            attrs.rs1 = stoi(arg1.substr(1));
            attrs.label     = arg2;
            attrs.label_alt = arg3;

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
        else if (!mnemonic.compare("movimm"))
        {
            lexer.GetNext(arg1).GetNext(arg2);
            attrs.rd  = stoi(arg1.substr(1));
            attrs.imm = stoi(arg2);

            if (!mnemonic.compare("movimm"))
                instrs_.push_back(Instruction(InstructionId::MOVIMM, attrs, exec::movimm, "movimm"));
        }
        else if (!mnemonic.compare("srand") || !mnemonic.compare("flush"))
        {
            lexer.GetNext(arg1);
            attrs.rs1 = stoi(arg1.substr(1));

            if (!mnemonic.compare("srand"))
                instrs_.push_back(Instruction(InstructionId::SRAND, attrs, exec::srand, "srand"));
            else if (!mnemonic.compare("flush"))
                instrs_.push_back(Instruction(InstructionId::FLUSH, attrs, exec::flush, "flush"));
        }
        else if (!mnemonic.compare("br"))
        {
            lexer.GetNext(arg1);
            attrs.label = arg1;

            if (!mnemonic.compare("br"))
                instrs_.push_back(Instruction(InstructionId::BR, attrs, exec::br, "br"));
        }
        else if (!mnemonic.compare("cf") || !mnemonic.compare("rand"))
        {
            lexer.GetNext(arg1);
            attrs.rd = stoi(arg1.substr(1));

            if (!mnemonic.compare("cf"))
                instrs_.push_back(Instruction(InstructionId::CF,   attrs, exec::cf,   "cf"));
            else if (!mnemonic.compare("rand"))
                instrs_.push_back(Instruction(InstructionId::RAND, attrs, exec::rand, "rand"));
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
    {
        bb_map_["__start"] = llvm::BasicBlock::Create(context_, "__start", main_func_);
        builder_.SetInsertPoint(bb_map_["__start"]);
    }

    for (auto &instr : instrs_)
    {
        if (instr.GetId() == InstructionId::LABEL)
        {
            builder_.SetInsertPoint(bb_map_[instr.GetAttrs().label]);
            continue;
        }
        else if (instr.GetId() == InstructionId::EXIT)
        {
            builder_.CreateRetVoid();
            continue;
        }
        else if (instr.GetId() == InstructionId::BR)
        {
            builder_.CreateBr(bb_map_[instr.GetAttrs().label]);
            continue;
        }
        else if (instr.GetId() == InstructionId::BRIF)
        {
            llvm::Value *val_ptr =
                builder_.CreateConstGEP2_64(reg_file_type_, reg_file_, 0, instr.GetAttrs().rs1);

            llvm::Value *cond = builder_.CreateICmpEQ(
                builder_.CreateLoad(builder_.getInt64Ty(), val_ptr),
                llvm::ConstantInt::get(llvm::Type::getInt64Ty(context_), 0, true));

            builder_.CreateCondBr(cond, bb_map_[instr.GetAttrs().label_alt],
                                        bb_map_[instr.GetAttrs().label]);

            continue;
        }

        llvm::Value *instr_ptr = builder_.getInt64((uint64_t) &instr);
        builder_.CreateCall(module_->getOrInsertFunction(instr.GetMnemonic(), instr_call_type),
            llvm::ArrayRef<llvm::Value *>({cpu_ptr, instr_ptr}));
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

    llvm::outs() << "\n#[Running code]\n";
    llvm::ArrayRef<llvm::GenericValue> noargs;
    ee->runFunction(main_func_, noargs);

    cpu_->DumpRegs();

    return true;
}

} // grasm
