#ifndef __CGEN_H_
#define __CGEN_H_

typedef enum {
	ROOT,
	LEFT,
	RIGHT
}TreeType;

char *codeGen(TreeNode *syntaxTree);

#endif