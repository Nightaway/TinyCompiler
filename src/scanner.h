#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>

namespace tiny {
namespace internal {
		class Utf8CharacterStream {
	
			public:
        Utf8CharacterStream() : pos_(0) {}
				Utf8CharacterStream(const char *stream) 
																							: pos_(0), 
																								len_(strlen(stream)), 
																								stream_(stream) 
				{

				}

				inline char Advance()
				{
					if (pos_ < len_) {
						return stream_[pos_++];
					}
				
					return kEndOfInput;
 				}
	
				inline void PushBack()
				{
					pos_--;
 				}

				inline unsigned pos() { return pos_; }
				inline unsigned length() { return len_; }

			protected:
				static const char kEndOfInput = -1;

				unsigned pos_;
				unsigned len_;
				const char *stream_;

				const char *buffer_cursor_;
				const char *buffer_end_;
		};

		class Scanner {
			public:
				Scanner();
				void Initialize(Utf8CharacterStream* source);

				Token::Value Next();
				Token::Value Curr();

				std::string GetLiteralNext() { return literalNext_; }
		
			private:
				Token::Value next_;
				Token::Value curr_;
				Utf8CharacterStream *source_;
				char c0_;
				std::string literalNext_;
				std::string literalCurr_;

  			// Whether there is a line terminator whitespace character after
  			// the current token, and  before the next. Does not count newlines
  			// inside multiline comments.
  			bool has_line_terminator_before_next_;

				void Advance() { c0_ = source_->Advance(); }
				void PushBack() { source_->PushBack(); }
				void scan();

				Token::Value ScannIdOrKeyword();
		};

}}

#endif
