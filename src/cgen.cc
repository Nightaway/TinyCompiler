#include "globals.h"
#include "parser.h"
#include "symtab.h"
#include "code.h"
#include "cgen.h"

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

		cGen(p1);

		switch (p1->nodekind)
		{
		int loc;
		case StmtK:
			switch(p1->kind.stmt)
			{
			case AssignK:

				break;
			}
			break;

		case ExpK:
			switch(p1->kind.exp)
			{
			case ConstK:

				break;
			case IdK:

				break;
			case OpK:
				break;
			}
			break;
		}

		break;

	case ReadK:
		break;

	case WriteK:
		break;
	default : break;
	}
}

static void genExp(TreeNode *tree, TreeType ttype)
{
	TreeNode *t1, *t2;
	int loc = 0;
	int tempvar_loc = 0;

	t1 = tree->child[0];
	t2 = tree->child[1];
	switch (tree->kind.exp)
	{
	case ConstK:
		break;

	case IdK:
		break;

	case OpK:
		genExp(t1, LEFT);
		genExp(t2, RIGHT);

		switch (t1->kind.exp)
		{
		case ConstK:
			break;

		case IdK:
			break;

		case OpK:
			break;
		}

		switch (t2->kind.exp)
		{
		case ConstK:
			break;

		case IdK:
			break;

		case OpK:
			break;
		}

		switch (ttype)
		{
		case ROOT:
			switch (tree->attr.token.ttype)
			{
			case ADD:
				break;
			case SUB:
				break;
			case MUL:
				break;
			case DIV:
				break;
			}

		break;

		case LEFT:
			switch (tree->attr.token.ttype)
			{
			case ADD:
				break;
			case SUB:

				break;

			case MUL:
				break;

			case DIV:
				break;
			}
		break;

		case RIGHT:
			switch (tree->attr.token.ttype)
			{
			case ADD:
				break;

			case SUB:
				break;

			case MUL:
				break;

			case DIV:
				break;
			}
		 break;
		};

	break;
	}
}

static void cGen(TreeNode *tree)
{
	if (tree != NULL)
	{
		switch (tree->nodekind)
		{
		case StmtK:
			genStmt(tree);
			break;

		case ExpK:
			genExp(tree, ROOT);
			break;

		default : break;
		}
		cGen(tree->sibling);
	}
}


void codeGen(TreeNode *syntaxTree)
{
	cGen(syntaxTree);
}

