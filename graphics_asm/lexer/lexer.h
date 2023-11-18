#ifndef GRASM_LEXER_H
#define GRASM_LEXER_H

#include "common/macros.h"

#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grasm
{

class Lexer
{
public:
    NO_COPY_SEMANTIC(Lexer);
    NO_MOVE_SEMANTIC(Lexer);

    Lexer(const char *filename)
    {
        asm_file_.open(filename);
        if (!asm_file_.is_open())
        {
            llvm::errs() << "Error: can't open \"" << filename << "\"\n";
        }
    }

    ~Lexer()
    {
        asm_file_.close();
    }

    void ResetToStart()
    {
        asm_file_.clear();
        asm_file_.seekg(0);
    }

    template <typename T>
    Lexer &GetNext(T &field)
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

#endif // GRASM_LEXER_H
