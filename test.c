#include <stdio.h>
#include "class.h"

CLASS(test) {
	CLASS_METHODS_START(test);
	void CLASS_METHOD(test, setX, int);
	int CLASS_METHOD(test, resetX);
	CLASS_METHODS_END();

	int x;
};

void CLASS_METHOD_DEF(test, setX, int x) {
	this->x = x;
}

int CLASS_METHOD_DEF(test, resetX) {
	this->x = 10;
	return 10;
}

CLASS_INIT_DEF(test) {
	CLASS_INIT_BEGIN(test);
	this->x = 10;
	CLASS_METHOD_INIT(test, setX);
	CLASS_METHOD_INIT(test, resetX);
	CLASS_INIT_END();
}

int main(int argc, char* argv[]) {
	CLASS(test) a = CONSTRUCT(test);
	CLASS(test)* b = NEW(test);

	CLASS_CALL(a, setX, 1);
	CLASS_CALL(*b, setX, 2);

	printf("%d, %d\n", a.x, b->x);

	CLASS_CALL(a, resetX);

	printf("%d, %d\n", a.x, b->x);

	DELETE(b);
}
