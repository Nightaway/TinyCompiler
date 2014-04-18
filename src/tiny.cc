#include "checks.h"
#include "tinystdint.h"
#include "token.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	tiny::internal::Token tok;
	printf("NUMS_TOKEN:%d\n", tiny::internal::Token::NUM_TOKENS);
	return 0;
}
