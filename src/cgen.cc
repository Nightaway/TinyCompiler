#include "globals.h"
#include "parser.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

static char asm_code[1024];
static int asm_code_idx = 0;

static void emitConst(int val) {
	const char code[] = "loadc %d\n";
	int n = sprintf(asm_code + asm_code_idx, code, val);
	//printf("%s", asm_code + asm_code_idx);
	asm_code_idx += n;
}

static void emitId(char *id) {
	emitConst(st_lookup(id)*16);
	const char code[] = "load\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	//printf("%s", asm_code + asm_code_idx);
	asm_code_idx += n;
}

static void emitOp(int op) {
	int n = 0;
	const char *code;
	switch (op) {
			case ADD:
			code = "add\n";
			n = sprintf(asm_code + asm_code_idx, code);
			//printf("%s", asm_code + asm_code_idx);
			asm_code_idx += n;
			break;

			case MUL:
			code = "mul\n";
			n = sprintf(asm_code + asm_code_idx, code);
			asm_code_idx += n;
			break;
	}
}

static void emitAssign(TreeNode *tree) {
	const char code[] = "store\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	//printf("%s", asm_code + asm_code_idx);
	asm_code_idx += n;
}

static void emitWrite(TreeNode *tree) {
	emitId(tree->attr.token.stype);
	const char code[] = "call print\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	//printf("%s", asm_code + asm_code_idx);
	asm_code_idx += n;
}

static void cGen(TreeNode *tree);

static void genStmt(TreeNode *tree)
{
	TreeNode *p1, *p2, *p3;
	
	switch (tree->kind.stmt)
	{
	case IfK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];

		cGen(p1);

		break;

	case RepeatK:
		break;

	case AssignK:
		p1 = tree->child[0];
		emitConst(st_lookup(tree->attr.token.stype)*16);
		cGen(p1);
		emitAssign(tree);
	break;

	case ReadK:
		break;

	case WriteK:
		p1 = tree->child[0];
		emitWrite(p1);
		break;
	default : break;
	}
}

static void genExp(TreeNode *tree, TreeType ttype)
{
	TreeNode *t1, *t2;
	t1 = tree->child[0];
	t2 = tree->child[1];
	
	switch (tree->kind.exp)
	{
	case ConstK:
		emitConst(tree->attr.token.val);
		break;

	case IdK:
		emitId(tree->attr.token.stype);
		break;

	case OpK:
		genExp(t1, LEFT);
		genExp(t2, RIGHT);
		emitOp(tree->attr.token.ttype);

		break;
	}
}

static void cGen(TreeNode *tree)
{
	if (tree != NULL) {
		switch (tree->nodekind)
		{
		case StmtK:
			genStmt(tree);
			break;

		case ExpK:
			genExp(tree, ROOT);
			break;
		}
		cGen(tree->sibling);
	}
}


char *codeGen(TreeNode *syntaxTree)
{
	cGen(syntaxTree);
	return asm_code;
}

