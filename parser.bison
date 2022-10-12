/* 
declare tokens
*/

%token	TOKEN_EOF
%token	TOKEN_ARRAY
%token	TOKEN_AUTO
%token	TOKEN_BOOL
%token	TOKEN_CHAR
%token	TOKEN_ELSE
%token	TOKEN_FALSE
%token	TOKEN_FOR
%token  TOKEN_FUNC
%token	TOKEN_IF
%token	TOKEN_INT
%token	TOKEN_PRINT
%token	TOKEN_RETURN
%token	TOKEN_STR
%token	TOKEN_TRUE
%token	TOKEN_VOID
%token	TOKEN_WHILE
%token	TOKEN_IDENT
%token	TOKEN_NUMBER
%token	TOKEN_CHAR_LIT
%token	TOKEN_STR_LIT
%token	TOKEN_COLON
%token	TOKEN_PST_FIX_INC
%token	TOKEN_PST_FIX_DEC
%token	TOKEN_NOT
%token	TOKEN_EXPO
%token	TOKEN_MULT
%token	TOKEN_DIV
%token	TOKEN_MOD
%token	TOKEN_ADD
%token	TOKEN_SUB
%token	TOKEN_LT
%token	TOKEN_LTE
%token	TOKEN_GT
%token	TOKEN_GTE
%token	TOKEN_EQ
%token	TOKEN_NEQ
%token	TOKEN_AND
%token	TOKEN_OR
%token	TOKEN_ASSIGN
%token	TOKEN_SEMICOLON
%token	TOKEN_LBRACE
%token	TOKEN_RBRACE
%token	TOKEN_LBRACKET
%token	TOKEN_RBRACKET
%token	TOKEN_LPAREN
%token	TOKEN_RPAREN
%token	TOKEN_COMMA
%token	TOKEN_QUESTION
%token	TOKEN_ERROR

%{
	#include <stdio.h>

	extern char *yytext;
	extern int yylex();
	extern int yyerror( char *str );
%}

%%

program		: decl_list		{ return 0; }
			| TOKEN_ERROR	{ return -1; } 
			;

decl_list	: decl TOKEN_SEMICOLON decl_list
			| function_def function_impl decl_list
			| function_def TOKEN_SEMICOLON
			| /* epsilon */
			;

function_impl	: TOKEN_ASSIGN TOKEN_LBRACE multistmt TOKEN_RBRACE
				;

function_def	:  TOKEN_IDENT TOKEN_COLON TOKEN_FUNC type TOKEN_LPAREN param_list TOKEN_RPAREN
				;

block	: TOKEN_LBRACE multistmt TOKEN_RBRACE
		;

				
multistmt 	: stmt_list
			| /* epsilon */
			;


stmt_list	: stmt stmt_list
			| stmt
			| expr_semi stmt_list
			| expr_semi
			;

stmt	: decl TOKEN_SEMICOLON
		| for_loop
		| if_stmt
		| stmt2
		;

expr_semi	: expr TOKEN_SEMICOLON
			;

stmt2		: TOKEN_RETURN expr_maybe TOKEN_SEMICOLON
			| TOKEN_PRINT expr_list TOKEN_SEMICOLON
			| block
			;


for_loop	: TOKEN_FOR TOKEN_LPAREN for_list TOKEN_RPAREN stmt
			;

if_stmt		: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt
			| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_m TOKEN_ELSE stmt
			;

if_m		: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_m TOKEN_ELSE if_m
			| TOKEN_FOR TOKEN_LPAREN for_list TOKEN_RPAREN if_m
			| stmt2
			;

for_list	: expr_maybe TOKEN_SEMICOLON expr_maybe TOKEN_SEMICOLON expr_maybe
			;

decl	: TOKEN_IDENT TOKEN_COLON type
		| TOKEN_IDENT TOKEN_COLON type TOKEN_ASSIGN expr
		;

type	: TOKEN_INT
		| TOKEN_CHAR
		| TOKEN_STR
		| TOKEN_BOOL
		| TOKEN_AUTO
		| TOKEN_VOID
		| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_NUMBER TOKEN_RBRACKET type
		| function_def
		;

param_list 	: param_comma
			| /* epsilon */
			;

param_comma	: param TOKEN_COMMA param_comma
			| param
			;

param	: TOKEN_IDENT TOKEN_COLON param_type
		;

param_type	: TOKEN_INT
			| TOKEN_CHAR
			| TOKEN_STR
			| TOKEN_BOOL
			| TOKEN_AUTO
			| TOKEN_VOID
			| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET param_type
			| TOKEN_FUNC type TOKEN_LPAREN param_list TOKEN_RPAREN
			;

expr_list	: expr_comma
			| /* epsilon */
			;

expr_comma	: expr TOKEN_COMMA expr_comma
			| expr
			;

expr_maybe	: expr
			| /* epsilon */

expr	: expr TOKEN_ASSIGN term
		| ternary
		| term
		;
	
ternary_item	: ternary
				| term
				;

ternary	:  ternary_item TOKEN_QUESTION ternary_item TOKEN_COLON term
		;

term	: term TOKEN_OR term0
		| term0
		;

term0	: term0 TOKEN_AND term1
		| term1
		;

term1	: term1 TOKEN_EQ term2
		| term1 TOKEN_LT term2
		| term1 TOKEN_LTE term2
		| term1 TOKEN_GT term2
		| term1 TOKEN_GTE term2
		| term1 TOKEN_NEQ term2
		| term2
		;

term2	: term2 TOKEN_ADD term3
		| term2 TOKEN_SUB term3
		| term3
		;

term3	: term3 TOKEN_MULT term4
		| term3 TOKEN_DIV term4
		| term3 TOKEN_MOD term4
		| term4 
		;

term4	: term4 TOKEN_EXPO term5
		| term5
		;

term5	: term5 TOKEN_PST_FIX_INC
		| term5 TOKEN_PST_FIX_DEC
		| term_array
		;

term_array	: TOKEN_LBRACE factor_list TOKEN_RBRACE
			| term_paren
			;
		
term_paren	: term_array_index
			| TOKEN_LPAREN expr TOKEN_RPAREN
			;

term_array_index 	: TOKEN_IDENT TOKEN_LBRACKET expr TOKEN_RBRACKET
					| factor
					;

factor  : TOKEN_SUB factor
		| TOKEN_NOT factor
		| TOKEN_NUMBER
		| TOKEN_IDENT
		| TOKEN_CHAR_LIT
		| TOKEN_STR_LIT
		| TOKEN_TRUE
		| TOKEN_FALSE
		| function_call
		;

factor_list		: factor_comma
				;

factor_comma	: factor TOKEN_COMMA factor_comma
				| factor
				| array_list
				;

array_list	: TOKEN_LBRACE factor_list TOKEN_RBRACE
			;

function_call 	: TOKEN_IDENT TOKEN_LPAREN expr_maybe TOKEN_RPAREN
				;


%%

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}
