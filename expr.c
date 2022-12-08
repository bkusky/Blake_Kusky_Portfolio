#include "expr.h"
#include "scope.h"
#include "scratch.h"
#include "label.h"

extern int typecheck_failure;
extern int print_rsv;
extern FILE *outfile;

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
			printf("%%");
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

	expr_print_literal_list(e);

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
					if (print_rsv) printf("%s resolves to local %d\n", e->name, e->symbol->which);
					break;
				case SYMBOL_PARAM:
					if (print_rsv) printf("%s resolves to param %d\n", e->name, e->symbol->which);
					break;
				case SYMBOL_GLOBAL:
					if (print_rsv) printf("%s resolves to global %s\n", e->name, e->symbol->name);
					break;
			}
		
		} else {
			// not found in current scope

			e->symbol = scope_lookup(e->name);

			if (e->symbol) {
				switch (e->symbol->kind) {
					case SYMBOL_LOCAL:
						if (print_rsv) printf("%s resolves to local %d\n", e->name, e->symbol->which);
						break;
					case SYMBOL_PARAM:
						if (print_rsv) printf("%s resolves to param %d\n", e->name, e->symbol->which);
						break;
					case SYMBOL_GLOBAL:
						if (print_rsv) printf("%s resolves to global %s\n", e->name, e->symbol->name);
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
			if (print_rsv) printf("%s resolves to global %s\n", e->name, e->symbol->name);
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
			if (lt->kind != TYPE_BOOLEAN && lt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical OR of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");
				typecheck_failure = 1;
			}  else	if (rt->kind != TYPE_BOOLEAN && rt->kind != TYPE_INTEGER) {
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
			if (lt->kind != TYPE_BOOLEAN && lt->kind != TYPE_INTEGER) {
				printf("type error: cannot perform a logical AND of a");
				type_print(lt);
				printf(" (");
				expr_print(e->left);
				printf(")\n");
				typecheck_failure = 1;
			}  else	if (rt->kind != TYPE_BOOLEAN && rt->kind != TYPE_INTEGER) {
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

			if (e->symbol) {
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
			} else {
				result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			}

			break;
	
		
		case EXPR_NAME:
			// lookup type from symbol table
			if (e->symbol) {
				result = type_copy(e->symbol->type);
			} else {
				result = type_create(TYPE_INTEGER, NULL, NULL, NULL);
			}
			break;

			
	}

	return result;
}

void expr_codegen(struct expr *e) {
	if (!e) return;
	
	switch (e->kind) {
		case EXPR_NAME:
			e->reg = scratch_alloc();
			if (e->symbol) {
				fprintf(outfile, "\tmovq %s, %s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
			} else {
				fprintf(outfile, "\tmovq -8(%%rbp), %s\n", scratch_name(e->reg));			
			}
			break;
		case EXPR_INT_LITERAL:
			e->reg = scratch_alloc();
			fprintf(outfile, "\tmovq $%d, %s\n", e->literal_value, scratch_name(e->reg));
			break;
		case EXPR_BOOL_LITERAL:
			e->reg = scratch_alloc();
			fprintf(outfile, "\tmovq $%d, %s\n", e->literal_value, scratch_name(e->reg));
			break;
		case EXPR_CHAR_LITERAL:
			e->reg = scratch_alloc();

			// clean char
			char *c = strdup(e->char_literal);
			int l = strlen(c);

			char res;
			if (l == 3) {
				res = c[1];
			} else {
				char t = c[2];
				switch (t) {
					case 'n':
						res = '\n';
						break;
					case '0':
						res = '\0';
					default:
						res = c[2];
						break;
				}
			}
			
			fprintf(outfile, "\tmovq $%d, %s\n", res, scratch_name(e->reg));
			break;
		case EXPR_STR_LITERAL:
			e->reg = scratch_alloc();
			int str_label = label_create();
			// put string in data section
			fprintf(outfile, ".data\n%s:\n\t.string %s\n.text\n\tleaq %s, %s\n", label_name(str_label), e->string_literal, label_name(str_label), scratch_name(e->reg));
			break;
		case EXPR_ADD:
			expr_codegen(e->left);
			expr_codegen(e->right);
			fprintf(outfile, "\taddq %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_SUB:
			expr_codegen(e->left);
			expr_codegen(e->right);
			fprintf(outfile, "\tsubq %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
			e->reg = e->left->reg;
			scratch_free(e->right->reg);
			break;
		case EXPR_MULT:
			expr_codegen(e->left);
			expr_codegen(e->right);
			// move into rax
			fprintf(outfile, "\tmovq %s, %%rax\n", scratch_name(e->left->reg));
			// multiply
			fprintf(outfile, "\timulq %s\n", scratch_name(e->right->reg));
			fprintf(outfile, "\tmovq %%rax, %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_DIV:
			expr_codegen(e->left);
			expr_codegen(e->right);
			// move into rax
			fprintf(outfile, "\tmovq %s, %%rax\n", scratch_name(e->left->reg));
			// clear rbx - rdx
			fprintf(outfile, "\tmovq $0, %%rbx\n\tmovq $0, %%rcx\n\tmovq $0, %%rdx\n");
			// divide
			fprintf(outfile, "\tidivq %s\n", scratch_name(e->right->reg));
			fprintf(outfile, "\tmovq %%rax, %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_ASSIGN:
			expr_codegen(e->right);
			fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e->right->reg), symbol_codegen(e->left->symbol));
			e->reg = e->right->reg;
			break;
		case EXPR_NOT:
			expr_codegen(e->right);
			fprintf(outfile, "\tnot %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			break;
		case EXPR_OR:
			expr_codegen(e->left);
			expr_codegen(e->right);
			fprintf(outfile, "\tor %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_AND:
			expr_codegen(e->left);
			expr_codegen(e->right);
			fprintf(outfile, "\tand %s, %s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_LT:
		case EXPR_LTE:
		case EXPR_GT:
		case EXPR_GTE:
		case EXPR_EQ:
		case EXPR_NEQ:
			expr_codegen(e->left);
			expr_codegen(e->right);
			// compare
			fprintf(outfile, "\tcmp %s, %s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
			scratch_free(e->left->reg);
			scratch_free(e->right->reg);
			int reg_result = scratch_alloc();
			int cmp_true_label = label_create();
			int cmp_skip_label = label_create();

			// jump depending on expr
			if (e->kind == EXPR_LT) {
				fprintf(outfile, "\tjl %s\n", label_name(cmp_true_label));
			} else if (e->kind == EXPR_LTE) {
				fprintf(outfile, "\tjle %s\n", label_name(cmp_true_label));
			} else if (e->kind == EXPR_GT) {
				fprintf(outfile, "\tjg %s\n", label_name(cmp_true_label));
			} else if (e->kind == EXPR_GTE) {
				fprintf(outfile, "\tjge %s\n", label_name(cmp_true_label));
			} else if (e->kind == EXPR_EQ) {
				fprintf(outfile, "\tje %s\n", label_name(cmp_true_label));
			} else if (e->kind == EXPR_NEQ) {
				fprintf(outfile, "\tjne %s\n", label_name(cmp_true_label));
			}
			// false
			fprintf(outfile, "\tmovq $0, %s\n", scratch_name(reg_result));
			fprintf(outfile, "\tjmp %s\n", label_name(cmp_skip_label));
			// true
			fprintf(outfile, "%s:\n", label_name(cmp_true_label));
			fprintf(outfile, "\tmovq $1, %s\n", scratch_name(reg_result));
			// end
			fprintf(outfile, "%s:\n", label_name(cmp_skip_label));
			e->reg = reg_result;
			break;
		case EXPR_FUNC_CALL:
			expr_codegen_function_call(e);
			// move result into register
			int func_result_reg = scratch_alloc();
			fprintf(outfile, "\tmovq %%rax, %s\n", scratch_name(func_result_reg));
			e->reg = func_result_reg;
			break;
		case EXPR_MOD:
			expr_codegen(e->left);
			expr_codegen(e->right);
			// move into rax
			fprintf(outfile, "\tmovq %s, %%rax\n", scratch_name(e->left->reg));
			// clear rbx - rdx
			fprintf(outfile, "\tmovq $0, %%rbx\n\tmovq $0, %%rcx\n\tmovq $0, %%rdx\n");
			// divide
			fprintf(outfile, "\tidivq %s\n", scratch_name(e->right->reg));
			fprintf(outfile, "\tmovq %%rdx, %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			break;
		case EXPR_EXPO:
			expr_codegen(e->left);
			expr_codegen(e->right);

			struct expr *e_loop = expr_create(0, NULL, NULL, NULL);
			struct expr *e_result = expr_create(0, NULL, NULL, NULL);
			e_loop->reg = scratch_alloc();
			e_result->reg = scratch_alloc();
			int expo_start_label = label_create();
			int expo_end_label = label_create();

			fprintf(outfile, "\tmovq $1, %s\n", scratch_name(e_result->reg));
			fprintf(outfile, "\tmovq $0, %s\n", scratch_name(e_loop->reg));
			fprintf(outfile, "%s:\n\tcmp %s, %s\n", label_name(expo_start_label) ,scratch_name(e_loop->reg), scratch_name(e->right->reg));
			fprintf(outfile, "\tje %s\n", label_name(expo_end_label));
			fprintf(outfile, "\taddq $1, %s\n", scratch_name(e_loop->reg));
			fprintf(outfile, "\tmovq %s, %%rax\n", scratch_name(e->left->reg));
			fprintf(outfile, "\timulq %s\n", scratch_name(e_result->reg));
			fprintf(outfile, "\tmovq %%rax, %s\n\tjmp %s\n%s:\n", scratch_name(e_result->reg), label_name(expo_start_label), label_name(expo_end_label));
			fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e_result->reg), scratch_name(e->right->reg));

			e->reg = e->right->reg;
			scratch_free(e->left->reg);
			scratch_free(e_loop->reg);
			scratch_free(e_result->reg);
			break;
		case EXPR_PST_FIX_INC:
			expr_codegen(e->left);
			int result_pst_inc = scratch_alloc();
			fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e->left->reg), scratch_name(result_pst_inc));
			fprintf(outfile, "\taddq $1, %s\n", scratch_name(e->left->reg));
			if (e->left->name) {
				// variable increment
				fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
			}
			scratch_free(e->left->reg);
			e->reg = result_pst_inc;
			break;
		case EXPR_PST_FIX_DEC:
			expr_codegen(e->left);
			int result_pst_dec = scratch_alloc();
			fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e->left->reg), scratch_name(result_pst_dec));
			fprintf(outfile, "\tsubq $1, %s\n", scratch_name(e->left->reg));
			if (e->left->name) {
				// variable increment
				fprintf(outfile, "\tmovq %s, %s\n", scratch_name(e->left->reg), symbol_codegen(e->left->symbol));
			}
			scratch_free(e->left->reg);
			e->reg = result_pst_dec;
			break;
		case EXPR_NEGATE:
			expr_codegen(e->right);
			fprintf(outfile, "\tneg %s\n", scratch_name(e->right->reg));
			e->reg = e->right->reg;
			break;
	}
}

void expr_codegen_function_call(struct expr *e) {
	// evaluate each arg
	struct expr *arg_start = e->right;
	struct expr *arg_curr = arg_start;

	while (arg_curr) {
		expr_codegen(arg_curr);
		arg_curr = arg_curr->next;
	}

	// move each argument into argument registers
	arg_curr = arg_start;
	
	// arg 1
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%rdi\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	// arg 2
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%rsi\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	// arg 3
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%rdx\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	// arg 4
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%rcx\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	// arg 5
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%r8\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	// arg 6
	if (arg_curr) {
		fprintf(outfile, "\tmovq %s, %%r9\n", scratch_name(arg_curr->reg));
		scratch_free(arg_curr->reg);
		arg_curr = arg_curr->next;
	}

	if (arg_curr) {
		printf("codegen error: too many arguments\n");
		exit(1);
	}
	
	// caller save registers
	fprintf(outfile, "\tpushq %%r10\n\tpushq %%r11\n");

	// call the function
	fprintf(outfile, "\tcall %s\n", e->name);

	// restore caller save registers
	fprintf(outfile, "\tpopq %%r11\n\tpopq %%r10\n");
}
