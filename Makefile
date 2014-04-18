
all : tiny

tiny :  src/tiny.cc src/token.cc
	g++ src/tiny.cc src/token.cc -o $@
clean :
	rm tiny 

.PHONY : all clean
