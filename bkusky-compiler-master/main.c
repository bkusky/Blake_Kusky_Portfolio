// main

#include "function.h"
#include "tokens.h"
#include "decl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>


extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyleng;
extern int yyparse();

bool run_scan = false;
bool run_parse = false;
bool run_print = false;
bool run_resolve = false;
bool run_typecheck = false;
bool run_codegen = false;

const char *outfile_name;
const char *in_file_name;
FILE *outfile;

int main(int argc, char **argv) {
	// read input
	// check args
	if (argc < 3 || argc > 4) {
		printf("incorrect number of arguments\n");
		usage();
		exit(1);
	}
	
	// check args
	if (!strcmp(argv[1], "-scan")) {
		run_scan = true;
	} else if (!strcmp(argv[1], "-parse")){
		run_parse = true;
	} else if (!strcmp(argv[1], "-print")) {
		run_print = true;
	} else if (!strcmp(argv[1], "-resolve")) {
		run_resolve = true;
	} else if (!strcmp(argv[1], "-typecheck")) {
		run_typecheck = true;
	} else if (!strcmp(argv[1], "-codegen")) {
		run_codegen = true;
		if (argc != 4) {
			printf("incorrect arguments for codegen\n");
			usage();
			exit(1);
		}
		in_file_name = argv[2];
		outfile_name = argv[3];

		outfile = fopen(outfile_name, "w");

		if (!outfile) {
			printf("error: %s\n", strerror(errno));
			exit(1);
		}
	} else {
		usage(1);
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
		int res = parse_execute();

		// error
		if (res < 0) { 
			exit(1);
		}
	}

	if (run_print) {
		int print_result = print_execute();

		// error
		if (print_result < 0) {
			exit(1);
		}
	}

	if (run_resolve) {
		int resolve_result = resolve_execute();

		// error
		if (resolve_result < 0) {
			exit(1);
		}
	}

	if (run_typecheck) {
		int typecheck_result = typecheck_execute();

		// error
		if (typecheck_result < 0) {
			exit(1);
		}
	}

	if (run_codegen) {
		int codegen_result = codegen_execute();

		// error
		if (codegen_result < 0) {
			exit(1);
		}
	}
	
	return 0;
}
