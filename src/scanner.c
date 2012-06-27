#include "Scanner.h"

#include <stdio.h>
#include <string.h>

//
// number of Reserved Words
//
#define RWCOUNT 8

//
//Dictionary of Reserved Words
//
const Token RWTokens[RWCOUNT] =
{
	{IF,	 "if"},
	{THEN,	 "then"},
	{ELSE,	 "else"},
	{END,	 "end"},
	{REPEAT, "repeat"},
	{UNTIL,	 "until"},
	{READ,   "read"},
	{WRITE,  "write"},
};

// the pointer to the stream of characters.
char *pStream;

// variable which store automa states
int state;

// line number
int lineno = 1;

// look up for Reserved Word, return the index of Dictionary if found it,
// else return -1 indicates false.
int lookupRW(const char *pRW)
{
	int i;
	for (i = 0; i < RWCOUNT; ++i)
	{
		if (strcmp(RWTokens[i].stype, pRW) == 0)
			return i;
	}
	return -1;
}

//

void ungetToken(TokenType ttype)
{
	Token token = makeToken(ttype);
	int len = strlen(token.stype);
}

void getTokenStream(Token *tokens, int count)
{
	int i;
	for (i = 0; i < count; ++i)
	{
		tokens[i] = getToken();
		if (tokens[i].ttype == ERROR || 
			tokens[i].ttype == ENDFILE)
			return ;
	}
}

//
Token makeToken(TokenType ttype)
{
	Token token;
	token.lineno = lineno;
	switch (ttype)
	{
	case IF :
		token.ttype = IF;
		strcpy_s(token.stype, 40, "if");
		token.val = 0;
		break;

	case THEN :
		token.ttype = THEN;
		strcpy_s(token.stype, 40, "then");
		token.val = 0;
		break;

	case ELSE :
		token.ttype = ELSE;
		strcpy_s(token.stype, 40, "else");
		token.val = 0;
		break;
	
	case END :
		token.ttype = END;
		strcpy_s(token.stype, 40, "end");
		token.val = 0;
		break;

	case REPEAT :
		token.ttype = REPEAT;
		strcpy_s(token.stype, 40, "repeat");
		token.val = 0;
		break;
		
	case UNTIL :
		token.ttype = UNTIL;
		strcpy_s(token.stype, 40, "until");
		token.val = 0;
		break;
	
	case READ :
		token.ttype = READ;
		strcpy_s(token.stype, 40, "read");
		token.val = 0;
		break;

	case WRITE :
		token.ttype = WRITE;
		strcpy_s(token.stype, 40, "write");
		token.val = 0;
		break;

	case ADD :
		token.ttype = ADD;
		strcpy_s(token.stype, 40, "+");
		token.val = 0;
		break;

	case SUB :
		token.ttype = SUB;
		strcpy_s(token.stype, 40, "-");
		token.val = 0;
		break;

	case MUL :
		token.ttype = MUL;
		strcpy_s(token.stype, 40, "*");
		token.val = 0;
		break;

	case DIV :
		token.ttype = DIV;
		strcpy_s(token.stype, 40, "/");
		token.val = 0;
		break;

	case EQUAL :
		token.ttype = EQUAL;
		strcpy_s(token.stype, 40, "=");
		token.val = 0;
		break;

	case LESS :
		token.ttype = LESS;
		strcpy_s(token.stype, 40, "<");
		token.val = 0;
		break;

	case LPARENTHESE :
		token.ttype = LPARENTHESE;
		strcpy_s(token.stype, 40, "(");
		token.val = 0;
		break;

	case RPARENTHESE :
		token.ttype = RPARENTHESE;
		strcpy_s(token.stype, 40, ")");
		token.val = 0;
		break;

	case SEMICOLON :
		token.ttype = SEMICOLON;
		strcpy_s(token.stype, 40, ";");
		token.val = 0;
		break;

	case ASSIGN :
		token.ttype = ASSIGN;
		strcpy_s(token.stype, 40, ":=");
		token.val = 0;
		break;
	}
	return token;
}

Token makeError(char pbuff[], int bufIndex)
{
	Token token;
	pbuff[bufIndex++] = *pStream; 
	pbuff[bufIndex] = '\0';
	sprintf_s(token.stype, 40, "Error : Unrecognized token '%s'", pbuff);
    token.lineno = lineno;
	token.ttype = ERROR;
	return token;
}

Token getToken(void)
{
	//
	// automa states defination
	//
	enum {
		S_START,
		S_DONE,
		S_ENDFILE,

		S_INCOMMENT,
		S_INNUM,
		S_INID,
		S_INASSIGN,
	};

	// used for saving error infomation
	/*char bufError[40];*/

	// used for saving characters which being used in scanning
	char pbuff[40];
	int result;

	// index to pbuff
	int bufIndex = 0;

	// token initalization
	Token token = {0};

	// Initalize the state for starting
	state = S_START;

	while (1)
	{
		switch (state)
		{
		case S_START:
			switch (*pStream)
			{
			// Advancing the pointer,
			// if character is blank or tab
			//
			case ' '  :
			case '	' :
				++pStream;
				break;

			// newline
			case 0x0d :
				++lineno;
				++pStream;
				break;
			case '\n' :
				++pStream;
			break;

			//
			// comment '{' 
			//
			case '{' :
				state = S_INCOMMENT;
				++pStream;
			break;

			//
			// Special Symbols
			//
			case '+' :
				state = S_START;
				++pStream;
			return makeToken(ADD);

			case '-' :
				state = S_START;
				++pStream;
			return makeToken(SUB);

			case '*' :
				state = S_START;
				++pStream;
			return makeToken(MUL);

			case '\\' :
				state = S_START;
				++pStream;
			return makeToken(DIV);

			case '=' :
				state = S_START;
				++pStream;
			return makeToken(EQUAL);

			case '<' :
				state = S_START;
				++pStream;
			return makeToken(LESS);

			case '(' :
				state = S_START;
				++pStream;
			return makeToken(LPARENTHESE);

			case ')' :
				state = S_START;
				++pStream;
			return makeToken(RPARENTHESE);

			case ';' :
				state = S_START;
				++pStream;
			return makeToken(SEMICOLON);

			//
			// assign := 
			//
			case ':' :
				++pStream;
				state = S_INASSIGN;
			break;

			//
			// end of file
			//
			case '#' :
				pbuff[bufIndex++] = *pStream;
				++pStream;
				state = S_ENDFILE;
			break;

			default :
				//
				// in identifier [A-Za-z]+
				//
				if ((*pStream >= 'A' && *pStream <= 'Z') ||
					(*pStream >= 'a' && *pStream <= 'z'))
				{
					pbuff[bufIndex++] = *pStream;
					++pStream;
					state = S_INID;
					break;
				}
				else
				//
				// in number      [0-9]+
				//
				if (*pStream >= '0' && *pStream <= '9')
				{
					pbuff[bufIndex++] = *pStream;
					++pStream;
					state = S_INNUM;
					break;
				}
				else
				{
					// 
					// return error token if characters can not be recognized 
					//
					return makeError(pbuff, bufIndex);
				}
			break;
			}
		break;

		//
		// assign := 
		//
		case S_INASSIGN :
			if (*pStream == '=')
			{
				++pStream;
				return makeToken(ASSIGN);
			}
			else
			{
				// 
				// return error token if characters can not be recognized
				//
				return makeError(pbuff, bufIndex);;
			}
		break;

		//
		// identifier
		//
		case S_INID :

			//
			// return the identifier or Reserved Word if next character not be a letter([A-Za-z]+).
			//
			if (!((*pStream >= 'A' && *pStream <= 'Z') ||
				(*pStream >= 'a' && *pStream <= 'z')))
			{
				//
				// looking up dictionary and if current string equals
				// the Reserved Word that already in dictionary then 
				// return the Reserved Word else return identifier
				//
				pbuff[bufIndex] = '\0';
				result = lookupRW(pbuff);
				if (result != -1)
				{
					// no need ++ pointer
					token =  RWTokens[result];
					token.lineno = lineno;
					return token;
				}
				else
				{
					// Identifier
					token.lineno = lineno;
					token.ttype = IDENTIFIER;
					strcpy_s(token.stype, 40, pbuff); 
					return token;
				}
				// Nerver execute here
			}

			// Do loop
			pbuff[bufIndex++] = *pStream;
			++pStream;
		break;

		//
		// in number 
		//
		case S_INNUM :
			//
			// return number if next character is not in range [0 - 9]
			//
			if (!(*pStream >= '0' && *pStream <= '9'))
			{
				pbuff[bufIndex] = '\0';
				token.lineno = lineno;
				token.ttype = NUMBER;
				strcpy_s(token.stype, 40, pbuff); 
				sscanf_s(pbuff, "%d", &token.val);
				return token;
			}

			// Do loop 
			pbuff[bufIndex++] = *pStream;
			++pStream;
		break;

		//
		// Just advance the pointer 
		// until encounter a character of '}' then break and
		// reback in S_STARTS state
		//
		case S_INCOMMENT :
			// new line
			if (*pStream == '\n')
			{
				++lineno;
				++pStream;
				break;
			}
			else
			// end in comment
			if (*pStream == '}')
			{
				++pStream;
				state = S_START;
				break;
			}

			++pStream;
		break;

		//
		// Reserved for future using
		//
		case S_DONE :

		break;

		//
		// return the token of ENDFILE if recognited "#e" which indicates end of a file
		// else return error token
		//
		case S_ENDFILE :
			if (*pStream == 'e')
			{
				token.lineno = lineno;
				token.ttype = ENDFILE;
				strcpy_s(token.stype, 40, "end of file.");
				return token;
			}
			else
			{
				//
				// return error token
				//
				return makeError(pbuff, bufIndex);
			}
		break;
		}
	}
}

