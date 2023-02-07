project_two: main.o scanner.o parser.o printer.o semantic.o tree.o
	gcc -o project_two main.o scanner.o parser.o printer.o semantic.o tree.o
main.o: main.c
	gcc -c main.c
scanner.o: scanner.c scanner.h
	gcc -c scanner.c
parser.o: parser.c parser.h
	gcc -c parser.c
printer.o: printer.c printer.h
	gcc -c printer.c
semantic.o: semantic.c semantic.h
	gcc -c semantic.c
tree.o: tree.h
	gcc -c tree.c