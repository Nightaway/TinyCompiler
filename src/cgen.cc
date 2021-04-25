#include "globals.h"
#include "parser.h"
#include "symtab.h"
#include "cgen.h"

static char asm_code[1024];
static int asm_code_idx = 0;
static int asm_line_num = 0;
static int label_num = 0;

static void emitConst(int val) {
	const char code[] = "loadc %d\n";
	int n = sprintf(asm_code + asm_code_idx, code, val);
	asm_code_idx += n;
	asm_line_num++;
}

static void emitId(char *id) {
	emitConst(st_lookup(id)*16);
	const char code[] = "load\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	asm_code_idx += n;
	asm_line_num++;
}

static void emitJumpz(const char *label) {
	const char code[] = "jumpz %s\n";
	int n = sprintf(asm_code + asm_code_idx, code, label);
	asm_code_idx += n;
	asm_line_num++;
}

static void emitLabel(const char *label) {
	const char code[] = "%s: ";
	int n = sprintf(asm_code + asm_code_idx, code, label);
	asm_code_idx += n;
}

static void emitOp(int op) {
	int n = 0;
	const char *code;
	switch (op) {
			case ADD:
			code = "add\n";
			break;

			case MUL:
			code = "mul\n";
			break;

			case SUB:
			code = "sub\n";
			break;

			case LESS:
			code = "less\n";
			break;

			case EQUAL:
			code = "equal\n";
			break;
	}
	n = sprintf(asm_code + asm_code_idx, code);
	asm_code_idx += n;
	asm_line_num++;
}

static void emitAssign(TreeNode *tree) {
	const char code[] = "store\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	asm_code_idx += n;
	asm_line_num++;
}

static void emitWrite(TreeNode *tree) {
	emitId(tree->attr.token.stype);
	const char code[] = "call print\n";
	int n = sprintf(asm_code + asm_code_idx, code);
	asm_code_idx += n;
	asm_line_num++;
}

static void cGen(TreeNode *tree);

static void genStmt(TreeNode *tree)
{
	TreeNode *p1, *p2, *p3;
	char temp_label_buff[5];
	
	switch (tree->kind.stmt)
	{
	case IfK:
		p1 = tree->child[0];
		p2 = tree->child[1];
		p3 = tree->child[2];


		cGen(p1);
		sprintf(temp_label_buff, "%d", label_num++);
		emitJumpz(temp_label_buff);
		cGen(p2);
		emitLabel(temp_label_buff);

		break;

	case RepeatK:
		p1 = tree->child[0];
		p2 = tree->child[1];

		sprintf(temp_label_buff, "%d", label_num++);
		emitLabel(temp_label_buff);
		cGen(p1);
		cGen(p2);
		emitJumpz(temp_label_buff);

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
		printf("nodekind:%d\n", tree->nodekind);
		printf("kind:%d\n", tree->kind);
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
	printf("asm_num:%d\n", asm_line_num);
	return asm_code;
}

