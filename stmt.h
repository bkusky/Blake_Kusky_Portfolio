
#ifndef STMT_H
#define STMT_H

#include "decl.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );
void stmt_print( struct stmt *s, int indent );

void stmt_print_for(struct stmt *s, int indent);
void stmt_print_print(struct stmt *s);
void stmt_print_return(struct stmt *s);
void stmt_print_if_else(struct stmt *s, int indent);
void stmt_print_block(struct stmt *s, int indent);


/*
	param: scope change
		before entering the code of a function, a new scope is created to manage the params
		of the function. The new block that comes from that function should not be created in a new scope.
		This param is a flag that makes sure that the params and the variables declared within the 
		function are all apart of the same scope
*/
void stmt_resolve(struct stmt *s, int which, int scope_change);

void stmt_resolve_for(struct stmt *s, int which);
void stmt_resolve_print(struct stmt *s, int which);
void stmt_resolve_return(struct stmt *s, int which);
void stmt_resolve_if_else(struct stmt *s, int which);

void stmt_typecheck(struct stmt *s);
void stmt_typecheck_for(struct stmt *s);
void stmt_typecheck_print(struct stmt *s);
void stmt_typecheck_return(struct stmt *s);
void stmt_typecheck_if_else(struct stmt *s);

#endif
