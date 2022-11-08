#include "stmt.h"
#include "function.h"

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
	//indent_print(indent);
	printf(" {");
	stmt_print(s->body, indent+1);
	printf("\n");
	indent_print(indent-1);
	printf("}");
}
