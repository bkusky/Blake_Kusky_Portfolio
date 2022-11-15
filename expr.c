#include "expr.h"
#include "scope.h"

extern int typecheck_failure;

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
			// check params
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

void expr_resolve(struct expr *e) {
	if (!e) return;
	
	if (e->kind == EXPR_NAME) {
		e->symbol = scope_lookup_current(e->name);

		if (e->symbol) {
			// found in current scope
			switch (e->symbol->kind) {
				case SYMBOL_LOCAL:
					printf("%s resolves to local %d\n", e->name, e->symbol->which);
					break;
				case SYMBOL_PARAM:
					printf("%s resolves to param %d\n", e->name, e->symbol->which);
					break;
				case SYMBOL_GLOBAL:
					printf("%s resolves to global %s\n", e->name, e->symbol->name);
					break;
			}
		
		} else {
			// not found in current scope

			e->symbol = scope_lookup(e->name);

			if (e->symbol) {
				switch (e->symbol->kind) {
					case SYMBOL_LOCAL:
						printf("%s resolves to local %d\n", e->name, e->symbol->which);
						break;
					case SYMBOL_PARAM:
						printf("%s resolves to param %d\n", e->name, e->symbol->which);
						break;
					case SYMBOL_GLOBAL:
						printf("%s resolves to global %s\n", e->name, e->symbol->name);
						break;
				}
			} else {
				// did not find variable declaration
				printf("resolve error: %s is not defined\n", e->name);
				typecheck_failure = 1;
			}
		}

		
	} else if (e->kind == EXPR_FUNC_CALL) {
		e->symbol = scope_lookup(e->name);

		if (e->symbol) {
			printf("%s resolves to global %s\n", e->name, e->symbol->name);
		} else {
			// error

			printf("resolve error: %s is not defined\n", e->name);
			typecheck_failure = 1;
		}
	} else {
		expr_resolve(e->left);
		expr_resolve(e->right);
		expr_resolve(e->next);
	}
}

struct type * expr_typecheck( struct expr *e) {
	if (!e) return NULL;

	struct type *lt = expr_typecheck(e->left);
	struct type *rt = expr_typecheck(e->right);
	struct type *nt = expr_typecheck(e->next);

	struct type *result;

	switch(e->kind) {
		case EXPR_INT_LITERAL:
			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;
		case EXPR_STR_LITERAL:
			result = type_create(TYPE_STRING, NULL, NULL, NULL);
			break;
		case EXPR_CHAR_LITERAL:
			result = type_create(TYPE_CHARACTER, NULL, NULL, NULL);
			break;
		case EXPR_BOOL_LITERAL:
			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_ADD:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot add a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;

		case EXPR_SUB:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot subtract a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(") from a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;
		
		case EXPR_MULT:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot multiply a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") and a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;

		case EXPR_DIV:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot divide a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") by a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;
		
		case EXPR_ASSIGN:
			if (lt->kind == TYPE_AUTO) {
				// update type
				e->left->symbol->type = rt;
				lt = rt;

				printf("notice: type of %s is", e->left->symbol->name);
				type_print(e->left->symbol->type);
				printf("\n");
			}

			if (rt->kind == TYPE_AUTO)

			if ( !type_equals(lt, rt)) {
				
				// handling array cases
				if (lt->kind == TYPE_ARRAY && rt->kind != TYPE_ARRAY) {
					
					if (!type_equals(lt->subtype, rt)) {
						printf("type error: cannot assign a");
						type_print(rt);
						printf(" (");
						expr_print(e->right);
						printf(") to a");
						type_print(lt);
						printf(" (");
						expr_print(e->left);
						printf(")\n");
						
						typecheck_failure = 1;
					} 
				
				} else if (lt->kind != TYPE_ARRAY && rt->kind == TYPE_ARRAY) {
					if (!type_equals(lt, rt->subtype)) {
						printf("type error: cannot assign a");
						type_print(rt);
						printf(" (");
						expr_print(e->right);
						printf(") to a");
						type_print(lt);
						printf(" (");
						expr_print(e->left);
						printf(")\n");
						
						typecheck_failure = 1;
					} 
				} else {

					// other cases
					printf("type error: cannot assign a");
					type_print(rt);
					printf(" (");
					expr_print(e->right);
					printf(") to a");
					type_print(lt);
					printf(" (");
					expr_print(e->left);
					printf(")\n");

					typecheck_failure = 1;
				}
			}

			result = type_copy(lt);
			break;
		case EXPR_NOT:
			if (rt->kind == TYPE_STRING) {
				printf("type error: cannot perform a logical NOT of a string (");
				expr_print(e->right);
				printf(")\n");
				typecheck_failure = 1;
			} else if (rt->kind == TYPE_CHARACTER) {
				printf("type error: cannot perform a logical NOT of a character (");
				expr_print(e->right);
				printf(")\n");
				typecheck_failure = 1;	
			}

			result = type_copy(rt);
			break;
		case EXPR_OR:
			if (lt->kind != TYPE_BOOLEAN || lt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical OR of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");
				typecheck_failure = 1;
			}  else	if (rt->kind != TYPE_BOOLEAN || rt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical OR of a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");
				typecheck_failure = 1;
			}  

			if (!type_equals(lt, rt)) {
				printf("type error: cannot perform a logical OR of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") and a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_copy(lt);
			break;
		case EXPR_AND:
			if (lt->kind != TYPE_BOOLEAN || lt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical AND of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");
				typecheck_failure = 1;
			}  else	if (rt->kind != TYPE_BOOLEAN || rt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical AND of a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");
				typecheck_failure = 1;
			}  

			if (!type_equals(lt, rt)) {
				printf("type error: cannot perform a logical AND of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") and a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_copy(lt);
			break;
		case EXPR_LT:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot compare (less than) a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_LTE:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot compare (less than or equal to) a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_GT:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot compare (greater than) a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_GTE:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot compare (greater than or equal to) a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_EQ:
		case EXPR_NEQ:
			if (!type_equals(lt, rt)) {
				printf("type error: cannot compare equality of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") and a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_BOOLEAN, NULL, NULL, NULL);
			break;
		case EXPR_MOD:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot mod a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") and a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;

		case EXPR_EXPO:
			if ( lt->kind != TYPE_INTEGER || rt->kind != TYPE_INTEGER ) {
				printf("type error: cannot exponentiate a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(") to a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;	
		case EXPR_PST_FIX_DEC:
		case EXPR_PST_FIX_INC:
			if (lt->kind != TYPE_INTEGER) {
				printf("type error: cannot postfix operate on a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");

				typecheck_failure = 1;
			}

			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;
		case EXPR_NEGATE:
			if (rt->kind != TYPE_INTEGER) {
				printf("type error: cannot negate a");
				type_print(rt);
				printf(" (");
				expr_print(e->right);
				printf(")\n");

				typecheck_failure = 1;
			}
			
			result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			break;
		case EXPR_TERNARY:
			if (lt->kind != TYPE_BOOLEAN) {
				printf("type error: ternary expression 1 (");
				type_print(lt);
				printf(" --> ");
				expr_print(e->left);
				printf("). Must be a boolean\n");
				
				typecheck_failure = 1;
			}

			if (!type_equals(rt, nt)) {
				printf("type error: ternary type conflict >>> (");
				type_print(rt);
				printf(" --> ");
				expr_print(e->right);
				printf(" ) : (");
				type_print(nt);
				printf(" --> ");
				expr_print(e->next);
				printf(" ). Types must match\n");

				typecheck_failure = 1;
			}

			result = type_copy(rt);
			break;

		case EXPR_ARRAY_INDEX:
			
			if (rt->kind != TYPE_INTEGER) {
				printf("type error: array index must contain an integer expression: (");
				expr_print(e->right);
				printf(") is of type");
				type_print(rt);
				printf("\n");

				typecheck_failure = 1;
			}

			// return subtype
			
			//result = type_copy(rt->subtype);
			result = type_copy(rt);
			break;
		case EXPR_ARRAY_DEF:
			result = type_create(TYPE_INTEGER, NULL, NULL, NULL); 
			break;
		case EXPR_FUNC_CALL:;

			struct type *function_def = type_create(TYPE_FUNCTION, type_copy(e->symbol->type->subtype), param_list_copy(e->symbol->type->params), NULL);

			/* 
			if (!type_equals(function_def, function_call)) {
				printf("type error: function call conflict >>> call (");
				expr_print(e);
				printf(") conflicts with definition (");
				type_print(function_def);
				printf(")\n");

				typecheck_failure = 1;
			}*/

			result = type_copy(function_def->subtype);
			
			break;
	
		
		case EXPR_NAME:
			// lookup type from symbol table
			
			result = type_copy(e->symbol->type);
			break;

			
	}

	return result;
}


