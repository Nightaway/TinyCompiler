#ifndef __SCANNER_H_
#define __SCANNER_H_

/*
		       Table of Tokens
_________________________________________________
| Reserved Words | Special Symbols | Other	    |										
|________________|_________________|____________|
|  if            | +               | number     |
|  then          | -               | identifier |
|  else          | *			   |			|
|  end			 | /			   |			|
|  repeat		 | =			   |			|
|  until		 | <			   |			|
|  read			 | (			   |			|
|  write		 | )			   |			|
|				 | ;			   |			|
|				 | :=			   |			|
|________________|_________________|____________|

Regular Expression List :
number = [0-9]+ ;       identifier = [a-zA-Z]+; 

*/

//
//	token type defination 
//
typedef enum {
	IF,
	THEN,
	ELSE,
	END,
	REPEAT,
	UNTIL,
	READ,
	WRITE,

	ADD,
	SUB,
	MUL,
	DIV,

	EQUAL,
	LESS,
	LPARENTHESE,
	RPARENTHESE,
	SEMICOLON,
	ASSIGN,

	NUMBER,
	IDENTIFIER,

	ENDFILE,
	ERROR
}TokenType;

//
// token attributes
//
typedef struct Token{
	TokenType ttype;    // for TokenType
	char stype[40];     // for string value
	int val;		    // for integer value
	int lineno; 
}Token;

//
// return the token and increase the pointer to the stream 
//
Token getToken(void);

void getTokenStream(Token *tokens, int count);

Token makeToken(TokenType ttype);
Token makeError();

#endif