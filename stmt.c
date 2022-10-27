#include "stmt.h"

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {
	return NULL;
}


void stmt_print( struct stmt *s, int indent ) {
}
