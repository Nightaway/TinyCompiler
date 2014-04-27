
all : tiny

tiny :  src/tiny.cc src/token.cc src/scanner.cc src/scanner-character-streams.cc
	g++ src/tiny.cc src/token.cc src/scanner.cc src/scanner-character-streams.cc -o $@
clean :
	rm tiny 

.PHONY : all clean
