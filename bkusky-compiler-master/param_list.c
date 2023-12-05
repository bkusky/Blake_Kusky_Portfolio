#include "param_list.h"
#include "scope.h"

extern FILE *outfile;

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

int param_list_resolve( struct param_list *p) {
	int which = 0;
//	struct param_list *t = p;
	
	while (p) {
		p->symbol = symbol_create(SYMBOL_PARAM, p->type, p->name, which);
		scope_bind(p->name, p->symbol);
	
		which++;
		p = p->next;
	}

	return which;
}

struct param_list *param_list_copy(struct param_list *p) {
	if (!p) return NULL;

	return param_list_create(p->name, type_copy(p->type), param_list_copy(p->next));

}

void param_list_codegen(struct param_list *p) {
	if (!p) return;

	// arg 1
	if (p) {
		fprintf(outfile, "\tpushq %%rdi\n");
		p->symbol = symbol_create(SYMBOL_PARAM, p->type, p->name, 0);
		p = p->next;
	}

	// arg 2
	if (p) {
		fprintf(outfile, "\tpushq %%rsi\n");
		p = p->next;
	}
	
	// arg 3
	if (p) {
		fprintf(outfile, "\tpushq %%rdx\n");
		p = p->next;
	}

	// arg 4
	if (p) {
		fprintf(outfile, "\tpushq %%rcx\n");
		p = p->next;
	}

	// arg 5
	if (p) {
		fprintf(outfile, "\tpushq %%r8\n");
		p = p->next;
	}

	// arg 6
	if (p) {
		fprintf(outfile, "\tpushq %%r9\n");
		p = p->next;
	}

	// argument error
	if (p) {
		printf("codegen error: too many arguments\n");
		exit(1);
	}
}
