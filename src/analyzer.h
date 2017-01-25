#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include "parser.h"
#include "symtab.h"

void buildSymtab(TreeNode *);

void typeCheck(TreeNode *);

#endif