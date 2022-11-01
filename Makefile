bminor: main.o function.o scanner.o parser.o decl.o stmt.o expr.o type.o param_list.o
	gcc main.o function.o scanner.o parser.o decl.o stmt.o expr.o type.o param_list.o -o bminor

main.o: main.c parser.c
	gcc main.c -c -o main.o -g

function.o: function.c parser.c
	gcc function.c -c -o function.o -g 

scanner.o: scanner.c
	gcc scanner.c -c -o scanner.o -g

parser.o: parser.c
	gcc parser.c -c -o parser.o -g

decl.o: decl.c
	gcc decl.c -c -o decl.o -g

stmt.o: stmt.c
	gcc stmt.c -c -o stmt.o -g

expr.o:	expr.c
	gcc expr.c -c -o expr.o -g

type.o: type.c
	gcc type.c -c -o type.o -g

param_list.o: param_list.c
	gcc param_list.c -c -o param_list.o -g

scanner.c: scanner.flex parser.c
	flex -o scanner.c scanner.flex

parser.c tokens.h: parser.bison
	bison --defines=tokens.h --output=parser.c -v parser.bison


	

clean:
	rm -f scanner.o
	rm -f scanner.c
	rm -f parser.c
	rm -f parser.o
	rm -f parser.output
	rm -f tokens.h
	rm -f function.o
	rm -f main.o
	rm -f bminor
	rm -f decl.o
	rm -f stmt.o
	rm -f expr.o
	rm -f type.o
	rm -f param_list.o
