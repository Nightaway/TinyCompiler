#ifndef __SCANNER_H__
#define __SCANNER_H__

#include "Globals.h"

namespace tiny {
namespace internal {

#define TOKEN_LIST(T, K)												\
	/* End of source indicator. */										\
	T(EOS, "EOS", 0)															\
																						\
	/* Binary Op */															\
	T(ADD, "+", 12)															\
	T(SUB,  "-",  12)															\
	T(MUL, "*",  12)															\
	T(DIV,   "/",  12)															\
																						\
	/* Compare Op */                                                       \
	T(EQ, "==",  9)																\
	T(LT,  "<"  ,  10)															\
	T(GT, ">"  ,  10)															\
	T(LTE, "<=", 10)															\
	T(GTE, ">=", 10)															\
																						\
	/* Keywords */																\
	K(IF, "if", 0)																	\
	K(THEN, "then", 0)														\
	K(ELSE, "else", 0)															\
	K(END, "end", 0)															\
																						\
	/* Internal use	*/															\
	T(WHITESPACE, NULL, 0)

class Token {
	public:
#define T(name, string, precedence) name,
		enum Value {
			TOKEN_LIST(T, T)
			NUM_TOKENS
		};
#undef T

 private:
  static const char* const name_[NUM_TOKENS];
  static const char* const string_[NUM_TOKENS];
  static const int8_t precedence_[NUM_TOKENS];
  static const char token_type[NUM_TOKENS];
};

class Utf8CharacterStream {
public:
	Utf8CharacterStream() : pos_(0) {}
	virtual ~Utf8CharacterStream() {}

	inline utf8 Advance()
	{
		if (buffer_cursor_ < buffer_end_ || ReadBlock()) {
			pos_++;
			return static_cast<utf8>(*(buffer_cursor_));
		}

		pos_++;

		return kEndOfInput;
	}

	inline unsigned pos() const { return pos_; }

	inline unsigned SeekForward(unsigned code_unit_count)  {
		unsigned buffered_chars = 
			static_cast<unsigned>(buffer_end_ - buffer_cursor_);
		if (code_unit_count <= buffered_chars) {
			buffer_cursor_ += code_unit_count;
			pos_ += code_unit_count;
			return code_unit_count;
		}
		return SlowSeekForward(code_unit_count);
	}

	virtual void PushBack(int32_t code_unit) = 0;

protected:
	static const utf8 kEndOfInput = -1;

	virtual bool ReadBlock() = 0;
	virtual unsigned SlowSeekForward(unsigned code_unit_count) = 0;

	const utf8 *buffer_cursor_;
	const utf8 *buffer_end_;
	unsigned pos_;
};

class BufferedUtf8CharacterStream : public Utf8CharacterStream {
public:
	BufferedUtf8CharacterStream();
	virtual ~BufferedUtf8CharacterStream();

	virtual void PushBack(utf8 character);

protected:
	static const unsigned kBufferSize = 512;
	static const unsigned kPushBackStepSize = 16;

	virtual unsigned SlowSeekForward(unsigned delta);
	virtual bool ReadBlock();
	virtual void SlowPushBack(utf8 character);

	virtual unsigned BufferSeekForward(unsigned delta) = 0;
	virtual unsigned FillBuffer(unsigned position, unsigned length) = 0;

	const utf8 *pushback_limit_;
	utf8 buffer_[kBufferSize];
};

class GenericStringUtf8CharacterStream : public BufferedUtf8CharacterStream {
public:
	GenericStringUtf8CharacterStream(utf8 *data,
															 unsigned start_position,
															 unsigned end_postition);
	virtual ~GenericStringUtf8CharacterStream();

protected:
	virtual unsigned BufferSeekForward(unsigned delta);
	virtual unsigned FillBuffer(unsigned position, unsigned legnth);

	utf8 *string_;
	unsigned start_position_;
	unsigned length_;
};

};};

#endif