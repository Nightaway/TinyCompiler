#ifndef TINY_SCANNER_CHARACTER_STREAM_H
#define TINY_SCANNER_CHARACTER_STREAM_H

#include "scanner.h"

namespace tiny {
namespace internal {

    // A buffered character stream based on a random access character
    // source (ReadBlock can be called with pos_ pointing to any position,
    // even positions before the current).
    class BufferedUtf8CharacterStream : public Utf8CharacterStream {
      public:
        BufferedUtf8CharacterStream();
        virtual ~BufferedUtf8CharacterStream();
      
        virtual void PushBack(char character);

      protected:
         static const unsigned kBufferSize = 512;
         static const unsigned kPushBackStepSize = 16;

         virtual unsigned SlowSeekForward(unsigned delta);
         virtual bool ReadBlock();
         virtual void SlowPushBack(char character);

         virtual unsigned BufferSeekForward(unsigned delta) = 0;
         virtual unsigned FillBuffer(unsigned position, unsigned length) = 0;

         const char* pushback_limit_;
         char buffer_[kBufferSize];
    };


  // Generic string stream.
  class GenericStringUtf8CharacterStream: public BufferedUtf8CharacterStream {
    public:
      GenericStringUtf8CharacterStream(const char *data,
                                       unsigned start_position,
                                       unsigned end_position);
      virtual ~GenericStringUtf8CharacterStream();

    protected:
      virtual unsigned BufferSeekForward(unsigned delta);
      virtual unsigned FillBuffer(unsigned position, unsigned length);

      const char *string_;
      unsigned start_position_;
      unsigned length_;
    };

 } } // namespace tiny::internal

#endif
