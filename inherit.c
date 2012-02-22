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

/* constructor for base */
CLASS_CONSTRUCT(base) {
	ADD_CLASS_FUNC(base, setX);
	ADD_CLASS_FUNC(base, resetX);
	ADD_CLASS_FUNC(base, getX);

	this->x = 10;

	CLASS_CONSTRUCT_END();
}

/* the second base class of inherit */
CLASS(base2) {
	void* vtable;

	int y;
};

void CLASS_METHOD_DEF(base2, setY, int y) {
	this->y = y;
}

CLASS_CONSTRUCT(base2) {
	ADD_CLASS_FUNC(base2, setY);
	
	this->y = 15;

	CLASS_CONSTRUCT_END();
}

#define INHERIT(TYPE) void* TYPE##_vtable;

/* the inherited class */
CLASS(inherit) {
	/* Methods:
	 * void resetX() {this->x = 5;}
	 */

	/* the vtable of the base */
	INHERIT(base);

	/* the same data variable */
	int x;

	INHERIT(base2);

	int y;

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

CLASS_CONSTRUCT(inherit) {
	ADD_CLASS_INHERIT(inherit, base);
	ADD_CLASS_INHERIT(inherit, base2);
	ADD_CLASS_FUNC(inherit, resetX);

	this->x = 5;

	CLASS_CONSTRUCT_END();
}

int main(int argc, char* argv[]) {
	/* allocate a base and an inherit class */
	CLASS(base)* b = NEW(base);
	CLASS(inherit)* i = NEW(inherit);

	CLASS_FUNC_CALL(b, void, setX, 2);
	CLASS_FUNC_CALL(i, void, setY, 2);

	printf("%d %d %d\n", b->x, i->x, i->y);

	CLASS_FUNC_CALL(b, void, resetX);
	CLASS_FUNC_CALL(i, void, resetX);

	printf("%d %d\n", b->x, i->x);

	int c = CLASS_FUNC_CALL(b, int, getX);
	int d = CLASS_FUNC_CALL(i, int, getX);

	printf("%d %d\n", c, d);
}
