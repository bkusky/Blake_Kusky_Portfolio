binor: main.o function.o scanner.o parser.o decl.o stmt.o expr.o type.o param_list.o symbol.o hash_table.o scope.o scratch.o label.o
	gcc main.o function.o scanner.o parser.o decl.o stmt.o expr.o type.o param_list.o symbol.o hash_table.o scope.o scratch.o label.o -o bminor

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

symbol.o: symbol.c
	gcc symbol.c -c -o symbol.o -g

hash_table.o: hash_table.c
	gcc hash_table.c -c -o hash_table.o -g

scope.o: scope.c
	gcc scope.c -c -o scope.o -g

scratch.o: scratch.c
	gcc scratch.c -c -o scratch.o -g

label.o: label.c
	gcc label.c -c -o label.o -g

scanner.c: scanner.flex parser.c
	flex -o scanner.c scanner.flex

parser.c tokens.h: parser.bison
	bison --defines=tokens.h --output=parser.c -v parser.bison	

clean:
	@rm -f scanner.o
	@rm -f scanner.c
	@rm -f parser.c
	@rm -f parser.o
	@rm -f parser.output
	@rm -f tokens.h
	@rm -f function.o
	@rm -f main.o
	@rm -f bminor
	@rm -f decl.o
	@rm -f stmt.o
	@rm -f expr.o
	@rm -f type.o
	@rm -f param_list.o
	@rm -f symbol.o
	@rm -f hash_table.o
	@rm -f scope.o
	@rm -f scratch.o
	@rm -f label.o
	@echo "cleaning..."
