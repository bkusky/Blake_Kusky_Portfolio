#include "decl.h"
#include "function.h"
#include "scope.h"
#include "scratch.h"
#include "label.h"

extern int typecheck_failure;
extern FILE *outfile;

// keep track of the current function type to use to typechecking
struct type *current_function_type;
char *current_function_name;
int num_curr_decls;

struct decl * decl_create( char *name, struct type *type, struct type *type_params, struct expr *value, struct stmt *code, struct decl *next ) {
	struct decl *d = malloc(sizeof(struct decl));
	d->name = name;
	d->type = type;
	d->type_params = type_params;
	d->value = value;
	d->code = code;
	// symbol?
	d->next = next;

	return d;
}

void decl_print( struct decl *d, int indent ) {
	if (!d) return;
	printf("%s:", d->name);
	type_print(d->type);
	
	if (d->value) {
		printf(" =");
		expr_print(d->value);
	}

	if (!d->code) {
		printf(";");
	}

	if (d->code) {
		printf(" =");	
		stmt_print(d->code, indent);
		indent_print(indent);
	}

	printf("\n");
	decl_print(d->next, indent);
}

void decl_resolve(struct decl *d, int which) {
	if (!d) return;
	
	symbol_t kind;
	int level = scope_level();

	if (level > 1) {
		kind = SYMBOL_LOCAL;
	} else {
		kind = SYMBOL_GLOBAL;
	}
	
	d->symbol = symbol_create(kind, d->type, d->name, which);
	expr_resolve(d->value);

	if (d->type->kind == TYPE_FUNCTION && d->code) {
		/* function definition found - need to bind differently 
			if the function is already in the symbol table, then it has a prototype and no need to
			insert into the symbol table

			if the function is not in the symbol table then insert it in
		*/

		struct symbol *s = scope_lookup(d->name);

		if (!s) {
			// function prototype not found so need to bind
			scope_bind(d->name, d->symbol);
		} 
		
	} else {
		// for every other declaration we can bind normally
		scope_bind(d->name, d->symbol);
	}

	if (d->code) {
		scope_enter();
		int w = param_list_resolve(d->type->params);
		stmt_resolve(d->code, w, 1);
		scope_exit();
	} else if (!d->code) { // function prototype

	} 

	decl_resolve(d->next, which+1);
}

void decl_typecheck(struct decl *d) {
	if (!d) return;


	if (d->value) { // declaration of a variable
		struct type *t;
		t = expr_typecheck(d->value);
	
		if (d->symbol->type->kind == TYPE_AUTO) {
			d->symbol->type = t;
			printf("notice: type of %s is", d->symbol->name);
			type_print(t);
			printf("\n");
		} else {

			if (!type_equals(t, d->symbol->type)) {
				
				// check for array edge cases
				if (t->kind == TYPE_ARRAY && d->symbol->type->kind != TYPE_ARRAY) {
					
					if (!type_equals(t->subtype, d->symbol->type)) {
						printf("type error: cannot assign a");
						type_print(t);
						printf(" to");
						type_print(d->symbol->type);
						printf("\n");

						typecheck_failure = 1;
					} 

				} else if (t->kind != TYPE_ARRAY && d->symbol->type->kind == TYPE_ARRAY) {
					
					if (!type_equals(t, d->symbol->type->subtype)) {
						printf("type error: cannot assign a");
						type_print(t);
						printf(" to");
						type_print(d->symbol->type);
						printf("\n");

						typecheck_failure = 1;
					} 
					
				} else {
					
					// main case
					printf("type error: cannot assign a");
					type_print(t);
					printf(" to");
					type_print(d->symbol->type);
					printf("\n");

					typecheck_failure = 1;
				}
			}
		}
	} else if (d->type->kind == TYPE_FUNCTION) { // declaration of a function

		if (d->code) { // not a prototype
			current_function_type = d->symbol->type->subtype;
			current_function_name = d->name;
			stmt_typecheck(d->code);	

			d->symbol->type->subtype = current_function_type;	
		} else { // a prototype

		}
	}

	decl_typecheck(d->next);
}

void decl_global_codegen(struct decl *d) {
	if (!d) return;

	// variable declaration
	if (d->value) {
		switch(d->symbol->type->kind) {
			case TYPE_INTEGER:
				fprintf(outfile, "\t%s: .quad %d\n", d->symbol->name, d->value->literal_value);
				break;
			case TYPE_STRING:
				fprintf(outfile, "\t%s: .string %s\n", d->symbol->name, d->value->string_literal);
				break;
			case TYPE_BOOLEAN:
				fprintf(outfile, "\t%s: .quad %d\n", d->symbol->name, d->value->literal_value);
				break;
			case TYPE_CHARACTER:;
				char *c = strdup(d->value->char_literal);
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
				fprintf(outfile, "\t%s: .quad %d\n", d->symbol->name, res);
				break;
		}
	}

	decl_global_codegen(d->next);
}

void decl_codegen(struct decl *d) {
	if (!d) return;

	if (d->type->kind == TYPE_FUNCTION) {
		if (d->code) { // function definition
			num_curr_decls = 0;
			decl_count(d->code->body);
			// printf("num decls = %d\n", num_curr_decls);
			// name 
			fprintf(outfile, ".globl %s\n%s:\n", d->name, d->name);
			
			/* ++++ preamble ++++ */
			// save rbp and update new rbp
			fprintf(outfile, "\tpushq %%rbp\n\tmovq %%rsp, %%rbp\n");

			// arguments
			param_list_codegen(d->type->params);

			// make stack space for local variables
			fprintf(outfile, "\tsubq $%d, %%rsp\n", num_curr_decls * 8);

			// callee save registers
			fprintf(outfile, "\tpushq %%rbx\n\tpushq %%r12\n\tpushq %%r13\n\tpushq %%r14\n\tpushq %%r15\n");

			current_function_name = d->name;	
			// body of the function
			stmt_codegen(d->code);

			/* ++++ epilogue ++++ */
			fprintf(outfile, ".%s_epilogue:\n", current_function_name);

			// restore callee save registers
			fprintf(outfile, "\tpopq %%r15\n\tpopq %%r14\n\tpopq %%r13\n\tpopq %%r12\n\tpopq %%rbx\n");

			// restore stack pointer
			fprintf(outfile, "\tmovq %%rbp, %%rsp\n");

			// restore old base pointer and return
			fprintf(outfile, "\tpopq %%rbp\n\tret\n");

		}
	} else if (d->symbol->kind == SYMBOL_LOCAL) { // local variable declarations	
		
		if (d->value) { // evaluate expression
			expr_codegen(d->value);	

			// put result on stack
			fprintf(outfile, "\tmovq %s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));

			// free reg
			scratch_free(d->value->reg);
		} else {
			// blank
			
			switch (d->type->kind) {
				case TYPE_INTEGER:
					fprintf(outfile, "\tmovq $0, %s\n", symbol_codegen(d->symbol));
					break;
				case TYPE_BOOLEAN:
					fprintf(outfile, "\tmovq $0, %s\n", symbol_codegen(d->symbol));
					break;
				case TYPE_CHARACTER:
					fprintf(outfile, "\tmovq $0, %s\n", symbol_codegen(d->symbol));
					break;
				case TYPE_STRING:;
					int str_label = label_create();
					int str_reg = scratch_alloc();
					fprintf(outfile, ".data\n%s:\n\t.string \"\"\n.text\n\tleaq %s, %s\n", label_name(str_label), label_name(str_label), scratch_name(str_reg));
					fprintf(outfile, "\tmovq %s, %s\n", scratch_name(str_reg), symbol_codegen(d->symbol));
					scratch_free(str_reg);
					break;
			}
		}	
	}

	decl_codegen(d->next);
}


void decl_count(struct stmt *s) {
	if (!s) return;

	switch(s->kind) {
		case STMT_DECL:
			num_curr_decls++;
			break;
		case STMT_FOR:
			decl_count(s->body);
			break;
		case STMT_IF_ELSE:
			decl_count(s->body);
			decl_count(s->else_body);
			break;
		case STMT_BLOCK:
			decl_count(s->body);
			break;
	}

	decl_count(s->next);
}


