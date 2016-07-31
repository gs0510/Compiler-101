# Compiler-101

This is a toy compiler written in C.

Known Issues: Crashes with GCC 5.3 (ships with Ubuntu 16.04)

Tested with GCC 4.9 and below. It works perfectly for them! The compiler follows the language given in grammar.txt.

Running Instructions

$ make #compile the compiler
$ ./compiler101 <filename> #compiles the code
$ nasm -f elf64 code.asm #converts the code.asm to machine code and executes
$ gcc code.o
$ ./a.out 
