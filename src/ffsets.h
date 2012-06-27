#ifndef __FFSETS_H_
#define __FFSETS_H_

/*
	BNF:
	program -> stmt-sequence 
	stmt-sequence -> stmt-sequence ; statement | statement
	statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
	if-stmt -> if exp then stmt-sequence else-part end
	else-part -> else stmt-squence | z 
	repeat-stmt -> repeat stmt-sequence until exp
	assign-stmt -> identifier := exp
	read-stmt -> read identifier
	write-stmt -> write exp
	exp -> simpler-exp comparison-op simpler-exp | simpler-exp
	comparison-op -> < | =
	simpler-exp -> simpler-exp addop term | term
	addop -> + | -
	term -> term mulop factor | factor
	mulop -> * | /
	factor -> ( exp ) | number | identifier

	EBNF:
	program -> stmt-sequence
	stmt-sequence -> statement { ;statement }
	statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
	if-stmt -> if exp then stmt-sequence [else stmt-squence] end
	repeat-stmt -> repeat stmt-sequence until exp
	assign-stmt -> identifier := exp
	read-stmt -> read identifier
	write-stmt -> write exp
	exp -> simpler-exp [ comparison-op simpler-exp ]
	comparison-op -> < | =
	simpler-exp -> term { addop term }
	addop -> + | -
	term -> factor { mulop factor }
	mulop -> * | /
	factor -> ( exp ) | number | identifier

	first set:
	first(program) (not require)
	first(stmt-sequence) = { if, repeat, identifier, read write }
	first(statement) = { if, repeat, identifier, read, write }
	first(if-stmt)  = { if }
	first(repeat-stmt) = { repeat }
	first(assign-stmt) = { identifier }
	first(read-stmt)  = { read }
	first(write-stmt) = { write }
	first(exp) = { (, number, identifier }
	first(comparison-op) = { <, = }
	first(simpler-exp) = { (, number, identifer }
	first(addop)  = { +, - }
	first(term)   = { (, number, identifier }
	first(mulop)  = { *, / }
 	first(factor) = { (, number, identifier }

	follow set:
	follow(prgram) (not require)
	follow(stmt-sequence) = { $, ;, else, end, until }
	follow(statement) = { $, ;, else, end, until }
	follow(if-stmt) = { $, ;, else, end, until }
	follow(repeat-stmt) = { $, ;, else, end, until }
	follow(assign-stmt) = { $, ;, else, end, until }
	follow(read-stmt) = { $, ;, else, end, until }
	follow(write-stmt) = { $, ;, else, end, until }
	follow(exp) = { $, ;, else, end until, then, ) }
	follow(comparson-op) =  { $, (, number, identifier }
	follow(simpler-exp) = { $, +, -, <, =, ;, else, end, until, thel, ) }
	follow(addop)  = { $, (, number, identifier }
	follow(term)   = { $, *, /, +, -, <, =, ;, else, end, until, then, ) }
	follow(mulop)  = { $, (, number, identifier }
	follow(factor) = { $, *, /, +, -, <, =, ;, else, end, until, then, ) }

*/

// first sets
#define FIRSTSC_STMT_SEQUENCE           5
#define FIRSTSC_STATEMENT               5
#define FIRSTSC_IF_STMT                 1
#define FIRSTSC_REPEAT_STMT				1
#define FIRSTSC_ASSIGN_STMT				1
#define FIRSTSC_READ_STMT				1
#define FIRSTSC_WRITE_STMT				1
#define FIRSTSC_EXP						3
#define FIRSTSC_COMPARISON_OP			2
#define FIRSTSC_SIMPLER_EXP				3
#define FIRSTSC_ADDOP					2
#define FIRSTSC_TERM					3
#define FIRSTSC_MULOP					2
#define FIRSTSC_FACTOR					3

TokenType firstset_stmt_sequence  [FIRSTSC_STMT_SEQUENCE] = { IF, REPEAT, IDENTIFIER, READ, WRITE };
TokenType firstset_statement      [FIRSTSC_STATEMENT]     = { IF, REPEAT, IDENTIFIER, READ, WRITE };
TokenType firstset_if_stmt        [FIRSTSC_IF_STMT]       = { IF };
TokenType firstset_repeat_stmt    [FIRSTSC_REPEAT_STMT]   = { REPEAT };
TokenType firstset_assign_stmt    [FIRSTSC_ASSIGN_STMT]   = { IDENTIFIER };
TokenType firstset_read_stmt      [FIRSTSC_WRITE_STMT]    = { READ };
TokenType firstset_write_stmt     [FIRSTSC_EXP]           = { WRITE };
TokenType firstset_exp            [FIRSTSC_EXP]           = { LPARENTHESE, NUMBER, IDENTIFIER };
TokenType firstset_comparison_op  [FIRSTSC_COMPARISON_OP] = { LESS, EQUAL };
TokenType firstset_simpler_exp    [FIRSTSC_SIMPLER_EXP]   = { LPARENTHESE, NUMBER, IDENTIFIER };
TokenType firstset_addop          [FIRSTSC_ADDOP]         = { ADD, SUB };
TokenType firstset_term           [FIRSTSC_TERM]          = { LPARENTHESE, NUMBER, IDENTIFIER };
TokenType firstset_mulop          [FIRSTSC_MULOP]         = { MUL, DIV };
TokenType firstset_factor         [FIRSTSC_FACTOR]        = { LPARENTHESE, NUMBER, IDENTIFIER };

// follow sets
#define FOLLOWSC_STMT_SEQUENCE     6
#define FOLLOWSC_STATEMENT         6
#define FOLLOWSC_IF_STMT           6
#define FOLLOWSC_REPEAT_STMT	   6
#define FOLLOWSC_ASSIGN_STMT       6
#define FOLLOWSC_READ_STMT         6
#define FOLLOWSC_WRITE_STMT        6
#define FOLLOWSC_EXP               7
#define FOLLOWSC_COMPARISON_OP     4
#define FOLLOWSC_SIMPLER_EXP       11
#define FOLLOWSC_ADDOP             4
#define FOLLOWSC_TERM              13
#define FOLLOWSC_MULOP             4
#define FOLLOWSC_FACTOR            13

TokenType followset_stmt_sequence[FOLLOWSC_STMT_SEQUENCE] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_statement[FOLLOWSC_STATEMENT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_if_stmt[FOLLOWSC_IF_STMT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_repeat_stmt[FOLLOWSC_REPEAT_STMT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_assign_stmt[FOLLOWSC_ASSIGN_STMT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_read_stmt[FOLLOWSC_READ_STMT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_write_stmt[FOLLOWSC_WRITE_STMT] = {ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN};
TokenType followset_exp[FOLLOWSC_EXP] = { ENDFILE, SEMICOLON, ELSE, END, UNTIL, THEN, RPARENTHESE};
TokenType followset_comparison_op[FOLLOWSC_COMPARISON_OP] = { ENDFILE, LPARENTHESE, NUMBER, IDENTIFIER};
TokenType followset_simpler_exp[FOLLOWSC_SIMPLER_EXP] = { ENDFILE, ADD, SUB, LESS, EQUAL, SEMICOLON, ELSE, END, UNTIL, THEN, RPARENTHESE};
TokenType followset_addop[FOLLOWSC_ADDOP] = { ENDFILE, LPARENTHESE, NUMBER, IDENTIFIER};
TokenType followset_term[FOLLOWSC_TERM] = { ENDFILE, MUL, DIV, ADD, SUB, LESS, EQUAL, SEMICOLON, ELSE, END, UNTIL, THEN, RPARENTHESE};
TokenType followset_mulop[FOLLOWSC_MULOP] = { ENDFILE, LPARENTHESE, NUMBER, IDENTIFIER};
TokenType followset_factor[FOLLOWSC_FACTOR] = { ENDFILE, MUL, DIV, ADD, SUB, LESS, EQUAL, SEMICOLON, ELSE, END, UNTIL, THEN, RPARENTHESE};



#endif