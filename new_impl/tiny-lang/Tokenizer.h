#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#define TINY_ID_MAX 24
#define TINY_KW_MAX 5

#define TINY_TK_BLANK ' '
#define TINY_TK_TAB     '	'
#define TINY_TK_NEWLINE '\n'

#define kStrUndefined ""

NS_TINY
	
	enum {
		kNumUndefined = -1
	};

	enum TokenType {
		kTokenType_Undefined = -1,
		kTokenType_Word = 0,
		kTokenType_Op,
		kTokenType_Blank,
		kTokenType_Tab,
		kTokenType_Newline,
		kTokenType_Error,
		kTokenType_Empty,
		kTokenType_End,
		kTokenType_Unknown
	};

	enum Word {
		kWord_Undefined = -1,
		kWord_KeyWord = 0,
		kWord_Id,
		kWord_Num,
	};

	enum KeyWord {
		kKeyWord_Undefined = -1,
		kKeyWord_if = 0,
		kKeyWord_then,
		kKeyWord_else,
		kKeyWord_end,
		kKeyWord_repeat,
		kKeyWord_until
	} ;

	struct KeyWordDescriptor {
		KeyWord type;
		const char *str;
	};
	extern KeyWordDescriptor keywords[];

	enum Op {
		kOperation_Undefined = -1,
		kOperation_add = 0,
		kOperation_sub,
		kOperation_mul,
		kOperation_div,
		kOperation_assign,
		kOperation_equal,
		kOperation_greate,
		kOperation_greatethan,
		kOperation_less,
		kOperation_lessthan,
	};

	struct SeparatorDescriptor {
		unsigned len;
		const char *str;
	};
	extern SeparatorDescriptor separators[];

	struct Token {
		TokenType type;
		Word          wordtype;
		KeyWord	   kwtype;
		Op			   optype;

		int intv;
		char strv[TINY_ID_MAX];
	};

	class Tokenizer {

	public:
		Tokenizer() {}
		~Tokenizer() {}

		static Token GetToken(char *str, unsigned len, unsigned &index);
	};

NS_END


#endif