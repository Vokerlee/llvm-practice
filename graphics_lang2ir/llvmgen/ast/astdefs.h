#ifndef GRLANG_LLVMGEN_AST_ASTDEFS_H
#define GRLANG_LLVMGEN_AST_ASTDEFS_H

namespace grlang
{

namespace llvmgen
{

struct Node;
class DeclNode;
class StoreableNode;
class AssignNode;
class IntNode;
class BinOpNode;
class VarNode;
class GlobVarNode;
class ArrayAccessNode;
class FuncProtNode;
class FuncParamNode;
class FuncScopeNode;
class IfNode;
class WhileNode;

class PrintNode;
class ScanNode;
class SGLInitNode;
class SGLCloseNode;
class SGLRandNode;
class SGLSrandNode;
class SGLUpdateNode;

} // namespace llvmgen
} // namespace grlang

#endif // GRLANG_LLVMGEN_AST_ASTDEFS_H
