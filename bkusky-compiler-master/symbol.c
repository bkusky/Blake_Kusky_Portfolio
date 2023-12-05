#include "symbol.h"

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name, int which ) {
	struct symbol *s = malloc(sizeof(struct symbol));
	s->kind = kind;
	s->type = type;
	s->name = name;
	s->which = which;

	return s;
}

const char * symbol_codegen( struct symbol *s) {
	if (!s) return NULL;

	int offset;

	switch (s->kind) {
		case SYMBOL_GLOBAL:
			return strdup(s->name);
		case SYMBOL_LOCAL:
		case SYMBOL_PARAM:
			offset = (s->which + 1) * 8;

			char *str = malloc(16);
			sprintf(str, "-%d(%%rbp)", offset);
			return str;
	}
}
