#include "stmt.h"
#include "scope.h"
#include "function.h"

extern int typecheck_failure;
extern struct type *current_function_type;
extern char *current_function_name;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next ) {
	struct stmt *s = malloc(sizeof(struct stmt));
	s->kind = kind;
	s->decl = decl;
	s->init_expr = init_expr;
	s->expr = expr;
	s->next_expr = next_expr;
	s->body = body;
	s->else_body = else_body;
	s->next = next;

	return s;
}


void stmt_print( struct stmt *s, int indent ) {
	if (!s) return;
	
	if (s->kind != STMT_BLOCK) {
		printf("\n");
		indent_print(indent);
	}

	switch (s->kind) {
		case STMT_DECL:
			decl_print(s->decl, indent);
			break;

		case STMT_EXPR:
			expr_print(s->expr);
			printf(";");
			break;

		case STMT_FOR:
			stmt_print_for(s, indent);
			break;

		case STMT_PRINT:
			stmt_print_print(s);
			printf(";");
			break;

		case STMT_RETURN:
			stmt_print_return(s);
			printf(";");
			break;

		case STMT_IF_ELSE:
			stmt_print_if_else(s, indent);
			break;
		
		case STMT_BLOCK:
			stmt_print_block(s, indent);
			break;
	}
	
	stmt_print(s->next, indent);
}

void stmt_print_for(struct stmt *s, int indent) {
	printf("for (");

	expr_print(s->init_expr);
	printf("; ");
	expr_print(s->expr);
	printf("; ");
	expr_print(s->next_expr);
	printf(")");

	if (s->body) {
		stmt_print(s->body, indent+1);
	}
}

void stmt_print_print(struct stmt *s) {

	printf("print ");
	if (!s->expr) {
		return;
	}

	struct expr *e = s->expr;
	expr_print(e);
	while (e->next) {
		printf(", ");
		e = e->next;
		expr_print(e);
	}

}

void stmt_print_return(struct stmt *s) {
	printf("return ");
	expr_print(s->expr);
}

void stmt_print_if_else(struct stmt *s, int indent) {
	printf("if (");
	expr_print(s->expr);
	printf(")");
	stmt_print(s->body, indent+1);

	

	if (s->else_body) {

		printf(" else");

		if (s->else_body->kind == STMT_IF_ELSE) {
			stmt_print(s->else_body, indent);
		} else {
			stmt_print(s->else_body, indent+1);
		}
	}
}

void stmt_print_block(struct stmt *s, int indent) {
	//indent_print(indent)
	printf(" {");
	stmt_print(s->body, indent+1);
	printf("\n");
	indent_print(indent-1);
	printf("}");
}


void stmt_resolve(struct stmt *s, int which, int scope_change) {
	if (!s) return;
	
	switch (s->kind) {
		case STMT_DECL:
			decl_resolve(s->decl, which++);
			break;

		case STMT_EXPR:
			expr_resolve(s->expr);
			break;

		case STMT_FOR:
			stmt_resolve_for(s, which);
			break;

		case STMT_PRINT:
			stmt_resolve_print(s, which);
			break;

		case STMT_RETURN:
			stmt_resolve_return(s, which);
			break;

		case STMT_IF_ELSE:
			stmt_resolve_if_else(s, which);
			break;
		
		case STMT_BLOCK:
			// if first time entering a function there is no need to create a new scope
			// because decl.c already does this
			if (scope_change) {
				stmt_resolve(s->body, which, 0);
			} else {

				scope_enter();
				stmt_resolve(s->body, 0, 0);
				scope_exit();

			}


			break;
	}

	stmt_resolve(s->next, which, 0);

}

void stmt_resolve_for(struct stmt *s, int which) {
	// resolves exprs
	
	expr_resolve(s->init_expr);
	expr_resolve(s->expr);
	expr_resolve(s->next_expr);
	
	// resolve body
	
	if (s->body) {
		stmt_resolve(s->body, which, 1);
	}

}

void stmt_resolve_print(struct stmt *s, int which) {
	if (!s->expr) return;


	struct expr *e = s->expr;
	expr_resolve(e);
	while (e->next) {
		e = e->next;
		expr_resolve(e);
	}



}

void stmt_resolve_return(struct stmt *s, int which) {
	expr_resolve(s->expr);
}


void stmt_resolve_if_else(struct stmt *s, int which) {
	expr_resolve(s->expr);
	stmt_resolve(s->body, which, 0);
	

	if (s->else_body) {

		stmt_resolve(s->else_body, which, 0);

	}
}

void stmt_typecheck(struct stmt *s) {
	if (!s) return;

	struct type *t;

	switch (s->kind) {
		case STMT_DECL:
			decl_typecheck(s->decl);
			break;
		case STMT_EXPR:
			expr_typecheck(s->expr);
			break;
		case STMT_FOR:
			stmt_typecheck_for(s);
			break;
		case STMT_PRINT:
			stmt_typecheck_print(s);
			break;
		case STMT_RETURN:
			stmt_typecheck_return(s);	
			break;
		case STMT_IF_ELSE:
			stmt_typecheck_if_else(s);
			break;
		case STMT_BLOCK:
			stmt_typecheck(s->body);
			break;
	}
	
	stmt_typecheck(s->next);
}

void stmt_typecheck_for(struct stmt *s) {
	struct type *init_t = expr_typecheck(s->init_expr);
	struct type *mid_t = expr_typecheck(s->expr);
	struct type *next_t = expr_typecheck(s->next_expr);

	if (init_t) {
		if (init_t->kind != TYPE_INTEGER) {
			printf("type error: for loop parameter expression 1 (");
			type_print(init_t);
			printf(" ) is not of type integer");
			typecheck_failure = 1;
		}
	}

	if (mid_t) {
		if (mid_t->kind != TYPE_BOOLEAN) {
			printf("type error: for loop parameter expression 2 (");
			type_print(mid_t);
			printf(" ) is not of type boolean");
			typecheck_failure = 1;
		}
	}

	if (next_t) {
		if (next_t->kind != TYPE_INTEGER) {
			printf("type error: for loop parameter expression 3 (");
			type_print(next_t);
			printf(" ) is not of type integer");
			typecheck_failure = 1;
		}
	}
}

void stmt_typecheck_print(struct stmt *s) {
	if (!s->expr);

	struct expr *e = s->expr;
	struct type *t = expr_typecheck(e);

	if (t->kind != TYPE_STRING && t->kind != TYPE_CHARACTER && t->kind != TYPE_INTEGER && t->kind != TYPE_BOOLEAN ) {
		printf("type error: cannot have print statement expression");
		type_print(t);
		printf(" (");
		expr_print(e);
		printf(")\n");
		typecheck_failure = 1;
	}

	while (e->next) {
		e = e->next;
		t = expr_typecheck(e);
		if (t->kind != TYPE_STRING && t->kind != TYPE_CHARACTER && t->kind != TYPE_INTEGER && t->kind != TYPE_BOOLEAN ) {
			printf("type error: cannot have print statement expression");
			type_print(t);
			printf(" (");
			expr_print(e);
			printf(")\n");
			typecheck_failure = 1;
		}
	}
}

void stmt_typecheck_return(struct stmt *s) {
	struct type *t = expr_typecheck(s->expr);

	if (current_function_type->kind == TYPE_AUTO) {
		current_function_type = t;
		printf("notice: return type of function %s is", current_function_name);
		type_print(current_function_type);
		printf("\n");
	} else {
		if (!type_equals(t, current_function_type)) {
			printf("type error: cannot return a");
			type_print(t);
			printf(" (");
			expr_print(s->expr);
			printf(") in a function that returns");
			type_print(current_function_type);
			printf("\n");

			typecheck_failure = 1;
		}
	}
}

void stmt_typecheck_if_else(struct stmt *s) {
	struct type *t = expr_typecheck(s->expr);

	if (t->kind != TYPE_BOOLEAN) {
		printf("type error: if condition");
		type_print(t);
		printf(" (");
		expr_print(s->expr);
		printf(") must be a boolean\n");

		typecheck_failure = 1;
	}

	stmt_typecheck(s->body);

	if (s->else_body) {
		stmt_typecheck(s->else_body);
	}
}