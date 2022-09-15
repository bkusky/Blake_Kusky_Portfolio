bminor: main.o scanner.o
	gcc main.o scanner.o -o bminor

scanner.o: scanner.c
	gcc scanner.c -c -o scanner.o -g

main.o: main.c
	gcc main.c -c -o main.o -g

scanner.c: scanner.flex
	flex -o scanner.c scanner.flex

clean:
	rm -f scanner.o
	rm -f scanner.c
	rm -f main.o
	rm -f bminor 
