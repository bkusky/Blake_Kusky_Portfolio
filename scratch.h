#ifndef SCRATCH_H
#define SCRATCH_H

#include <stdio.h>
#include <stdlib.h>

struct scratch {
	int r;
	const char *name;
	int inuse;
	struct scratch *next;
};

void scratch_init();
struct scratch *scratch_create(int r, const char *name, int inuse, struct scratch *next);
int scratch_alloc();
void scratch_free(int r);
const char * scratch_name(int r);

#endif
