#include "type.h"

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params , struct expr * arr_length) {
	struct type *t = malloc(sizeof(struct type));
	t->kind = kind;
	t->subtype = subtype;
	t->params = params;
	t->arr_length = arr_length;

	return t;
}

void type_print( struct type *t ) {
	if (!t) return;

	switch (t->kind) {
		case TYPE_VOID:
			printf(" void");
			break;
		case TYPE_BOOLEAN:
			printf(" boolean");
			break;
		case TYPE_CHARACTER:
			printf(" char");
			break;
		case TYPE_INTEGER:
			printf(" integer");
			break;
		case TYPE_STRING:
			printf(" string");
			break;
		case TYPE_AUTO:
			printf(" auto");
			break;
		case TYPE_ARRAY:
			if (t->arr_length) {
				printf(" array [%d]", t->arr_length->literal_value);
			} else {
				printf(" array []");
			}
			break;
		case TYPE_FUNCTION:
			printf(" function");
			break;
		case TYPE_ARRAY_ARG:
			printf(" array []");
			break;
	}

	if (t->subtype) {
		type_print(t->subtype);
	}
	
	if (t->kind == TYPE_FUNCTION) {
		type_func_print(t);
	}
}

void type_func_print(struct type *t) {
	printf(" ( ");

	if (t->params) {
		param_list_print(t->params);
	}

	printf(" )");
}

bool type_equals(struct type *a, struct type *b) {
	
	if (!a && !b) return true;
	if (!a || !b) return false;

	if (a->kind == b->kind) {
		if (a->kind == TYPE_ARRAY && b->kind == TYPE_ARRAY) {
			// check subtypes
			puts("0");	
			if (type_equals(a->subtype, b->subtype)) {
				return true;
			} else {
				return false;
			}
		} else if (a->kind == TYPE_FUNCTION) {
			// check subtypes
			if (type_equals(a->subtype, b->subtype)) {
				
				// check params
				
				struct param_list *p1 = a->params;
				struct param_list *p2 = b->params;

				while (p1 || p2) {
					
					if (!p1 || !p2) {
						return false;
					} else {
						if (type_equals(p1->type, p2->type)) {
							p1 = p1->next;
							p2 = p2->next;
						} else {
							return false;
						}
					}
				} 

				return true;

			} else {
				return false;
			}

			
		} else { // atomic type
			return true;
		}
		
	} else {
		return false;
	}
	
}

struct type *type_copy(struct type *t) {
	if (!t) return NULL;

	return type_create(t->kind, type_copy(t->subtype), param_list_copy(t->params), t->arr_length);

}
