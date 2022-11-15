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
	#include <string.h>
	#include "decl.h"
	#include "param_list.h"

	struct decl *ast;

	extern char *yytext;
	extern int yylex();
	extern int yyerror( char *str );
%}

%union {
	struct decl *decl;
	struct stmt *stmt;
	struct expr *expr;
	struct type *type;
	struct param_list *param_list;
};

%type <decl> program decl_list decl
%type <stmt> block bunch_stmts stmt_list stmt for if print return stmt_ for_param if_else
%type <expr> expr expr_maybe expr_comma expr_start ident assign_expr multi_array multi_array_list literal_list literal_comma tern expr_tern array_lookup function_call array_index multi_index term0 term1 term2 term3 term4 term5 term6 term7 literal 
%type <type> type_function type_static recursive_type multi_type param_array 
%type <param_list> param_list param_comma param 

%%

program		: decl_list	TOKEN_EOF	
				{ 
					ast = $1;
					return 0;
				}
			| TOKEN_ERROR	{ return -1; } 
			;

ident	: TOKEN_IDENT
			{ $$ = expr_create_name(strdup(yytext)); }
		;

decl_list	: decl decl_list 
				{	
					$1->next = $2;
					$$ = $1;
				}
			| /* epsilon */
				{ $$ = NULL; }
			;

decl 	: ident TOKEN_COLON recursive_type TOKEN_ASSIGN assign_expr TOKEN_SEMICOLON
			{ $$ = decl_create($1->name, $3, NULL, $5, NULL, NULL); }
		| ident TOKEN_COLON recursive_type TOKEN_SEMICOLON
			{ $$ = decl_create($1->name, $3, NULL, NULL, NULL, NULL); }
		| ident TOKEN_COLON TOKEN_FUNC type_function TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_ASSIGN block
			{ $$ = decl_create($1->name, type_create(TYPE_FUNCTION, $4, $6, NULL), NULL, NULL, $9, NULL); }
		| ident TOKEN_COLON TOKEN_FUNC type_function TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_SEMICOLON
			{ $$ = decl_create($1->name, type_create(TYPE_FUNCTION, $4, $6, NULL), NULL, NULL, NULL, NULL); }
		;


recursive_type	: multi_type type_static
			{ 
				if ($1 == NULL) {
					$$ = $2;
				} else { 
					struct type *t = $1;

					while(t->subtype) {
						t = t->subtype;
					}
					t->subtype = $2;
					$$ = $1;
				}
			}
		;

multi_type	: TOKEN_ARRAY TOKEN_LBRACKET expr_maybe TOKEN_RBRACKET multi_type
				{ $$ = type_create(TYPE_ARRAY, $5, NULL, $3); } 
			| /* epsilon */
				{ $$ = NULL; }
			;


param_list	: param_comma
				{ $$ = $1; }
			| /* epsilon */
				{ $$ = NULL; }
			;

param_comma	: param TOKEN_COMMA param_comma
				{ 
					$1->next = $3; 
					$$ = $1;
				}
			| param
				{ $$ = $1; }
			;

param	: ident TOKEN_COLON param_array type_static
			{ 
				$3->subtype = $4;
				$$ = param_list_create($1->name, $3, NULL);
			}
		| ident TOKEN_COLON type_function
			{ $$ = param_list_create($1->name, $3, NULL); }
		;

param_array	: TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET
				{ $$ = type_create(TYPE_ARRAY_ARG, NULL, NULL, NULL); }
			| TOKEN_ARRAY TOKEN_LBRACKET TOKEN_RBRACKET param_array
				{ $$ = type_create(TYPE_ARRAY_ARG, $4, NULL, NULL); }
			;
		

type_function	: TOKEN_INT
					{ $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL); }
				| TOKEN_VOID
					{ $$ = type_create(TYPE_VOID, NULL, NULL, NULL); }
				| TOKEN_BOOL
					{ $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
				| TOKEN_STR
					{ $$ = type_create(TYPE_STRING, NULL, NULL, NULL); }
				| TOKEN_CHAR
					{ $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL); }
				| TOKEN_AUTO
					{ $$ = type_create(TYPE_AUTO, NULL, NULL, NULL); }
				;

type_static	: TOKEN_INT
				{ $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL); }
			| TOKEN_STR
				{ $$ = type_create(TYPE_STRING, NULL, NULL, NULL); }
			| TOKEN_AUTO
				{ $$ = type_create(TYPE_AUTO, NULL, NULL, NULL); }
			| TOKEN_CHAR
				{ $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL); }
			| TOKEN_BOOL
				{ $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
			;

block	: TOKEN_LBRACE bunch_stmts TOKEN_RBRACE
			{ $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, $2, NULL, NULL); }
		;

bunch_stmts	: stmt_list
				{ $$ = $1; }	
			| /* epsilon */
				{ $$ = NULL; }
			;

stmt_list	: stmt
				{ $$ = $1; }
			| expr TOKEN_SEMICOLON
				{ $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
			| stmt stmt_list
				{ $$ = stmt_create($1->kind, $1->decl, $1->init_expr, $1->expr, $1->next_expr, $1->body, $1->else_body, $2); }
			| expr TOKEN_SEMICOLON stmt_list
				{ $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, $3); }
			;

stmt	: decl
			{ $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
		| for
			{ $$ = $1; }
		| if
			{ $$ = $1; }
		| stmt_
			{ $$ = $1; }
		;

stmt_	: block
			{ $$ = $1; }
		| return
			{ $$ = $1; }
		| print
			{ $$ = $1; }
		;

return	: TOKEN_RETURN expr_maybe TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
		;

print	: TOKEN_PRINT expr_maybe TOKEN_SEMICOLON
			{ $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
		;

for		: TOKEN_FOR TOKEN_LPAREN for_param TOKEN_RPAREN stmt
			{ 
				$$ = $3;
				$3->body = $5;
			}
		;

for_param	: expr_maybe TOKEN_SEMICOLON expr_maybe TOKEN_SEMICOLON expr_maybe
				{ $$ = stmt_create(STMT_FOR, NULL, $1, $3, $5, NULL, NULL, NULL); } 
			;

if	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_else TOKEN_ELSE stmt
		{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
	| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN stmt
		{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, NULL, NULL); }
	;

if_else	: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN if_else TOKEN_ELSE if_else
			{ $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $3, NULL, $5, $7, NULL); }
		| stmt_
			{ $$ = $1; }
		| TOKEN_FOR TOKEN_LPAREN for_param TOKEN_RPAREN if_else
			{ $$ = stmt_create(STMT_FOR, NULL, $3->init_expr, $3->expr, $3->next_expr, $5, NULL, NULL); }
		;

assign_expr	: term0
				{ $$ = $1; }
			| multi_array
				{ $$ = $1; }
			| tern
				{ $$ = $1; } 
			;

multi_array	: TOKEN_LBRACE literal_list TOKEN_RBRACE
				{ 
					struct expr *e = expr_create(EXPR_ARRAY_DEF, NULL, NULL, NULL); 
					e->array_multi_next = $2->array_multi_next;
					$$ = e;
				}
			;

expr_maybe	: expr_comma
				{ $$ = $1; }
			| /* epsilon */
				{ $$ = NULL; }
			;

expr_comma	: expr TOKEN_COMMA expr_comma
				{
					$1->next = $3;
					$$ = $1;
				}
			| expr
				{ $$ = $1; }
			;

expr	: expr_start TOKEN_ASSIGN expr
			{ $$ = expr_create(EXPR_ASSIGN, $1, $3, NULL); }
		/* | TOKEN_NOT term0
			{ $$ = expr_create(EXPR_NOT, NULL, $2, NULL); } */
		| term0
			{ $$ = $1; }
		| tern
			{ $$ = $1; } 
		;

expr_start	: ident array_index
				{ 
					if ($2) {
						$2->left = $1;
				 		$$ = $2;
					} else {
						$$ = $1;
					}
				}	
			;

term0	: term0 TOKEN_OR term1
			{ $$ = expr_create(EXPR_OR, $1, $3, NULL); }
		| term1
			{ $$ = $1; }
		;

term1	: term1 TOKEN_AND term2
			{ $$ = expr_create(EXPR_AND, $1, $3, NULL); }
		| term2
			{ $$ = $1; }
		;
		
term2	: term2 TOKEN_LT term3
			{ $$ = expr_create(EXPR_LT, $1, $3, NULL); }
		| term2 TOKEN_LTE term3
			{ $$ = expr_create(EXPR_LTE, $1, $3, NULL); }
		| term2 TOKEN_GT term3
			{ $$ = expr_create(EXPR_GT, $1, $3, NULL); }
		| term2 TOKEN_GTE term3
			{ $$ = expr_create(EXPR_GTE, $1, $3, NULL); }
		| term2 TOKEN_EQ term3
			{ $$ = expr_create(EXPR_EQ, $1, $3, NULL); }
		| term2 TOKEN_NEQ term3
			{ $$ = expr_create(EXPR_NEQ, $1, $3, NULL); }
		| term3
			{ $$ = $1; }
		;

term3	: term3 TOKEN_ADD term4
			{ $$ = expr_create(EXPR_ADD, $1, $3, NULL); }
		| term3 TOKEN_SUB term4
			{ $$ = expr_create(EXPR_SUB, $1, $3, NULL); }
		| term4
			{ $$ = $1; }
		;

term4	: term4 TOKEN_MULT term5
			{ $$ = expr_create(EXPR_MULT, $1, $3, NULL); }
		| term4 TOKEN_DIV term5
			{ $$ = expr_create(EXPR_DIV, $1, $3, NULL); }
		| term4 TOKEN_MOD term5
			{ $$ = expr_create(EXPR_MOD, $1, $3, NULL); }
		| term5
			{ $$ = $1; }
		;

term5	: term5 TOKEN_EXPO term6
			{ $$ = expr_create(EXPR_EXPO, $1, $3, NULL); }
		| term6
			{ $$ = $1; }
		;

term6	: term6 TOKEN_PST_FIX_INC
			{ $$ = expr_create(EXPR_PST_FIX_INC, $1, NULL, NULL); }
		| term6 TOKEN_PST_FIX_DEC
			{ $$ = expr_create(EXPR_PST_FIX_DEC, $1, NULL, NULL); }
		| TOKEN_SUB term7
			{ $$ = expr_create(EXPR_NEGATE, NULL, $2, NULL); }
		| TOKEN_NOT term7
			{ $$ = expr_create(EXPR_NOT, NULL, $2, NULL); } 
		| term7
			{ $$ = $1; }
		;

term7	: TOKEN_LPAREN expr TOKEN_RPAREN
			{ $$ = $2; }
		| literal
			{ $$ = $1; }
		;

literal	: TOKEN_NUMBER
			{ $$ = expr_create_integer_literal(atoi(yytext)); }
		| TOKEN_CHAR_LIT
			{ $$ = expr_create_char_literal(strdup(yytext)); }
		| TOKEN_STR_LIT
			{ $$ = expr_create_string_literal(strdup(yytext)); } 
		| TOKEN_TRUE
			{ $$ = expr_create_boolean_literal(1); }
		| TOKEN_FALSE
			{ $$ = expr_create_boolean_literal(0); } 
		| array_lookup
			{ $$ = $1; }
		| function_call
			{ $$ = $1; } 
		;

literal_list	: literal_comma
					{ $$ = $1; }
				/* |  epsilon 
					{ $$ = NULL; } */
				/* | multi_array TOKEN_COMMA multi_array  
					{ 
						$1->array_multi_next = $3;
						$$ = $1; 
					} */
				;

literal_comma	: literal TOKEN_COMMA literal_comma
					{ 
						$1->next = $3;
						/* $1->kind = EXPR_ARRAY_DEF; */
						$$ = $1;
					}
				| literal
					{ $$ = $1; }
				| multi_array multi_array_list
					{ 
						$1->array_multi_next = $2;
						$$ = $1; 
					}
				;

multi_array_list	: TOKEN_COMMA literal_list
						{ $$ = $2; }
					|
						{ $$ = NULL; }
					;

tern	: expr_tern TOKEN_QUESTION expr_tern TOKEN_COLON expr_tern
			{ $$ = expr_create_ternary($1, $3, $5); }
		;

expr_tern	: /* tern
				{ $$ = $1; }
			| */ term0
				{ $$ = $1; }
			/* | TOKEN_NOT term0
				{ $$ = expr_create(EXPR_NOT, $2, NULL, NULL); } */
			;

array_lookup	: ident array_index
					{ 
						if ($2) {
							$2->left = $1;
							$$ = $2;
						} else {
							$$ = $1;
						}
					}
				;

array_index		: multi_index
					{ $$ = $1; }
				| /* epsilon */
					{ $$ = NULL; } 
				;

multi_index		: TOKEN_LBRACKET expr TOKEN_RBRACKET multi_index
					{ 
						struct expr *e = expr_create(EXPR_ARRAY_INDEX, NULL, $2, NULL);
						e->array_lookup = $4;
						$$ = e;
					}
				| TOKEN_LBRACKET expr TOKEN_RBRACKET
					{ $$ = expr_create(EXPR_ARRAY_INDEX, NULL, $2, NULL); }
				;

function_call	: ident TOKEN_LPAREN expr_maybe TOKEN_RPAREN
					{ 
						struct expr *e = expr_create(EXPR_FUNC_CALL, NULL, $3, NULL);
						e->name = $1->name;
						$$ = e;
					}
				;
			

%%

int yyerror( char *str )
{
	printf("parse error: %s\n",str);
	return 0;
}
