#include "scope.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct scope *scope_top = NULL;
extern int typecheck_failure;

void scope_enter() {
    struct scope *scope_node = malloc(sizeof(struct scope));

    scope_node->hash_table = hash_table_create(0,0);
    scope_node->next = scope_top;
    scope_top = scope_node;

}

void scope_exit() {
    // error handling
    if (!scope_top) {
        printf("Scope exit error: no tables exist\n");
        exit(1);
    }
    
    // removing top most table
    struct scope *t = scope_top;
    scope_top = t->next;

    hash_table_delete(t->hash_table);
    free(t);
}

int scope_level() {
    if (!scope_top) {
        return 0;
    }
    int count = 0;
    struct scope *t = scope_top;

    while (t) {
        count++;
        t = t->next;
    }

    return count;
}

void scope_bind( const char *name, struct symbol *sym) {
    struct scope *t = scope_top;

    int insert_res = hash_table_insert(t->hash_table, name, sym);

    if (insert_res != 1) {
        // insert failed --> key is already in hash table
		printf("resolve error: multiple declarations of %s\n", name);
		typecheck_failure = 1;
    }
}


struct symbol *scope_lookup(const char *name) {
    struct scope *t = scope_top;
    if (!t) {
        printf("scope lookup current error: no hashtables exist\n");
        exit(1);
    }

    char **key = malloc(128);
    void **value = malloc(sizeof(struct symbol));
    int result;
    while (t) {
        hash_table_firstkey(t->hash_table); 

        while (1) {
            result = hash_table_nextkey(t->hash_table, key, value);

            if (!result) {
                break;
            } 

            if (!strcmp(*key, name)) {
                // found key
                struct symbol *s = (struct symbol *) hash_table_lookup(t->hash_table, *key);

                free(key);
                free(value);
                return s;
            }
        }

        t = t->next;
    }

    free(key);
    free(value);
    // no match found
    return NULL;    
}

struct symbol *scope_lookup_current(const char * name) {
    struct scope *t = scope_top;
    if (!t) {
        printf("scope lookup current error: no hashtables exist\n");
        exit(1);
    }


    // start iteration
    hash_table_firstkey(t->hash_table);
    char **key = malloc(128);
    void **value = malloc(sizeof(struct symbol));
    int result;
    while (1) {
        result = hash_table_nextkey(t->hash_table, key, value);

        if (!result) {
            break;
        }

        if (!strcmp(*key, name)) {
            // found key
            struct symbol *s = (struct symbol *) hash_table_lookup(t->hash_table, *key);
            
            free(key);
            free(value);

            return s;
        }
    }

    free(key);
    free(value);

    // key not found
    return NULL;
}
