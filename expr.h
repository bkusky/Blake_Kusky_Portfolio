#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdlib.h>


typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MULT,
	EXPR_DIV,
	EXPR_ASSIGN,
	EXPR_NOT,
	EXPR_OR,
	EXPR_AND,
	EXPR_LT,
	EXPR_LTE,
	EXPR_GT,
	EXPR_GTE,
	EXPR_EQ,
	EXPR_NEQ,
	EXPR_MOD,
	EXPR_EXPO,
	EXPR_PST_FIX_INC,
	EXPR_PST_FIX_DEC,
	EXPR_NEGATE,
	EXPR_TERNARY,
	EXPR_ARRAY_DEF,
	EXPR_ARRAY_INDEX,
	EXPR_FUNC_CALL,
	
	EXPR_NAME,
	EXPR_INT_LITERAL,
	EXPR_CHAR_LITERAL,
	EXPR_BOOL_LITERAL,
	EXPR_STR_LITERAL,
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;
	struct expr *next;

	/* used by various leaf exprs */
	char *name;
	int literal_value;
	const char *char_literal;
	const char * string_literal;
	struct expr *array_lookup;
	struct expr *array_multi_next;
	struct symbol *symbol;

	int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, struct expr *next );

struct expr * expr_create_name( char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( const char *c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_ternary(struct expr *left, struct expr *right, struct expr *next);

void expr_print_int(struct expr *e);
void expr_print_char(struct expr *e);
void expr_print_bool(struct expr *e);
void expr_print_string(struct expr *e);
void expr_print_array(struct expr *e);
void expr_print_literal_list(struct expr *e);
void expr_print_func_call(struct expr *e);
void expr_print_tern(struct expr *e);
void expr_print_array_lookup(struct expr *e);

void expr_print( struct expr *e );

void expr_resolve(struct expr *e);
struct type *expr_typecheck(struct expr *e);

void expr_codegen(struct expr *e);
void expr_codegen_function_call(struct expr *e);
#endif
