
all : tiny

tiny :  src/tiny.cc src/token.cc src/scanner.cc src/scanner-character-streams.cc src/ast.cc src/parser.cc
	g++ src/tiny.cc src/token.cc src/scanner.cc src/scanner-character-streams.cc src/ast.cc src/parser.cc -o $@
clean :
	rm tiny 

.PHONY : all clean
