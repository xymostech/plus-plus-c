#ifndef CLASS_H
#define CLASS_H

#include <stdlib.h>

/* Class.h
 * 
 * An attempt to use a ton of macros and structures to produce
 * C++ style classes and inheritance
 */

/* Class keyword */
#define CLASS(TYPE) struct TYPE

/* Macros for referencing class methods */
#define CLASS_METHOD_NAME(TYPE, METHOD) TYPE##_##METHOD##_def
#define CLASS_METHOD_DEF(TYPE, METHOD, ...) TYPE##_##METHOD##_def(CLASS(TYPE)* this, ##__VA_ARGS__)

/* Macros to define the constructor */
#define CLASS_CONSTRUCT_NAME(TYPE) TYPE##_construct
#define CLASS_CONSTRUCT(TYPE) void* CLASS_CONSTRUCT_NAME(TYPE)(CLASS(TYPE)* this)
#define CLASS_CONSTRUCT_END() return this

/* Macros to add functions and inheritance to class constructors */
#define ADD_CLASS_INHERIT(TYPE, BASE) CLASS_CONSTRUCT_NAME(BASE)((CLASS(BASE)*)&(this->BASE##_vtable)); add_node_end(&(this->vtable), new_vtable_node(this->BASE##_vtable, &(this->BASE##_vtable)))
#define ADD_CLASS_FUNC(TYPE, FUNC) add_node(&(this->vtable), new_func_node(CLASS_METHOD_NAME(TYPE, FUNC), #FUNC))

/* Macro to call functions of a class on an instance */
#define CLASS_FUNC_CALL(X, RETURN, FUNC, ...) ((typeof(RETURN) (*) ())find_func((X)->vtable,str_hash(#FUNC),&base_pos,(X)))(base_pos, ##__VA_ARGS__)

/* Macros to create and destroy classes */
#define NEW(TYPE) CLASS_CONSTRUCT_NAME(TYPE)((CLASS(TYPE)*)malloc(sizeof(CLASS(TYPE)))) 
#define CONSTRUCT(TYPE) *NEW(TYPE)
#define DELETE(X) free((X)->vtable); free(X)

/* A function which hashes a string
 * mainly used on function names */
unsigned str_hash(char* str) {
	unsigned a = 1;
	while(*str) a = (*str++) * a + 1;
	return a;
}


/* linked list nodes for holding vtables */

/* a node which exposes the type and next of each node */
struct general_node {
	int type;
	void* next;
};

/* function nodes, which store a function in a vtable */
#define FUNC_NODE_TYPE 0
struct func_node {
	int type;
	void* next;
	void* func;
	unsigned name_hash;
};

/* vtable nodes, which store a list of functions/vtables */
#define VTABLE_NODE_TYPE 1
struct vtable_node {
	int type;
	void* next;
	void* funcs;
	void* pos;
};

/* functions for creating new func nodes,
 * new vtable nodes, and for searching through a
 * node list for a function, by its hasn
 */
void* new_func_node(void* func, char* func_name) {
	struct func_node* node = malloc(sizeof(*node));
	node->type = FUNC_NODE_TYPE;
	node->func = func;
	node->name_hash = str_hash(func_name);
	return node;
}

void* new_vtable_node(void* funcs, void* pos) {
	struct vtable_node* node = malloc(sizeof(*node));
	node->type = VTABLE_NODE_TYPE;
	node->funcs = funcs;
	node->pos = pos;
	return node;
}

void add_node(void** root, struct general_node* add) {
	add->next = *root;
	*root = add;
}

void add_node_end(void** root, struct general_node* add) {
	if(!(*root)) {
		*root = add;
	} else {
		struct general_node* node = *root;
		while(node->next) node = node->next;
		node->next = add;
	}
}

void* find_func(struct general_node* root, unsigned hash, void** pos, void* curr_pos) {
	void* ret;
	while(root) {
		if(root->type == FUNC_NODE_TYPE) {
			if(((struct func_node*)root)->name_hash == hash) {
				*pos = curr_pos;
				return ((struct func_node*)root)->func;
			}
		} else {
			if(ret = find_func(((struct vtable_node*)root)->funcs, hash, pos, ((struct vtable_node*)root)->pos)) {
				return ret;
			}
		}
		root = root->next;
	}
	return NULL;
}

void* base_pos;

#endif
