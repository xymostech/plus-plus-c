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
	/* the second vtable */

	CLASS(base) base;

	void* vtable;

	/* the same data variable */
	int x;

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

#define ADD_CLASS_FUNC(TYPE, FUNC) add_node(&(this->vtable), new_func_node(CLASS_METHOD_NAME(TYPE, FUNC), CLASS_METHOD_NAME_STR(TYPE, FUNC)))

/* construct a vtable for the base function */
void* base_construct(CLASS(base)* this) {
	ADD_CLASS_FUNC(base, setX);
	ADD_CLASS_FUNC(base, resetX);
	ADD_CLASS_FUNC(base, getX);

	this->x = 10;

	return this;
}

#define ADD_CLASS_INHERIT(TYPE, BASE) BASE##_construct(&(this->BASE)); add_node(&(this->vtable), new_vtable_node(this->BASE.vtable))

/* construct a vtable for the inherited function */
void* inherit_construct(CLASS(inherit)* this) {
	base_construct(&(this->base));
	ADD_CLASS_INHERIT(inherit, base);
	ADD_CLASS_FUNC(inherit, resetX);

	this->x = 5;

	return this;
}

int main(int argc, char* argv[]) {
	/* allocate a base and an inherit class */
	CLASS(base)* b = base_construct((CLASS(base)*)malloc(sizeof(*b)));
	CLASS(inherit)* i = inherit_construct((CLASS(inherit)*)malloc(sizeof(*i)));
}
