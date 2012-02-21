#include <stdio.h>
#include "class.h"

/*
 * This class attempts to create a base class and an inherited
 * class, and through the use of vtables, call functions from
 * both and show that the functions are inherited properly */

/* the base class */
CLASS(base) {
	/* Methods:
	 * void setX(int x) {this->x = x;}
	 * void resetX() {this->x = 10;}
	 * int getX() {return this->x;}
	 */

	/* the vtable */
	void* vtable;

	/* the data variable */
	int x;
};

/* definitions of the base class methods */
void CLASS_METHOD_DEF(base, setX, int x) {
	this->x = x;
}

void CLASS_METHOD_DEF(base, resetX) {
	this->x = 10;
}

int CLASS_METHOD_DEF(base, getX) {
	return this->x;
}

/* the inherited class */
CLASS(inherit) {
	/* Methods:
	 * void resetX() {this->x = 5;}
	 */

	/* the vtable of the base */
	void* base_vtable;

	/* the same data variable */
	int x;

	/* the inherit vtable */
	void* vtable;

	/* I need to come up with a way for the data variables
	 * from base classes are automatically accessible from
	 * inherited classes, without redefining them
	 *
	 * I also need to find a way for a type of an inherited
	 * class to be cast as one of the base class
	 *
	 * I looked online, and it said that classes are stored like
	 * <base class 1 vtable>
	 * <base class 1 data>
	 * <base class 2 vtable>
	 * <base class 2 data>
	 * <my vtable>
	 * <my data>
	 *
	 * So then to cast to base class 1, nothing needs to be done,
	 * to cast to base class 2, we simply shift the pointer
	 * up to the point where the base class 2 vtable starts
	 *
	 * that works, but then I need a way to dynamically
	 * find out where the base class 2 vtable pointer is
	 */
};

/* definitions of the inherited class methods */
void CLASS_METHOD_DEF(inherit, resetX) {
	this->x = 5;
}

#define ADD_CLASS_FUNC(TYPE, FUNC) add_node(&(this->vtable), new_func_node(CLASS_METHOD_NAME(TYPE, FUNC), #FUNC))

#define CLASS_CONSTRUCT_NAME(TYPE) TYPE##_construct
#define CLASS_CONSTRUCT(TYPE) void* CLASS_CONSTRUCT_NAME(TYPE)(CLASS(TYPE)* this)

CLASS_CONSTRUCT(base) {
	ADD_CLASS_FUNC(base, setX);
	ADD_CLASS_FUNC(base, resetX);
	ADD_CLASS_FUNC(base, getX);

	this->x = 10;

	return this;
}

#define ADD_CLASS_INHERIT(TYPE, BASE) BASE##_construct((CLASS(BASE)*)&(this->BASE##_vtable)); add_node(&(this->vtable), new_vtable_node(this->BASE##_vtable))

CLASS_CONSTRUCT(inherit) {
	ADD_CLASS_INHERIT(inherit, base);
	ADD_CLASS_FUNC(inherit, resetX);

	this->x = 5;

	return this;
}

#define FUNC_CALL(X, RETURN, FUNC, ...) ((typeof(RETURN) (*) ())find_func((X)->vtable,str_hash(#FUNC)))((X), ##__VA_ARGS__)

int main(int argc, char* argv[]) {
	/* allocate a base and an inherit class */
	CLASS(base)* b = NEW(base);
	CLASS(inherit)* i = NEW(inherit);

	FUNC_CALL(b, void, setX, 2);
	FUNC_CALL(i, void, setX, 2);

	printf("%d %d\n", b->x, i->x);

	FUNC_CALL(b, void, resetX);
	FUNC_CALL(i, void, resetX);

	printf("%d %d\n", b->x, i->x);

	int c = FUNC_CALL(b, int, getX);
	int d = FUNC_CALL(i, int, getX);

	printf("%d %d\n", c, d);
}
