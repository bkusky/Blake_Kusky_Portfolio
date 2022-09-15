// main

#include "tokens.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyleng;

void string_clean(char * in_str);

int main(int argc, char **argv) {
	// read input
	if (argc != 3) {
		printf("incorrect number of arguments\n");
		exit(1);
	}
	if (strcmp(argv[1], "-scan")) {
		printf("incorrect arguments\n");
		exit(1);
	}
	char *file_name = argv[2];
	yyin = fopen(file_name, "r");
	if (!yyin) {
		printf("could not open %s\n", file_name);
		return 1;
	}

	while (1) {
		token_t t = yylex();
		if (t == TOKEN_EOF) break;

		//printf("token: %d	text: %s\n", t, yytext);

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
			case TOKEN_OPEN_CURLY:
				printf("OPEN_CURLY_BRACE\n");
				break;
			case TOKEN_CLOSED_CURLY:
				printf("CLOSED_CURLY_BRACE\n");
				break;
			case TOKEN_OPEN_HARD:
				printf("OPEN_HARD_BRACE\n");
				break;
			case TOKEN_CLOSED_HARD:
				printf("CLOSED_HARD_BRACE\n");
				break;
			case TOKEN_OPEN_PAREN:
				printf("OPEN_PARENTHESIS\n");
				break;
			case TOKEN_CLOSED_PAREN:
				printf("CLOSED_PARENTHESIS\n");
				break;
			case TOKEN_MCOM:
				break;
			case TOKEN_COM:
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

	return 0;
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

