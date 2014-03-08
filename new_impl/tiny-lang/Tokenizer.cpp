#include <cassert>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include "Macros.h"
#include "Tokenizer.h"

NS_USING_TINY

enum States {
	kStateInit = 0,
	kStateBlankOrTab,
	kStateEqual,
	kStateWord,
};

KeyWordDescriptor tiny::keywords[] = {
	kKeyWord_if, "if",
	kKeyWord_then, "then",
	kKeyWord_else, "else",
	kKeyWord_end, "end",
};

SeparatorDescriptor tiny::separators[] = {
	strlen("\0"), "\0",
	strlen("+"), "+",
	strlen("-"), "-",
	strlen("*"), "*",
	strlen("/"), "/",
	strlen("="), "=",
	strlen("=="), "==",
	strlen(">"), ">",
	strlen(">="), ">=",
	strlen("<"), "<",
	strlen("<="), "<="
};

static inline Token MakeBlankToken()
{
	Token t = {
		kTokenType_Blank,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		TINY_TK_BLANK
	};
	return t;
}

static inline Token MakeTabToken()
{
	Token t = {
		kTokenType_Tab,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		TINY_TK_TAB
	};
	return t;
}

static inline Token MakeNewlineToken()
{
	Token t = {
		kTokenType_Newline,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		TINY_TK_NEWLINE
	};
	return t;
}

static inline Token MakeWordToken(const char *str, unsigned len)
{
	Token t = {
		kTokenType_Word,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		"",
	};
	strncpy_s(t.strv, str, len);
	return t;
}

static inline Token MakeUnknownToken(const char *str, unsigned len)
{
	Token t = {
		kTokenType_Unknown,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		"",
	};
	strncpy_s(t.strv, str, len);
	return t;
}

static inline Token MakeAddToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_add,
		kNumUndefined,
		"+",
	};
	return t;
}

static inline Token MakeSubToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_sub,
		kNumUndefined,
		"-",
	};
	return t;
}

static inline Token MakeMulToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_mul,
		kNumUndefined,
		"*",
	};
	return t;
}

static inline Token MakeDivToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_div,
		kNumUndefined,
		"/"
	};
	return t;
}

static inline Token MakeAssignToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_assign,
		kNumUndefined,
		"="
	};
	return t;
}

static inline Token MakeEqualToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_equal,
		kNumUndefined,
		"=="
	};
	return t;
}

static inline Token MakeGreateToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_greate,
		kNumUndefined,
		">"
	};
	return t;
}

static inline Token MakeGreateThanToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_greatethan,
		kNumUndefined,
		">="
	};
	return t;
}

static inline Token MakeLessToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_less,
		kNumUndefined,
		"<"
	};
	return t;
}

static inline Token MakeLessThanToken()
{
	Token t = {
		kTokenType_Op,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_lessthan,
		kNumUndefined,
		"<="
	};
	return t;
}

static inline Token MakeEndToken()
{
	Token t = {
		kTokenType_End,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		""
	};
	return t;
}

static inline Token MakeEmptyToken()
{
	Token t = {
		kTokenType_Empty,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		""
	};
	return t;
}

static inline Token MakeErrorToken(const char *str)
{
	Token t = {
		kTokenType_Error,
		kWord_Undefined,
		kKeyWord_Undefined,
		kOperation_Undefined,
		kNumUndefined,
		""
	};
	strcpy_s(t.strv, str);
	return t;
}

static SeparatorDescriptor *isSeparator(const char *source, int &index)
{
	assert(source);

	index = 0;
	const char ch = source[0];
	const char *sp = separators[0].str;
	const unsigned len = sizeof(separators)/sizeof(separators[0]);
	for (unsigned i = 0; i < len; i++, sp = separators[i].str)
	{
		if (ch == sp[0]) 
			return &separators[i];
	}
	return NULL;
}

static Token ParseToken(char *str, unsigned len, unsigned &index)
{
	assert(str);

	char buf[255];
	unsigned buf_idx = 0;
	index = 0;
	States state = kStateInit;
	for(char ch = str[index]; index < len; ch = str[++index])
	{
		if (ch == TINY_TK_BLANK || ch == TINY_TK_TAB || ch == '\0') {
			if (state == kStateInit)
				continue;
			else
				goto ret;
		}	else if (isalnum(ch) != 0) {
			if (state == kStateInit)
				state = kStateWord;

			buf[buf_idx++] = ch;
		} else if (ch == '\n') {
			index++;
			return MakeNewlineToken();
		} else if (ch == '+') {
			index++;
			return MakeAddToken();
		} else if (ch == '-') {
			index++;
			return MakeSubToken();
		} else if (ch == '*') {
			index++;
			return MakeMulToken();
		} else if (ch == '/') {
			index++;
			return MakeDivToken();
		} else if (ch == '=') {
			int idx;
			SeparatorDescriptor *se = isSeparator(&str[index+1], idx);
			if (se == NULL) {
				index++;
				return MakeAssignToken();
			} else {
				if (strcmp(se->str, "=") == 0) {
					index += 2;
					return MakeEqualToken();
				}
			}
		} else if (ch == '>') {
			int idx;
			SeparatorDescriptor *se = isSeparator(&str[index+1], idx);
			if (se == NULL) {
				index++;
				return MakeGreateToken();
			} else {
				if (strcmp(se->str, "=") == 0) {
					index += 2;
					return MakeGreateThanToken();
				}
			} 
		} else if (ch == '<') {
			int idx;
			SeparatorDescriptor *se = isSeparator(&str[index+1], idx);
			if (se == NULL) {
				index++;
				return MakeLessToken();
			} else {
				if (strcmp(se->str, "=") == 0) {
					index += 2;
					return MakeLessThanToken();
				}
			} 
		}
	}

ret:
	if (state == kStateWord) {
		buf[buf_idx] = '\0';
		return MakeWordToken(buf, buf_idx);
	} else if (state == kStateInit) {
		buf[buf_idx] = '\0';
		return MakeEmptyToken();
	}  else {
		buf[buf_idx] = '\0';
		return MakeUnknownToken(buf, buf_idx);
	}
}

Token Tokenizer::GetToken(char *str, unsigned len, unsigned &index)
{
	assert(str);

	if (strlen(str) == 0) {
		return MakeEmptyToken();
	}

	Token tok = ParseToken(str, len, index);
	
	if (tok.type == kTokenType_Word) {
		char *strv = tok.strv;
		unsigned stridx = 0;
		if (isdigit(strv[stridx]) != 0) {
			unsigned strl = strlen(strv);
			stridx += 1;
			for (char ch = strv[stridx]; stridx < strl; ++stridx)
			{
				if (isdigit(ch) == 0)
				{
					return MakeErrorToken("Error #1 : Number can not contain any letter.");
				}
			}

			tok.kwtype    = kKeyWord_Undefined;
			tok.intv          = atoi(strv);
			tok.wordtype = kWord_Num;
			return tok;
		} else if (isalpha(strv[stridx]) != 0) {
			unsigned len = sizeof(keywords) / sizeof(keywords[0]);
			for (unsigned i=0; i<len; ++i)
			{
				if (strcmp(strv, keywords[i].str) == 0)
				{
					tok.wordtype = kWord_KeyWord;
					tok.kwtype     = keywords[i].type;
					return tok;
				}
			}

			tok.kwtype     = kKeyWord_Undefined;
			tok.intv          = kNumUndefined;
			tok.wordtype = kWord_Id;
			return tok;
		}

	} else {
			return tok;
	}

	return tok;
}