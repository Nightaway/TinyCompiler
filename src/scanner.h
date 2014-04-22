#ifndef __SCANNER_H__
#define __SCANNER_H__

namespace tiny {
	namespace internal {
		class Utf8CharacterStream {
	
			public:
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
				
					pos_++;
					return kEndOfInput;
 				}

				inline unsigned pos() { return pos_; }
				inline unsigned length() { return len_; }

			private:
				static const char kEndOfInput = -1;

				unsigned pos_;
				unsigned len_;
				const char *stream_;
		};

		class Scanner {
			public:
				Scanner(Utf8CharacterStream *source);
			
				Token::Value Next();
				Token::Value Curr();
		
			private:
				Token::Value next_;
				Token::Value curr_;
				Utf8CharacterStream *source_;
				char c0_;

  			// Whether there is a line terminator whitespace character after
  			// the current token, and  before the next. Does not count newlines
  			// inside multiline comments.
  			bool has_line_terminator_before_next_;

				void Advance() { c0_ = source_->Advance(); }
				void scan();

				Token::Value ScannIdOrKeyword();
		};

	};
};

#endif
