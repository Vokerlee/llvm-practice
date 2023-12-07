#ifndef GRLANG_LLVMGEN_AST_CONTROL_FLOW_H
#define GRLANG_LLVMGEN_AST_CONTROL_FLOW_H

#include "llvmgen/ast/base.h"
#include "llvmgen/ast/func.h"

namespace grlang
{

namespace llvmgen
{

class IfNode : public Node
{
public:
    IfNode(FuncScopePtr parent_scope, NodePtr cond, FuncScopePtr scope_true, FuncScopePtr scope_false = nullptr) :
        parent_scope_(parent_scope), condition_(cond), scope_true_(scope_true), scope_false_(scope_false)
    {}

    llvm::Value *LLVMGen(Context &ctx) override;

private:
    std::weak_ptr<FuncScopeNode> parent_scope_;

    NodePtr condition_;
    FuncScopePtr scope_true_;
    FuncScopePtr scope_false_;
};

class WhileNode : public Node
{
public:
    WhileNode(FuncScopePtr parent_scope, NodePtr cond, FuncScopePtr body) :
        parent_scope_(parent_scope), condition_(cond), body_(body)
    {}

    llvm::Value *LLVMGen(Context &ctx) override;

private:
    std::weak_ptr<FuncScopeNode> parent_scope_;

    NodePtr condition_;
    FuncScopePtr body_;
};

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_CONTROL_FLOW_H
