#include "checks.h"
#include "tinystdint.h"
#include "token.h"
#include "scanner.h"

#include <stdio.h>

const char *source1 = "var i=1; var j=2; var result = i+j; print result;";

int main(int argc, char *argv[])
{
  tiny::internal::Utf8CharacterStream stream(source1);
  tiny::internal::Scanner scanner;
  scanner.Initialize(&stream);
  tiny::internal::Token::Value tok = scanner.Next();
  while (tok != tiny::internal::Token::EOS) {
    printf("%s\n", tiny::internal::Token::String(tok));
    if (tok == tiny::internal::Token::ID || tok == tiny::internal::Token::NUMBER) {
      std::string id = scanner.GetLiteralNext();
      printf("literal:%s\n", id.c_str());
    }
    tok = scanner.Next();
  }

  return 0;
}
