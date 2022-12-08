#include "label.h"

int counter = 0;

int label_create() {
	counter = counter + 1;
	return counter;
}

char * label_name(int label) {
	char *str = malloc(8);
	sprintf(str, ".L%d", label);

	return str;
}
