#include "Scannner.h"

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




// --------------------
// BufferedUtf8CharacterStreams

BufferedUtf8CharacterStream::BufferedUtf8CharacterStream()
	: Utf8CharacterStream(),
	  pushback_limit_(NULL) {
	  buffer_cursor_ = buffer_;
	  buffer_end_     = buffer_;
}

BufferedUtf8CharacterStream::~BufferedUtf8CharacterStream() {}

void BufferedUtf8CharacterStream::PushBack(utf8 character) {
	if (character == kEndOfInput) {
		pos_--;
		return;
	}
	if (pushback_limit_ == NULL && buffer_cursor_ > buffer_) {
		buffer_[--buffer_cursor_ - buffer_] = static_cast<utf8>(character);
		pos_--;
		return;
	}
	SlowPushBack(static_cast<utf8>(character));
}

};};