#include "checks.h"
#include "tinystdint.h"
#include "token.h"

#include "scanner-character-streams.h"

namespace tiny {
namespace internal {

BufferedUtf8CharacterStream::BufferedUtf8CharacterStream()
    : Utf8CharacterStream(),
      pushback_limit_(NULL) {
  // Initialize buffer as being empty. First read will fill the buffer.
  buffer_cursor_ = buffer_;
  buffer_end_ = buffer_;
}

BufferedUtf8CharacterStream::~BufferedUtf8CharacterStream() {}

void BufferedUtf8CharacterStream::PushBack(char character)
{

}

unsigned BufferedUtf8CharacterStream::SlowSeekForward(unsigned delta)
{
}

bool BufferedUtf8CharacterStream::ReadBlock()
{
}

void BufferedUtf8CharacterStream::SlowPushBack(char character)
{
}

}}
