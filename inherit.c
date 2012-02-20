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

/* construct a vtable for the base function */
void* base_vtable_construct() {
	void* root;
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, setX), CLASS_METHOD_NAME_STR(base, setX)));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, resetX), CLASS_METHOD_NAME_STR(base, resetX)));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, getX), CLASS_METHOD_NAME_STR(base, getX)));
	return root;
}

/* construct a vtable for the inherited function */
void* inherit_vtable_construct() {
	void* root;
	add_node(&root, new_vtable_node(base_vtable_construct()));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(inherit, resetX), CLASS_METHOD_NAME_STR(inherit, resetX)));
	return root;
}

/* the base class constructor */
CLASS_INIT_DEF(base) {
	/*CLASS_INIT_BEGIN(test);*/
	this->x = 10;

	/*CLASS_METHOD_INIT(test, setX);*/
	/*CLASS_METHOD_INIT(test, resetX);*/
	/*CLASS_INIT_END();*/
}

int main(int argc, char* argv[]) {
	/*CLASS(test) a = CONSTRUCT(test);*/
	/*CLASS(test)* b = NEW(test);*/

	/*CLASS_CALL(a, setX, void, 1);*/
	/*CLASS_CALL(*b, setX, void, 2);*/

	/*printf("%d, %d\n", a.x, b->x);*/

	/*CLASS_CALL(a, resetX, void);*/

	/*printf("%d, %d\n", a.x, b->x);*/

	/*DELETE(b);*/
}
