#ifndef TINY_AST_H
#define TINY_AST_H

#include <string>

namespace tiny {
namespace internal {

  class AST {

  public:
    virtual ~AST();
  };
 
  class ExprAST : public AST {

  public:
    
  };

  class VariableExprAST : public ExprAST {
    std::string name_;
  public:
    VariableExprAST(std::string name) : name_(name) {}
  };

 class NumberExprAST : public ExprAST {
    int num_;
  public:
    NumberExprAST(int num) : num_(num) {}
  };

  class BinOPExprAST : public ExprAST {
    
  public:
    BinOPExprAST(int op, ExprAST *lhs, ExprAST *rhs) {}
  };

}}

#endif

