#ifndef TINY_PARSER_H
#define TINY_PARSER_H

#include "checks.h"
#include "tinystdint.h"
#include "ast.h"
#include "token.h"

namespace tiny {
namespace internal {

ExprAST *ParseTopLevelExpr(Token::Value);

}}


#endif 
