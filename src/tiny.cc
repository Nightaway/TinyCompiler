#include "checks.h"
#include "tinystdint.h"
#include "token.h"
#include "scanner.h"

#include <stdio.h>

const char *source1 = "var i=1; var j=2; var result = i+j; print result;";

int main(int argc, char *argv[])
{
	tiny::internal::Utf8CharacterStream stream(source1);
	return 0;
}
