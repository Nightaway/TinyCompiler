
all : tiny

tiny :  src/tiny.cc src/token.cc src/scanner.cc
	g++ src/tiny.cc src/token.cc src/scanner.cc -o $@
clean :
	rm tiny 

.PHONY : all clean
