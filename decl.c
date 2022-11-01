#include "decl.h"
#include "function.h"

struct decl * decl_create( char *name, struct type *type, struct type *type_params, struct expr *value, struct stmt *code, struct decl *next ) {
	struct decl *d = malloc(sizeof(struct decl));
	d->name = name;
	d->type = type;
	d->type_params = type_params;
	d->value = value;
	d->code = code;
	// symbol?
	d->next = next;

	return d;
}

void decl_print( struct decl *d, int indent ) {
	if (!d) return;
	printf("%s:", d->name);
	type_print(d->type);
	
	if (d->value) {
		printf(" = ");
		expr_print(d->value);
	}

	if (!d->code) {
		printf(";");
	}

	if (d->code) {
		printf(" = \n{");	
		stmt_print(d->code, indent+1);
		indent_print(indent);
		printf("\n}");
	}

	printf("\n");
	decl_print(d->next, indent);
}
