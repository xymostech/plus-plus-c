#include <stdio.h>

struct default_func { void* a; int b; };

#define SETUP_FUNC(NAME, RETURN, ...) struct NAME##_func { RETURN (*func)(__VA_ARGS__); RETURN x; } NAME##_func = {NAME}
#define GET_FUNC(NAME) NAME##_func

#define FUNC_CALL(FUNC) ((typeof(GET_FUNC(FUNC).x) (*) ()) get_func((struct default_func*)&GET_FUNC(FUNC)))

void* get_func(struct default_func* f) {
	return f->a;
}

int square(int x) {
	return x * x;
}

SETUP_FUNC(square, int, int);

int main(int argc, char* argv[]) {
	int x = FUNC_CALL(square)(2);

	printf("%d\n", x);
}
