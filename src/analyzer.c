#include <stdio.h>

#include "scanner.h"
#include "globals.h"
#include "analyzer.h"

static int location = 0;

static void traverse( TreeNode *t,
					 void (* preProc) (TreeNode *),
					 void (* postProc) (TreeNode *))
{
	if (t != NULL)
	{
		preProc(t);
		{
			int i;
			for (i = 0; i < MAXCHILDREN; ++i)
				traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);
	}
}

static void nullProc(TreeNode *t)
{
	if (t==NULL) return ;
	else return ;
}

static void insertNode(TreeNode *t)
{
	switch	(t->nodekind)
	{
	case StmtK:
		switch (t->kind.stmt)
		{
		case AssignK:
		case ReadK:
			if (st_lookup(t->attr.token.stype) == -1)
				st_insert(t->attr.token.stype, t->attr.token.lineno, location++);
			else
				st_insert(t->attr.token.stype, t->attr.token.lineno, 0);
			break;
		default:
			break;

		}
	break;

	case ExpK:
		switch (t->kind.exp)
		{
		case IdK:
			if (st_lookup(t->attr.token.stype) == -1)
				st_insert(t->attr.token.stype, t->attr.token.lineno, location++);
			else
				st_insert(t->attr.token.stype, t->attr.token.lineno, 0);
			break;
		default:
			break;
		}
	break;
	}
}

void buildSymtab(TreeNode *syntaxTree)
{
	traverse(syntaxTree, insertNode, nullProc);
	printf("\nSymbol table:\n\n");
	printSymtab();
}

static void typeError(TreeNode *t, char *message)
{
	printf("Type error at line %d: %s\n", t->attr.token.lineno, message);
}

static void checkNode(TreeNode *t)
{
	switch (t->nodekind)
	{
	case ExpK:
		switch (t->kind.exp)
		{
		case OpK:
			if ((t->child[0]->type != Integer) ||
				(t->child[1]->type != Integer))
				typeError(t, "Op applied to non-integer");
			if ((t->attr.token.ttype == EQUAL) || (t->attr.token.ttype == LESS))
				t->type = Boolean;
			else
				t->type = Integer;
			break;
		case ConstK:
		case IdK:
			t->type = Integer;
			break;
		default:
			break;
		}
	break;

	case StmtK:
		switch (t->kind.stmt)
		{
		case IfK:
			if (t->child[0]->type == Integer)
				typeError(t->child[0], "if test is not Boolean");
			break;
		case AssignK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "assignment of non-integer value");
			break;
		case WriteK:
			if (t->child[0]->type != Integer)
				typeError(t->child[0], "write of non-integer vaule");
			break;
		case RepeatK:
			if (t->child[0]->type == Integer)
				typeError(t->child[0], "repeat test is not Boolean");
			break;
		default:
			break;
		}
	break;

	default:
		break;
	}
}

void typeCheck(TreeNode *node)
{
	traverse(node, nullProc, checkNode);
}