#ifndef GRASM_PARSER_H
#define GRASM_PARSER_H

#include "common/macros.h"

#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class Parser
{
public:
    NO_COPY_SEMANTIC(Parser);
    NO_MOVE_SEMANTIC(Parser);

    Parser(const char *filename)
    {
        asm_file_.open(filename);
        if (!asm_file_.is_open())
        {
            // llvm::errs << "Error: can't open \"" << filename << "\"\n";
            llvm::errs() << "Error: can't open \"\n";
        }
    }

    ~Parser()
    {
        asm_file_.close();
    }

    void ResetToStart()
    {
        asm_file_.clear();
        asm_file_.seekg(0);
    }

    template <typename T>
    Parser &GetNext(T &field)
    {
        asm_file_ >> field;
        return *this;
    }

    bool IsValid() const
    {
        return asm_file_.is_open();
    }

private:
    std::ifstream asm_file_;
};

}; // grasm

#endif // GRASM_PARSER_H
