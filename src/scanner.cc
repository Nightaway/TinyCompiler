#include "checks.h"
#include "tinystdint.h"
#include "token.h"

#include "scanner.h"

#include <ctype.h>

namespace tiny {
namespace internal {

Scanner::Scanner() 
{
	
}

void Scanner::Initialize(Utf8CharacterStream* source)
{
	source_ = source;
}

Token::Value Scanner::Next()
{
	Advance();
	scan();
	return next_;
}

void Scanner::scan()
{
	Token::Value token;
	do {
	switch(c0_) {
		case ' '  :
		case '\t' :
	        Advance();
        	token = Token::WHITESPACE;
		break;

		case '\n' :
					Advance();
					has_line_terminator_before_next_ = true;
					token = Token::WHITESPACE;
		break;

		case ';' :
					//Advance();
					token = Token::SEMICOLON;
		break;

		case '(' :
					//Advance();
					token = Token::LPAREN;
		break;

		case ')' :
					//Advance();
					token = Token::RPAREN;
		break;

		case '=' :
					//Advance();
					token = Token::ASSIGN;
		break;

		case '+' :
					//Advance();
					token = Token::ADD;
		break;
		default:
				if (isalpha(c0_)) {
					token = ScannIdOrKeyword();
				} if (isdigit(c0_)) {
					token = ScanNumber();
				} else if (c0_ < 0) {
					token = Token::EOS;
				}
					
		break;
	} 
	} while (token == Token::WHITESPACE);
	next_ = token;
}

Token::Value Scanner::ScanNumber()
{
		int idx = 0;
		char buf[255];
		Token::Value token;
		while (isdigit(c0_)) {
			buf[idx++] = c0_;
			Advance();
		}
		buf[idx] = '\0';
		PushBack();

	  literalNext_ = std::string(buf, strlen(buf));
		token = Token::NUMBER;
		return token;
}

Token::Value Scanner::ScannIdOrKeyword()
{
		int idx = 0;
		char buf[255];
		Token::Value token;
		while (isalpha(c0_)) {
			buf[idx++] = c0_;
			Advance();
		}
		buf[idx] = '\0';
		PushBack();

		if (strcmp(buf, "var") == 0) {
				token = Token::VAR;
				return token;
		}
		literalNext_ = std::string(buf, strlen(buf));
		token = Token::ID;
		return token;
}

}}
