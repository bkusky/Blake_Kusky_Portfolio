
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "param_list.h"
#include <stdio.h>
#include <stdlib.h>

struct decl {
	char *name;
	struct type *type;
	struct type *type_params;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct type *type_params, struct expr *value, struct stmt *code, struct decl *next );
void decl_print( struct decl *d, int indent );

#endif


