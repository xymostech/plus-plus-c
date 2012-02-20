#include <stdlib.h>

/* Class.h
 * 
 * An attempt to use a ton of macros and structures to produce
 * C++ style classes and inheritance
 */

/* Class keyword */
#define CLASS(TYPE) struct TYPE

/* Macros for initializing class methods */
#define CLASS_METHODS_START(TYPE) struct TYPE##_vtable {
#define CLASS_METHOD(TYPE, METHOD, ...) (* METHOD )(CLASS(TYPE)*, ##__VA_ARGS__)
#define CLASS_METHODS_END() } *vtable

/* Macros for referencing class methods */
#define CLASS_METHOD_NAME(TYPE, METHOD) TYPE##_##METHOD##_def
#define CLASS_METHOD_NAME_STR(TYPE, METHOD) #TYPE "_" #METHOD "_def"
#define CLASS_METHOD_DEF(TYPE, METHOD, ...) TYPE##_##METHOD##_def(CLASS(TYPE)* this, ##__VA_ARGS__)

/* Macros to define the constructor */
#define CLASS_INIT(TYPE) TYPE##_init
#define CLASS_INIT_DEF(TYPE, ...) CLASS(TYPE)* CLASS_INIT(TYPE)(CLASS(TYPE)* this, ##__VA_ARGS__)
#define CLASS_INIT_BEGIN(TYPE) this->vtable = malloc(sizeof(struct TYPE##_vtable))
#define CLASS_INIT_END() return this
#define CLASS_METHOD_INIT(TYPE, METHOD) this->vtable->METHOD = TYPE##_##METHOD##_def

/* Macro to call functions of a class on an instance */
#define CLASS_CALL(X, METHOD, ...) (X).vtable->METHOD(&(X), ##__VA_ARGS__)

/* Macros to create and destroy classes */
#define NEW(TYPE) CLASS_INIT(TYPE)((CLASS(TYPE)*)malloc(sizeof(CLASS(TYPE)))) 
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

void* new_vtable_node(void* funcs) {
	struct vtable_node* node = malloc(sizeof(*node));
	node->type = VTABLE_NODE_TYPE;
	node->funcs = funcs;
	return node;
}

void add_node(void** root, struct general_node* add) {
	add->next = *root;
	*root = add;
}

void* find_func(struct general_node* root, unsigned hash) {
	void* ret;
	while(root) {
		if(root->type == FUNC_NODE_TYPE) {
			if(((struct func_node*)root)->name_hash == hash) {
				return ((struct func_node*)root)->func;
			}
		} else {
			if(ret = find_func(((struct vtable_node*)root)->funcs, hash)) {
				return ret;
			}
		}
		root = root->next;
	}
	return NULL;
}
