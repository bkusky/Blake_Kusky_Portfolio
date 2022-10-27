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

decl_list	: decl decl_list 
			| /* epsilon */
			;

decl 	: def_start subdecl TOKEN_ASSIGN assign_expr TOKEN_SEMICOLON
		| def_start subdecl TOKEN_SEMICOLON
		| def_start TOKEN_FUNC type_function TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_ASSIGN block
		| def_start TOKEN_FUNC type_function TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_SEMICOLON
		;

def_start	: TOKEN_IDENT TOKEN_COLON
			;

subdecl	: multi_decl type_static
		;

multi_decl	: multi_decl TOKEN_ARRAY TOKEN_LBRACKET expr TOKEN_RBRACKET
			| /* epsilon */
			;

param_list	: param_comma
			| /* epsilon */
			;

param_comma	: param TOKEN_COMMA param_comma
			| param
			;

param	: def_start param_array type_static
		| def_start type_function
		;

param_array	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET
			| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET param_array
			;
		

type_function	: TOKEN_INT
				| TOKEN_VOID
				| TOKEN_BOOL
				| TOKEN_STR
				| TOKEN_CHAR
				;

type_static	: TOKEN_INT
			| TOKEN_STR
			| TOKEN_AUTO
			| TOKEN_CHAR
			| TOKEN_BOOL
			;

block	: TOKEN_LBRACE bunch_stmts TOKEN_RBRACE
		;

bunch_stmts	: stmt_list
			| /* epsilon */
			;

stmt_list	: stmt
			| expr TOKEN_SEMICOLON
			| stmt stmt_list
			| expr TOKEN_SEMICOLON stmt_list
			;

stmt	: decl
		| for
		| if
		| stmt_
		;

stmt_	: block
		| return
		| print
		;

return	: TOKEN_RETURN expr_maybe TOKEN_SEMICOLON
		;

print	: TOKEN_PRINT expr_maybe TOKEN_SEMICOLON
		;

for		: TOKEN_FOR TOKEN_LPAREN for_param TOKEN_RPAREN stmt
		;

for_param	: expr_maybe TOKEN_SEMICOLON expr_maybe TOKEN_SEMICOLON expr_maybe
			;

if	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_else TOKEN_ELSE stmt
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt
	;

if_else	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_else TOKEN_ELSE if_else
		| stmt_
		| TOKEN_FOR TOKEN_LPAREN for_param TOKEN_RPAREN if_else
		;

assign_expr	: term0
			| multi_array
			;

multi_array	: TOKEN_LBRACE literal_list TOKEN_RBRACE
			;

expr_maybe	: expr_comma
			| /* epsilon */
			;

expr_comma	: expr TOKEN_COMMA expr_comma
			| expr
			;

expr	: expr_start TOKEN_ASSIGN term0
		| TOKEN_NOT term0
		| term0
		| tern
		;

expr_start	: TOKEN_IDENT array_index
			;

term0	: term0 TOKEN_OR term1
		| term1
		;

term1	: term1 TOKEN_AND term2
		| term2
		;
		
term2	: term2 TOKEN_LT term3
		| term2 TOKEN_LTE term3
		| term2 TOKEN_GT term3
		| term2 TOKEN_GTE term3
		| term2 TOKEN_EQ term3
		| term2 TOKEN_NEQ term3
		| term3
		;

term3	: term3 TOKEN_ADD term4
		| term3 TOKEN_SUB term4
		| term4
		;

term4	: term4 TOKEN_MULT term5
		| term4 TOKEN_DIV term5
		| term4 TOKEN_MOD term5
		| term5
		;

term5	: term5 TOKEN_EXPO term6
		| term6
		;

term6	: term6 TOKEN_PST_FIX_INC
		| term6 TOKEN_PST_FIX_DEC
		| TOKEN_SUB term7
		| term7
		;

term7	: TOKEN_LPAREN expr TOKEN_RPAREN
		| literal
		;

literal	: TOKEN_NUMBER
		| TOKEN_CHAR_LIT
		| TOKEN_STR_LIT
		| TOKEN_TRUE
		| TOKEN_FALSE
		| array_lookup
		| function_call
		;

literal_list	: literal_comma
				| /* epsilon */
				;

literal_comma	: literal TOKEN_COMMA literal_comma
				| literal
				| multi_array
				;

tern	: expr_start TOKEN_ASSIGN expr_tern TOKEN_QUESTION expr_tern TOKEN_COLON expr_tern
		;

expr_tern	: tern
			| term0
			| TOKEN_NOT term0
			;

array_lookup	: TOKEN_IDENT array_index
				;

array_index		: multi_index
				| /* epsilon */
				;

multi_index		: TOKEN_LBRACKET expr TOKEN_RBRACKET multi_index
				| TOKEN_LBRACKET expr TOKEN_RBRACKET
				;

function_call	: TOKEN_IDENT TOKEN_LPAREN expr_maybe TOKEN_RPAREN
				;
			

%%

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}
