
all : tiny

tiny :  src/main.cc src/scanner.cc src/parser.cc src/symtab.cc src/analyzer.cc src/cgen.cc src/utility.cc src/boo-vm/vm.cc
	g++ src/main.cc src/scanner.cc src/parser.cc src/symtab.cc src/analyzer.cc src/cgen.cc src/utility.cc src/boo-vm/vm.cc -g -std=c++0x -o $@
clean :
	rm tiny 

.PHONY : all clean
