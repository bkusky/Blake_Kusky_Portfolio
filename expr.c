#include "expr.h"

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right, struct expr *next) {
	struct expr *e = malloc(sizeof(struct expr));
	e->kind = kind;
	e->left = left;
	e->right = right;
	e->next = next;

	return e;
}

struct expr * expr_create_name( char *n ) {
	struct expr *e = expr_create(EXPR_NAME, NULL, NULL, NULL);
	e->name = n;

	return e;
}

struct expr * expr_create_integer_literal( int c ) {
	struct expr *e = expr_create(EXPR_INT_LITERAL, NULL, NULL, NULL);
	e->literal_value = c;

	return e;
}

struct expr * expr_create_boolean_literal( int c ) {
	struct expr *e = expr_create(EXPR_BOOL_LITERAL, NULL, NULL, NULL);
	e->literal_value = c;

	return e;
}

struct expr * expr_create_char_literal( const char *c ) {
	struct expr *e = expr_create(EXPR_CHAR_LITERAL, NULL, NULL, NULL);
	e->char_literal = c;
	return e;
}	

struct expr * expr_create_string_literal( const char *str ) {
	struct expr *e = expr_create(EXPR_STR_LITERAL, NULL, NULL, NULL);
	e->string_literal = str;
	return e;
}

struct expr * expr_create_ternary(struct expr *left, struct expr *right, struct expr *next) {
	struct expr *e = expr_create(EXPR_TERNARY, left, right, next);
	return e;
}

void expr_print( struct expr *e ) {
	if (!e) return;

	expr_print(e->left);

	
	switch (e->kind) {
		case EXPR_ADD:
			printf("+");
			break;
		case EXPR_SUB:
			printf("-");
			break;
		case EXPR_MULT:
			printf("*");
			break;
		case EXPR_DIV:
			printf("/");
			break;
		case EXPR_ASSIGN:
			printf("=");
			break;
		case EXPR_NOT:
			printf("!");
			break;
		case EXPR_OR:
			printf("||");
			break;
		case EXPR_AND:
			printf("&&");
			break;
		case EXPR_LT:
			printf("<");
			break;
		case EXPR_LTE:
			printf("<=");
			break;
		case EXPR_GT:
			printf(">");
			break;
		case EXPR_GTE:
			printf(">=");
			break;
		case EXPR_EQ:
			printf("==");
			break;
		case EXPR_NEQ:
			printf("!=");
			break;
		case EXPR_MOD:
			printf("%");
			break;
		case EXPR_EXPO:
			printf("^");
			break;
		case EXPR_PST_FIX_INC:
			printf("++");
			break;
		case EXPR_PST_FIX_DEC:
			printf("--");
			break;
		case EXPR_NEGATE:
			printf("-");
			break;
		case EXPR_TERNARY:
			printf("?");
			break;
		case EXPR_INT_LITERAL:
			expr_print_int(e);
			break;
		case EXPR_CHAR_LITERAL:
			expr_print_char(e);
			break;
		case EXPR_BOOL_LITERAL:
			expr_print_bool(e);
			break;
		case EXPR_STR_LITERAL:
			expr_print_string(e);
			break;
		case EXPR_ARRAY_INDEX:
			expr_print_array_lookup(e);
			break;
		case EXPR_ARRAY_DEF:
			expr_print_array(e);
			break;
		case EXPR_FUNC_CALL:
			expr_print_func_call(e);
			break;
		case EXPR_NAME:
			printf("%s", e->name);
			break;
	}

	if (e->kind != EXPR_ARRAY_INDEX) {
		expr_print(e->right);
	}

	if (e->kind == EXPR_TERNARY) {
		expr_print_tern(e);
	}
	
}

void expr_print_int(struct expr *e) {
	printf("%d", e->literal_value);
}

void expr_print_char(struct expr *e) {
	printf("%s", e->char_literal);
}

void expr_print_bool(struct expr *e) {
	if (e->literal_value) {
		printf("true");
	} else {
		printf("false");
	}
}

void expr_print_string(struct expr *e) {
	printf("%s", e->string_literal);	
}

void expr_print_array(struct expr *e) {
	printf("{");	
	
	printf("0");
	printf("}");
}

void expr_print_literal_list(struct expr *e) {
	struct expr *curr = e;
	while (curr) {

		if (e->string_literal != NULL) {
			printf("%s", curr->string_literal);
		} else {
			printf("%d", curr->literal_value);
		}

		if (curr->next) {
			printf(", ");
		}

		curr = curr->next;
	}
}

void expr_print_func_call(struct expr *e) {
	
	printf("%s(", e->name);

	// print arguments
	
	struct expr *arg = e->right;

	while (arg) {
		expr_print(arg);

		if (arg->next) {
			printf(", ");
		}
		arg = arg->next;
	}

	printf(")");
	e->right = NULL;
}

void expr_print_tern(struct expr *e) {
	printf(":");
	expr_print(e->next);
}

void expr_print_array_lookup(struct expr *e) {
	printf("[");
	expr_print(e->right);
	printf("]");
	expr_print(e->array_lookup);

}
