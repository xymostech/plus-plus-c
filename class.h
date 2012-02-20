#include <stdlib.h>

#define CLASS(TYPE) struct TYPE

#define CLASS_METHODS_START(TYPE) struct TYPE##_vtable {
#define CLASS_METHOD(TYPE, METHOD, ...) (* METHOD )(CLASS(TYPE)*, ##__VA_ARGS__)
#define CLASS_METHODS_END() } *vtable


#define CLASS_METHOD_NAME(TYPE, METHOD) TYPE##_##METHOD##_def
#define CLASS_METHOD_NAME_STR(TYPE, METHOD) #TYPE "_" #METHOD "_def"
#define CLASS_METHOD_DEF(TYPE, METHOD, ...) TYPE##_##METHOD##_def(CLASS(TYPE)* this, ##__VA_ARGS__)


#define CLASS_INIT_DEF(TYPE, ...) CLASS(TYPE)* TYPE##_init(CLASS(TYPE)* this, ##__VA_ARGS__)
#define CLASS_INIT_BEGIN(TYPE) this->vtable = malloc(sizeof(struct TYPE##_vtable))
#define CLASS_INIT_END() return this
#define CLASS_METHOD_INIT(TYPE, METHOD) this->vtable->METHOD = TYPE##_##METHOD##_def


#define CLASS_INIT(TYPE) TYPE##_init
#define CLASS_CALL(X, METHOD, ...) (X).vtable->METHOD(&(X), ##__VA_ARGS__)


#define NEW(TYPE) CLASS_INIT(TYPE)((CLASS(TYPE)*)malloc(sizeof(CLASS(TYPE)))) 
#define CONSTRUCT(TYPE) *NEW(TYPE)
#define DELETE(X) free((X)->vtable); free(X)

unsigned str_hash(char* str) {
	unsigned a = 1;
	while(*str) a = (*str++) * a + 1;
	return a;
}

struct general_node {
	int type;
	void* next;
};

#define FUNC_NODE_TYPE 1
struct func_node {
	int type;
	void* next;
	void* func;
	unsigned name_hash;
};

#define VTABLE_NODE_TYPE 0
struct vtable_node {
	int type;
	void* next;
	void* funcs;
};
