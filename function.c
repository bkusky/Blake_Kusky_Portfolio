// function.c
// helper file for main.c

#include "function.h"
#include "tokens.h"
#include "decl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int yylex();
extern char *yytext;
extern int yyleng;
extern struct decl *ast;

void usage() {
	printf("bminor <option> <sourcefile>\n");
	printf("options:\n");
	printf("\t-scan:		scan <sourcefile>\n");
	printf("\t-parse:		scan --> parse <sourcefile>\n");
	printf("\t-print:		scan --> parse --> print <sourcefile>\n");
	printf("\t-resolve: 	scan --> parse --> resolve <sourcefile>\n");
	printf("\t-typecheck:	scan --> parse --> resolve --> typecheck <sourcefile>\n");
}

void scan_execute() {
	while (1) {
		enum yytokentype t = yylex();
		
		// yylex() returns 0 for eof
		if (t == 0) break;


		switch (t) {
			case TOKEN_ARRAY:
				printf("ARRAY\n");
				break;
			case TOKEN_AUTO:
				printf("AUTO\n");
				break;
			case TOKEN_BOOL:
				printf("BOOL\n");
				break;
			case TOKEN_CHAR:
				printf("CHAR\n");
				break;
			case TOKEN_ELSE:
				printf("ELSE\n");
				break;
			case TOKEN_FALSE:
				printf("FALSE\n");
				break;
			case TOKEN_FOR:
				printf("FOR\n");
				break;
			case TOKEN_FUNC:
				printf("FUNCTION\n");
				break;
			case TOKEN_IF:
				printf("IF\n");
				break;
			case TOKEN_INT:
				printf("INTEGER\n");
				break;
			case TOKEN_PRINT:
				printf("PRINT\n");
				break;
			case TOKEN_RETURN:
				printf("RETURN\n");
				break;
			case TOKEN_STR:
				printf("STRING\n");
				break;
			case TOKEN_TRUE:
				printf("TRUE\n");
				break;
			case TOKEN_VOID:
				printf("VOID\n");
				break;
			case TOKEN_WHILE:
				printf("WHILE\n");
				break;
			case TOKEN_IDENT:
				// print the identifier
				printf("IDENTIFIER %s\n", yytext);
				break;
			case TOKEN_NUMBER:
				// print number
				printf("INTEGER_LITERAL %s\n", yytext);
				break;
			case TOKEN_CHAR_LIT:;
				// print char
				char *chl = strdup(yytext);
				printf("CHARACTER_LITERAL ");
				if (yyleng != 3) {
					char c = chl[2];
					switch(c) {
						case 'n':
							printf("\n\n");
							break;
						case '0':
							printf("%c\n", c);
							break;
						default:
							printf("%c\n", c);
							break;
					}
				} else {
					printf("%c\n", chl[1]);
				}
				free(chl);
				break;
			case TOKEN_STR_LIT:;
				// print string
				char * in_str = malloc(yyleng-2);
				string_clean(in_str);
				printf("STRING_LITERAL %s\n", in_str);
				free(in_str);
				break;
			case TOKEN_PST_FIX_INC:
				printf("POSTFIX_INCREMENT\n");
				break;
			case TOKEN_PST_FIX_DEC:
				printf("POSTFIX_DECREMENT\n");
				break;
			case TOKEN_NOT:
				printf("NOT\n");
				break;
			case TOKEN_EXPO:
				printf("EXPONENT\n");
				break;
			case TOKEN_MULT:
				printf("MULTIPLY\n");
				break;
			case TOKEN_DIV:
				printf("DIVIDE\n");
				break;
			case TOKEN_MOD:
				printf("MODULO\n");
				break;
			case TOKEN_ADD:
				printf("ADD\n");
				break;
			case TOKEN_SUB:
				printf("SUBTRACT\n");
				break;
			case TOKEN_LT:
				printf("LESS_THAN\n");
				break;
			case TOKEN_LTE:
				printf("LESS_THEN_OR_EQUAL_TO\n");
				break;
			case TOKEN_GT:
				printf("GREATER_THAN\n");
				break;
			case TOKEN_GTE:
				printf("GREATER_THAN_OR_EQUAL_TO\n");
				break;
			case TOKEN_EQ:
				printf("EQUAL\n");
				break;
			case TOKEN_NEQ:
				printf("TOKEN_NEQ\n");
				break;
			case TOKEN_AND:
				printf("AND\n");
				break;
			case TOKEN_OR:
				printf("OR\n");
				break;
			case TOKEN_ASSIGN:
				printf("ASSIGNMENT\n");
				break;
			case TOKEN_LBRACE:
				printf("OPEN_BRACE\n");
				break;
			case TOKEN_RBRACE:
				printf("CLOSED_BRACE\n");
				break;
			case TOKEN_LBRACKET:
				printf("OPEN_BRACKET\n");
				break;
			case TOKEN_RBRACKET:
				printf("CLOSED_BRACE\n");
				break;
			case TOKEN_LPAREN:
				printf("OPEN_PARENTHESIS\n");
				break;
			case TOKEN_RPAREN:
				printf("CLOSED_PARENTHESIS\n");
				break;
			case TOKEN_COLON:
				printf("COLON\n");
				break;
			case TOKEN_SEMICOLON:
				printf("SEMICOLON\n");
				break;
			case TOKEN_COMMA:
				printf("COMMA\n");
				break;
			case TOKEN_QUESTION:
				printf("QUESTION_MARK\n");
				break;
			case TOKEN_ERROR:
				printf("ERROR\n");
				exit(1);
				break;	
		}
	}
}

void string_clean(char * in_str) {
	// remove / and " from strings
	// append \0
		
	int index = 0;
	for (int c = 1; c < yyleng-1; c++) {

		if (yytext[c] == '\\') {
			if (yytext[c+1]=='n') {
				in_str[index] = '\n';
				index++;
				c++;
				continue;
			}
			continue;
		} 
		in_str[index] = yytext[c];
		index++;
	
	}
	in_str[yyleng] = '\0';	
}

int parse_execute() {

	int res = yyparse();
	if (res == 0) {
		printf("Parse successful!\n");
		return 1;
	} else {
		printf("Parse failed.\n");
		return -1;
	}
}

int print_execute() {
	int res = yyparse();
	
	if (res != 0) {
		printf("Parse error: parse failed\n");
		return -1;
	}

	// if we have a valid ast we can print it
	if(!ast) {
		printf("invalid AST\n");
		return -1;
	}

	// print ast
	decl_print(ast, 0);

	return 0;
} 

void indent_print(int indent) {

	for (int i = 0; i < indent; i++) {		
		printf("    ");
	}
}

int resolve_execute() {
	int res = yyparse();

	if (res != 0) {
		printf("Parse error: parse failed\n");
		return -1;
	}	
	
	// check ast 
	if(!ast) {
		printf("invalid AST\n");
		return -1;
	}

	// resolve
	return 0;
}
