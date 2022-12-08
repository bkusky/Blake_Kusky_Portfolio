#include "scratch.h"

struct scratch *s_top = NULL;

struct scratch *scratch_create(int r, const char *name, int inuse, struct scratch *next) {
	struct scratch *s = malloc(sizeof(struct scratch));
	s->r = r;
	s->name = name;
	s->inuse = inuse;
	s->next = next;
	
	return s;
}
	
void scratch_init() {
	struct scratch *s = scratch_create(0, "%rbx", 0,
						scratch_create(1, "%r10", 0,
						scratch_create(2, "%r11", 0,
						scratch_create(3, "%r12", 0,
						scratch_create(4, "%r13", 0,
						scratch_create(5, "%r14", 0,
						scratch_create(6, "%r15", 0, 
						NULL)))))));

	s_top = s;
}

int scratch_alloc() {
	
	struct scratch *s = s_top;

	int result = -1;
	while (s) {
		if (!s->inuse) {
			s->inuse = 1;
			result = s->r;
			break;
		}

		s = s->next;
	}

	if (result == -1) {
		printf("codegen error: out of registers\n");
		exit(1);
	}	

	return result;
}

void scratch_free(int r) {
	struct scratch *s = s_top;

	while (s) {
		if (s->r == r) {
			s->inuse = 0;
		}

		s = s->next;
	}
}

const char * scratch_name(int r) {
	struct scratch *s = s_top;

	while (s) {
		if (s->r == r) {
			return s->name;
		}

		s = s->next;
	}

	printf("codegen error: r%d is not a scratch register\n", r);
	exit(1);
}
