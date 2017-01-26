#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "ffsets.h"

enum {
	NORMAL,
	EXTRA
};

int mode_tokens = NORMAL;

int index_tokens = 0;

#define COUNT_TOKENS 2
Token tokens[COUNT_TOKENS];
Token token;

enum {
	IN_READ_STMT,
	IN_IF_STMT,
	IN_REPEAT_STMT
};

int follow_state = 0;
int read_stmt_error_flag = 0;

void setInReadStmtFlag()
{
	if (read_stmt_error_flag != IN_IF_STMT &&
		read_stmt_error_flag != IN_REPEAT_STMT)
		read_stmt_error_flag = IN_READ_STMT;
}

void setInIfStmtFlag()
{
	read_stmt_error_flag = IN_IF_STMT;
}


//
void advanceToken(void);
void error(void);
void match(TokenType ttype);

//
TokenType *combineset(TokenType *pFirstSet, int firstscount,
					  TokenType *pFollowSet, int followscount);
void checkinput(TokenType *pFirstSet, int firstscount, 
				TokenType *pFollowSet, int followscount);
void scanto(TokenType *synchset, int setcount);

//

TreeNode *stmt_sequence(TokenType *pFollowSet, int count);
TreeNode *statement(void);

TreeNode *if_stmt(void);
TreeNode *repeat_stmt(void);
TreeNode *assign_stmt(void);
TreeNode *read_stmt(void);
TreeNode *write_stmt(void);
TreeNode *exp(TokenType *pFollowSet, int count);
TreeNode *simpler_exp(void);
TreeNode *factor(void);
TreeNode *term(void);

TreeNode *makeNewNode(void);
TreeNode *makeConstNode(Token token);
TreeNode *makeOpNode(Token token);
TreeNode *makeIdNode(Token token);

void tokencpy(Token *dest, Token *src);

void leftChild(TreeNode *destNode, TreeNode *leftNode);
void rightChild(TreeNode *destNode, TreeNode *rightNode);
void elseChild(TreeNode *destNode, TreeNode *elseNode);
void siblingNode(TreeNode *destNode, TreeNode *siblingNode);


TreeNode *parse(void)
{
	getTokenStream(tokens, COUNT_TOKENS);
	advanceToken();
	return stmt_sequence(followset_stmt_sequence, FOLLOWSC_STMT_SEQUENCE);
}

void advanceToken(void)
{
	token = tokens[index_tokens++];
	if (mode_tokens == NORMAL)
	{
		if (index_tokens >= 2)
		{
			getTokenStream(tokens, COUNT_TOKENS);
			index_tokens = 0;
		}
	}
	else if (mode_tokens == EXTRA)
	{
		if (index_tokens >= 3)
		{
			getTokenStream(tokens, COUNT_TOKENS);
			index_tokens = 0;
			mode_tokens = NORMAL;
		}
	}

	switch (token.ttype)
	{
	case ERROR:
		printf("%s\n", token.stype);
	break;
	}
}

void tokencpy(Token *dest, Token *src)
{
	dest->ttype = src->ttype;
	dest->lineno = src->lineno;
	strcpy(dest->stype, src->stype);
	dest->val = src->val;
}

void error(void)
{
	printf("syntax error at line %d : unexpected token '%s'\n", 
		token.lineno, token.stype);
}

void match(TokenType ttype)
{
	if (token.ttype == ttype)
		advanceToken();
	else
	{
		error();
	}
}

//
int lookupset(TokenType *pSet, int setc)
{
	int i;
	for (i = 0; i < setc; ++i)
	{
		if (token.ttype == pSet[i])
		{
			i = 1;
			return i;
		}
	}
	i = 0;
	return i;
}

TokenType *combineset(TokenType *pFirstSet, int firstscount, 
					  TokenType *pFollowSet, int followscount)
{
	int i;
	TokenType *cset = (TokenType *)malloc(sizeof(pFirstSet) * firstscount  
										+ sizeof(pFollowSet) * followscount);
	TokenType *pset = cset;
	for (i = 0; i < firstscount; ++i, ++pset)
	{
		*pset = pFirstSet[i];
	}

	
	for (i = 0; i < followscount; ++i, ++pset)
	{
		*pset = pFollowSet[i];
	}

	return cset;
}

void checkinput(TokenType *pFirstSet, int firstscount, 
				TokenType *pFollowSet, int followscount)
{
	int i, flag = 0;
	for (i = 0; i < firstscount; ++i)
	{
		if (token.ttype == pFirstSet[i])
		{
			flag = 1;
			break;
		}
	}

	if (!flag)
	{
		error();
		scanto(combineset(pFirstSet, firstscount, pFollowSet, followscount), 
			  firstscount + followscount);
	}
}

void scanto(TokenType *synchset, int setcount)
{
	int i;
	do {
		for (i = 0; i < setcount; ++i)
		{
			if (token.ttype == synchset[i])
				return ;
		}
		token = getToken();
	} while (token.ttype != ENDFILE);
}

//
void comparison_op(void)
{
	switch (token.ttype)
	{
	case LESS:
		match(LESS);
		break;
	case EQUAL:
		match(EQUAL);
		break;
	default: error();
	}
}

void addop(void)
{
	switch (token.ttype)
	{
	case ADD:
		match(ADD);
		break;
	case SUB:
		match(SUB);
		break;
	default: error();
	}
}

void mulop(void)
{
	switch (token.ttype)
	{
	case MUL:
		match(MUL);
		break;
	case DIV:
		match(DIV);
		break;
	default: error();
	}

}

TreeNode *stmt_sequence(TokenType *pFollowSet, int count)
{
	/*
		BNF:
			stmt-squence -> stmt-squence; stmt | stmt

		EBNF:
			stmt-squence -> stmt { ;stmt }
	*/
	TreeNode *temp = NULL, *newtemp = NULL, *headtemp = NULL;

	checkinput(firstset_stmt_sequence, FIRSTSC_STMT_SEQUENCE, pFollowSet, count);

	if (token.ttype != lookupset(pFollowSet, count))
	{
		headtemp = statement();
		temp = headtemp;

		while (token.ttype == SEMICOLON)
		{
			match(SEMICOLON);
			newtemp = statement();
			if (newtemp != NULL)
			{
				if (temp != NULL)
				{
					siblingNode(temp, newtemp);
					temp = newtemp;
				}
				else
				{
					headtemp = newtemp;
					temp = headtemp;
				}
			}
		}
	}

	return headtemp;

	
	/*BNF:
		stmt-squence -> stmt-squence stmt; | stmt;

	EBNF:
		stmt-squence -> { stmt; }
	
	TreeNode *temp = NULL, *newtemp = NULL, *headtemp = NULL;
	temp = statement();
	headNode = temp;

	while (temp != NULL)
	{
		match(';');

		if (newtemp != NULL)
		{
			siblingNode(newtemp, temp);
		}

		newtemp = temp;
		temp = statement();
	}

	return headNode;*/
}

TreeNode *statement(void)
{
	TreeNode *temp = NULL;

	checkinput(firstset_statement, FIRSTSC_STATEMENT, followset_statement, FOLLOWSC_STATEMENT);

	if (!lookupset(followset_statement, FOLLOWSC_STATEMENT))
	{

		switch (token.ttype)
		{
		case IF :
			setInIfStmtFlag();
			temp = if_stmt();
			break;

		case REPEAT :
			temp = repeat_stmt();
			break;

		case IDENTIFIER:
			temp = assign_stmt();
			break;

		case READ:
			setInReadStmtFlag();
			temp = read_stmt();
			break;

		case WRITE:
			temp = write_stmt();
			break;

		default: error();
		}
	}

	return temp;
}

TreeNode *if_stmt(void)
{
	TreeNode *ifNode = NULL, *expNode = NULL, *stmtsNode = NULL, *estmtsNodo = NULL;

	checkinput(firstset_if_stmt, FIRSTSC_IF_STMT, followset_statement, FOLLOWSC_STATEMENT);

	if (!lookupset(followset_statement, FOLLOWSC_STATEMENT))
	{
		if (token.ttype == IF)
		{
			TokenType followset[2] = {ENDFILE, THEN};
			TokenType followset2[3] = {ENDFILE, ELSE, END};

			ifNode = makeNewNode();
			tokencpy(&ifNode->attr.token, &token);
			ifNode->nodekind  = StmtK;
			ifNode->kind.stmt = IfK;
	

			match(IF);
			
			expNode = exp(followset, 2);

			match(THEN);

			stmtsNode = stmt_sequence(followset2, 3);

			leftChild(ifNode, expNode);
			rightChild(ifNode, stmtsNode);

			if (token.ttype == ELSE)
			{
				TokenType followset3[2] = { ENDFILE, END};
				match(ELSE);
				estmtsNodo = stmt_sequence(followset3, 2);
				elseChild(ifNode, estmtsNodo);
			}

			match(END);
		}
		else
		{
			error();
		}
	}

	return ifNode;
}

TreeNode *repeat_stmt(void)
{
	TreeNode *repeatNode = NULL, *stmtsNode = NULL, *expNode = NULL;

	checkinput(firstset_repeat_stmt, FIRSTSC_REPEAT_STMT, followset_repeat_stmt, FOLLOWSC_REPEAT_STMT);

	if (!lookupset(followset_repeat_stmt, FOLLOWSC_REPEAT_STMT))
	{
		if (token.ttype == REPEAT)
		{
			TokenType followset = UNTIL;

			repeatNode = makeNewNode();
			tokencpy(&repeatNode->attr.token, &token);
			repeatNode->nodekind  = StmtK;
			repeatNode->kind.stmt = RepeatK;
		
			match(REPEAT);
	
			stmtsNode = stmt_sequence(&followset, 1);

			match(UNTIL);

			expNode = exp(followset_exp, FOLLOWSC_EXP);

			leftChild(repeatNode, stmtsNode);
			rightChild(repeatNode, expNode);
		}
	}

	return repeatNode;
}

TreeNode *assign_stmt(void)
{
	TreeNode *AssignNode = NULL, *leftNode = NULL, *rightNode = NULL;

	checkinput(firstset_assign_stmt, FIRSTSC_ASSIGN_STMT, followset_assign_stmt, FOLLOWSC_ASSIGN_STMT);

	if (!lookupset(followset_assign_stmt, FOLLOWSC_ASSIGN_STMT))
	{
		if (token.ttype == IDENTIFIER)
		{
			AssignNode = makeNewNode();
			tokencpy(&AssignNode->attr.token, &token);
			AssignNode->nodekind  = StmtK;
			AssignNode->kind.stmt = AssignK;

			match(IDENTIFIER);

			if (token.ttype == ASSIGN)
			{
				match(ASSIGN);

				leftNode = exp(followset_exp, FOLLOWSC_EXP);
				
				leftChild(AssignNode, leftNode);
			}
			else
			{
				printf("syntax error at line %d : missing ':=' \n", token.lineno);
			}
		}
	}
	
	return AssignNode;
}

TreeNode *read_stmt(void)
{
	TreeNode *readNode = NULL, *temp = NULL; 

	checkinput(firstset_read_stmt, FIRSTSC_READ_STMT, followset_read_stmt, FOLLOWSC_READ_STMT);

	if (!lookupset(followset_read_stmt, FOLLOWSC_READ_STMT))
	{
		if (token.ttype == READ)
		{
			match(READ);

			if (token.ttype == IDENTIFIER)
			{
				readNode = makeNewNode();
				tokencpy(&readNode->attr.token, &token);
				readNode->nodekind = StmtK;
				readNode->kind.stmt = ReadK;
		
				advanceToken();
			}
			else
			{
				printf("syntax error at line %d : missing identifier \n", token.lineno);
			}
	
		}

		while (1)
		{
			switch (follow_state)
			{
			case 0 :
				switch (read_stmt_error_flag)
				{
				case IN_READ_STMT:
					if (token.ttype != ENDFILE &&
						token.ttype != SEMICOLON)
					{
						follow_state = 1;
					}
					else
					{
						follow_state = 2;
					}
				break;
				case IN_IF_STMT:
					if (token.ttype != SEMICOLON &&
						token.ttype != ELSE &&
						token.ttype != END)
					{
						follow_state = 1;
					}
					else
					{
						follow_state = 2;
					}
				break;
				case IN_REPEAT_STMT:
					follow_state = 2;
				break;

				default : follow_state = 2; break;
				}
			break;
			
			case 1 :
				if (index_tokens == 0)
				{
					tokens[2] = tokens[1];
					tokens[1] = tokens[0];
					tokens[0] = token;
					token = makeToken(SEMICOLON);
					token.lineno = tokens[0].lineno;
					mode_tokens = EXTRA;
					error();
				}
				else 
				if (index_tokens == 1)
				{
					tokens[0] = token;
					token = makeToken(SEMICOLON);
					token.lineno = tokens[0].lineno;
					--index_tokens;
					error();
				}

			return readNode;


			case 2 :
			default: return readNode;
			}
		}
	}

	return readNode;
}

TreeNode *write_stmt(void)
{
	TreeNode *writeNode = NULL, *temp = NULL; 

	checkinput(firstset_write_stmt, FIRSTSC_WRITE_STMT, followset_write_stmt, FOLLOWSC_WRITE_STMT);

	if (!lookupset(followset_write_stmt, FOLLOWSC_WRITE_STMT))
	{
		if (token.ttype == WRITE)
		{
			writeNode = makeNewNode();
			tokencpy(&writeNode->attr.token, &token);
			writeNode->nodekind  = StmtK;
			writeNode->kind.stmt = WriteK;
			match(WRITE);

			temp = exp(followset_exp, FOLLOWSC_EXP);
			leftChild(writeNode, temp);

		}
		else
		{
			error();
		}
	}

	return writeNode;
}

TreeNode *exp(TokenType *pFollowSet, int count)
{
	int flag = 0;
	TreeNode *temp = NULL, *newtemp = NULL;

	checkinput(firstset_exp, FIRSTSC_EXP, pFollowSet, count);

	if (!lookupset(pFollowSet, count))
	{
		temp = simpler_exp();

		if (token.ttype == LESS ||
			token.ttype == EQUAL)
		{
			newtemp = makeOpNode(token);
			leftChild(newtemp, temp);
			comparison_op();
			rightChild(newtemp, simpler_exp());

			flag = 1;
		}

		if (flag)
			return newtemp; 
		else
			return temp;
	}

	return NULL;
}

TreeNode *simpler_exp(void)
{
	int flag = 0;
	TreeNode *temp = NULL, *newtemp = NULL;

	checkinput(firstset_simpler_exp, FIRSTSC_SIMPLER_EXP, followset_simpler_exp, FOLLOWSC_SIMPLER_EXP);

	if (!lookupset(followset_simpler_exp, FOLLOWSC_SIMPLER_EXP))
	{
		temp = term();
		while (token.ttype == ADD || 
			   token.ttype == SUB)
		{
			newtemp = makeOpNode(token);
			leftChild(newtemp, temp);
			addop();
			rightChild(newtemp, term());
		
			flag = 1;
		}

		if (flag)
			return newtemp; 
		else
			return temp;
	}

	return NULL;
}

TreeNode *term(void)
{
	int flag = 0;
	TreeNode *temp = NULL, *newtemp = NULL;

	checkinput(firstset_term, FIRSTSC_TERM, followset_term, FOLLOWSC_TERM);

	if (!lookupset(followset_term, FOLLOWSC_TERM))
	{
		temp = factor();
		while (token.ttype == MUL || 
			   token.ttype == DIV)
		{
			newtemp = makeOpNode(token);
			leftChild(newtemp, temp);
			mulop();
			rightChild(newtemp, factor());

			flag = 1;
		}

		if (flag)
			return newtemp;
		else
			return temp;
	}

	return NULL;
}

TreeNode *factor(void)
{
	TreeNode *node = NULL;

	checkinput(firstset_factor, FIRSTSC_FACTOR, followset_factor, FOLLOWSC_FACTOR);

	if (!lookupset(followset_factor, FOLLOWSC_FACTOR))
	{
		TokenType followset = RPARENTHESE;
		switch (token.ttype)
		{
		case LPARENTHESE:
			match(LPARENTHESE);
			node = exp(&followset, 1);
			match(RPARENTHESE);
			break;

		case NUMBER:
			node = makeConstNode(token);
			advanceToken();
			break;

		case IDENTIFIER:
			node = makeIdNode(token);
			advanceToken();
			break;

		default: error(); break;
		}
	}
	return node;
}

TreeNode *makeNewNode(void)
{
	TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
	memset(newNode, 0, sizeof(TreeNode));

	return newNode;
}

TreeNode *makeConstNode(Token token)
{
	TreeNode *constOp = makeNewNode();

	constOp->attr.token.ttype = token.ttype;
	strcpy(constOp->attr.token.stype, token.stype);
	constOp->attr.token.val = token.val;
	constOp->attr.token.lineno = token.lineno;
	constOp->nodekind = ExpK;
	constOp->kind.exp = ConstK;

	return constOp;
}

TreeNode *makeOpNode(Token token)
{
	TreeNode *opNode = makeNewNode();

	opNode->attr.token.ttype = token.ttype;
	strcpy(opNode->attr.token.stype, token.stype);
	opNode->attr.token.val = token.val;
	opNode->attr.token.lineno = token.lineno;
	opNode->nodekind = ExpK;
	opNode->kind.exp = OpK;

	return opNode;
}

TreeNode *makeIdNode(Token token)
{
	TreeNode *idNode = makeNewNode();

	idNode->attr.token.ttype = token.ttype;
	strcpy(idNode->attr.token.stype, token.stype);
	idNode->attr.token.val = token.val;
	idNode->attr.token.lineno = token.lineno;
	idNode->nodekind = ExpK;
	idNode->kind.exp = IdK;

	return idNode;
}
void leftChild(TreeNode *destNode, TreeNode *leftNode)
{
	destNode->child[0] = leftNode;
}

void rightChild(TreeNode *destNode, TreeNode *rightNode)
{
	destNode->child[1] = rightNode;
}

void elseChild(TreeNode *destNode, TreeNode *elseNode)
{
	destNode->child[2] = elseNode;
}

void siblingNode(TreeNode *destNode, TreeNode *siblingNode)
{
	destNode->sibling = siblingNode;
}

