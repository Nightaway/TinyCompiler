#ifndef __CGEN_H_
#define __CGEN_H_

typedef enum {
	ROOT,
	LEFT,
	RIGHT
}TreeType;

void codeGen(TreeNode *syntaxTree, const char *codefile);

#endif