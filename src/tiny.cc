#include "checks.h"
#include "tinystdint.h"
#include "token.h"
#include "scanner.h"

#include <stdio.h>

const char *source1 = "var i=1; var j=2; var result = i+j; print result;";

int main(int argc, char *argv[])
{
	tiny::internal::Utf8CharacterStream stream(source1);
	tiny::internal::Scanner scanner(&stream);
	tiny::internal::Token::Value tok = scanner.Next();
	printf("%s\n", tiny::internal::Token::String(tok));
	tok = scanner.Next();
	printf("%s\n", tiny::internal::Token::String(tok));
	return 0;
}
