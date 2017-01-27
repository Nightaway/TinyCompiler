#ifndef __PARSER_H_
#define __PARSER_H_

#include "scanner.h"

typedef enum {StmtK, ExpK} NodeKind;
typedef enum {IfK, RepeatK, AssignK, ReadK, WriteK} StmtKind;
typedef enum {OpK, ConstK, IdK} ExpKind;

/* ExpType is used for type checking */ 
typedef enum {Void, Integer, Boolean} ExpType;

#define MAXCHILDREN 3

typedef struct treeNode
{
	struct treeNode *child[MAXCHILDREN];
	struct treeNode *sibling;
	//int lineno;
	NodeKind nodekind;
	union { StmtKind stmt; ExpKind exp;} kind;
	union { Token token; 
			//int val; 
			//char *name;
	} attr;
	ExpType type; /* for type checking of exps */
}TreeNode;

TreeNode *parse(void);

#endif
