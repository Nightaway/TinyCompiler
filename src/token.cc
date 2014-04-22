#include "checks.h"
#include "tinystdint.h"
#include "token.h"

namespace tiny {
	namespace internal {

#define T(name, string, precedence) #name,
const char* const Token::name_[NUM_TOKENS] = {
  TOKEN_LIST(T, T)
};
#undef T

#define T(name, string, precedence) string,
const char* const Token::string_[NUM_TOKENS] = {
  TOKEN_LIST(T, T)
};
#undef T

#define T(name, string, precedence) precedence,
const int8_t Token::precedence_[NUM_TOKENS] = {
  TOKEN_LIST(T, T)
};
#undef T

#define KT(a, b, c) 'T',
#define KK(a, b, c) 'K',
const char Token::token_type[] = {
  TOKEN_LIST(KT, KK)
};
#undef KT
#undef KK

	};
};
