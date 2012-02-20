#include <stdio.h>
#include "class.h"

/* Currently, this file is a test of basic class definitions, and
 * testing of the class capabilities
 *
 * it provides no sort of inheritance
 */

/* a testing class */
CLASS(test) {
	/* It has two functions */
	CLASS_METHODS_START(test);
	void CLASS_METHOD(test, setX, int);
	int CLASS_METHOD(test, resetX);
	CLASS_METHODS_END();

	/* and one member variable */
	int x;
};

/* define setX */
void CLASS_METHOD_DEF(test, setX, int x) {
	this->x = x;
}

/* define resetX */
int CLASS_METHOD_DEF(test, resetX) {
	this->x = 10;
	return 10;
}

/* create the "constructor" */
CLASS_INIT_DEF(test) {
	CLASS_INIT_BEGIN(test);
	/* set the default value */
	this->x = 10;
	/* add the methods to the constructor */
	CLASS_METHOD_INIT(test, setX);
	CLASS_METHOD_INIT(test, resetX);
	CLASS_INIT_END();
}

int main(int argc, char* argv[]) {
	/* initialize two test classes, one on stack,
	 * one in heap */
	CLASS(test) a = CONSTRUCT(test);
	CLASS(test)* b = NEW(test);

	/* call setX on both */
	CLASS_CALL(a, setX, 1);
	CLASS_CALL(*b, setX, 2);

	/* print their x values */
	printf("%d, %d\n", a.x, b->x);

	/* call resetX on one */
	CLASS_CALL(a, resetX);

	/* print the result */
	printf("%d, %d\n", a.x, b->x);

	/* cleanup the one on the heap 
	 *
	 * actually, the one on the stack should probably be
	 * cleaned up as well, because it currently contains a
	 * dynamically allocated virtual function table,
	 * but that can come later. */
	DELETE(b);
}
