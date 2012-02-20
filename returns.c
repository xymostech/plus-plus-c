#include <stdio.h>

/* An attempt to make dynamically accessible functions
 *
 * For example, if there is a list of functions, each taking
 * different amounts of arguments and returning different
 * values, then I can use these macros to access each one correctly
 * and allow it to be used as a normal function
 *
 * This doesn't completely solve the problem of a list of
 * functions, because having a structure type entirely devoted to
 * a specific function defeats the purpose of having more than one
 * function of the same name...
 *
 * Actually, they can't have the same name. However, it would be
 * impossible to dynamically figure out what name it had been stored
 * under, and retrieve a structure of a specific type based on that
 * name
 */

/* structure for holding a function, and its type */
struct default_func { void* a; int b; };

/* macros that sets up a function struct */
#define GET_FUNC(NAME) NAME##_func
#define SETUP_FUNC(NAME, RETURN, ...) struct GET_FUNC(NAME) { RETURN (*func)(__VA_ARGS__); RETURN x; } GET_FUNC(NAME) = {NAME}

/* macro that returns the function stored in the func structure */
#define FUNC_CALL(FUNC) ((typeof(GET_FUNC(FUNC).x) (*) ()) GET_FUNC(FUNC).func)

/* a default function */
int square(int x) {
	return x * x;
}

/* set up the function structure */
SETUP_FUNC(square, int, int);

int main(int argc, char* argv[]) {
	/* get the square function, and call it with (2) */
	int x = FUNC_CALL(square)(2);

	/* print the result (should be 4) */
	printf("%d\n", x);
}
