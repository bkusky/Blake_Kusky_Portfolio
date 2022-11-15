#include "decl.h"
#include "function.h"
#include "scope.h"

extern int typecheck_failure;

// keep track of the current function type to use to typechecking
struct type *current_function_type;
char *current_function_name;

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
		printf(" =");
		expr_print(d->value);
	}

	if (!d->code) {
		printf(";");
	}

	if (d->code) {
		printf(" =");	
		stmt_print(d->code, indent);
		indent_print(indent);
	}

	printf("\n");
	decl_print(d->next, indent);
}

void decl_resolve(struct decl *d, int which) {
	if (!d) return;
	
	symbol_t kind;
	int level = scope_level();

	if (level > 1) {
		kind = SYMBOL_LOCAL;
	} else {
		kind = SYMBOL_GLOBAL;
	}
	
	d->symbol = symbol_create(kind, d->type, d->name, which);
	expr_resolve(d->value);

	if (d->type->kind == TYPE_FUNCTION && d->code) {
		/* function definition found - need to bind differently 
			if the function is already in the symbol table, then it has a prototype and no need to
			insert into the symbol table

			if the function is not in the symbol table then insert it in
		*/

		struct symbol *s = scope_lookup(d->name);

		if (!s) {
			// function prototype not found so need to bind
			scope_bind(d->name, d->symbol);
		} 
		
	} else {
		// for every other declaration we can bind normally
		scope_bind(d->name, d->symbol);
	}

	if (d->code) {
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code, 0, 1);
		scope_exit();
	} else if (!d->code) { // function prototype

	} 

	decl_resolve(d->next, which+1);
}

void decl_typecheck(struct decl *d) {
	if (!d) return;


	if (d->value) { // declaration of a variable
		struct type *t;
		t = expr_typecheck(d->value);
	
		if (d->symbol->type->kind == TYPE_AUTO) {
			d->symbol->type = t;
			printf("notice: type of %s is", d->symbol->name);
			type_print(t);
			printf("\n");
		} else {

			if (!type_equals(t, d->symbol->type)) {
				
				// check for array edge cases
				if (t->kind == TYPE_ARRAY && d->symbol->type->kind != TYPE_ARRAY) {
					
					if (!type_equals(t->subtype, d->symbol->type)) {
						printf("type error: cannot assign a");
						type_print(t);
						printf(" to");
						type_print(d->symbol->type);
						printf("\n");

						typecheck_failure = 1;
					} 

				} else if (t->kind != TYPE_ARRAY && d->symbol->type->kind == TYPE_ARRAY) {
					
					if (!type_equals(t, d->symbol->type->subtype)) {
						printf("type error: cannot assign a");
						type_print(t);
						printf(" to");
						type_print(d->symbol->type);
						printf("\n");

						typecheck_failure = 1;
					} 
					
				} else {
					
					// main case
					printf("type error: cannot assign a");
					type_print(t);
					printf(" to");
					type_print(d->symbol->type);
					printf("\n");

					typecheck_failure = 1;
				}
			}
		}
	} else if (d->type->kind == TYPE_FUNCTION) { // declaration of a function

		if (d->code) { // not a prototype
			current_function_type = d->symbol->type->subtype;
			current_function_name = d->name;
			stmt_typecheck(d->code);	

			d->symbol->type->subtype = current_function_type;	
		} else { // a prototype

		}
	}

	decl_typecheck(d->next);
}
