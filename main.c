// main

#include "function.h"
#include "tokens.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyleng;
extern int yyparse();

bool run_scan = false;
bool run_parse = false;

int main(int argc, char **argv) {
	// read input
	// check args
	if (argc != 3) {
		printf("incorrect number of arguments\n");
		exit(1);
	}
	
	// check args
	if (!strcmp(argv[1], "-scan")) {
		run_scan = true;
	} else if (!strcmp(argv[1], "-parse")){
		run_parse = true;
	} else {
		printf("incorrect arguments\n");
		exit(1);
	}
	
	// set yyin to filename
	char *file_name = argv[2];
	yyin = fopen(file_name, "r");
	if (!yyin) {
		printf("could not open %s\n", file_name);
		return 1;
	}

	if (run_scan) {
		scan_execute();
	}

	if (run_parse) {
		int p = yyparse();
		if(p==0) {
			printf("Parse successful!\n");
			return 0;
		} else if (p == -1) {
			printf("Scan Error\n");
			return 1;
		} else {
			printf("Parse failed.\n");
			return 1;
		}
	}

	return 0;
}
