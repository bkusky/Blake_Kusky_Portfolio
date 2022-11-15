#include "param_list.h"
#include "scope.h"

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
	struct param_list *p = malloc(sizeof(struct param_list));
	p->name = name;
	p->type = type;
	p->next = next;

	return p;
}

void param_list_print( struct param_list *p ) {
	printf("%s:", p->name);
	type_print(p->type);
	
	if (p->next) {
		printf(", ");
		param_list_print(p->next);
	}
}

void param_list_resolve( struct param_list *p) {
	int which = 0;
	struct param_list *t = p;
	
	while (t) {
		t->symbol = symbol_create(SYMBOL_PARAM, t->type, t->name, which);
		scope_bind(t->name, t->symbol);
	
		which++;
		t = t->next;
	}
}

struct param_list *param_list_copy(struct param_list *p) {
	if (!p) return NULL;

	return param_list_create(p->name, type_copy(p->type), param_list_copy(p->next));

}
