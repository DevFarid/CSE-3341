project_three: main.o scanner.o parser.o printer.o semantic.o execute.o memory.o
	gcc -o project_three main.o scanner.o parser.o printer.o semantic.o execute.o memory.o
main.o: main.c tree.h
	gcc -c main.c
scanner.o: scanner.c scanner.h
	gcc -c scanner.c
parser.o: parser.c parser.h tree.h
	gcc -c parser.c
printer.o: printer.c printer.h tree.h
	gcc -c printer.c
semantic.o: semantic.c semantic.h tree.h
	gcc -c semantic.c
execute.o: execute.c execute.h
	gcc -c execute.c
memory.o: memory.c memory.h
	gcc -c memory.c