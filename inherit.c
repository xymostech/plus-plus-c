#include <stdio.h>
#include "class.h"

CLASS(base) {
	/*CLASS_METHODS_START(test);*/
	/*void CLASS_METHOD(test, setX, int);*/
	/*int CLASS_METHOD(test, resetX);*/
	/*CLASS_METHODS_END();*/

	/* Methods:
	 * void setX(int x) {this->x = x;}
	 * void resetX() {this->x = 10;}
	 * int getX() {return this->x;}
	 */

	int x;
	void* vtable;
};

void CLASS_METHOD_DEF(base, setX, int x) {
	this->x = x;
}

void CLASS_METHOD_DEF(base, resetX) {
	this->x = 10;
}

int CLASS_METHOD_DEF(base, getX) {
	return this->x;
}

CLASS(inherit) {
	/* Methods:
	 * void resetX() {this->x = 5;}
	 */

	int x;
	void* vtable;
};

void CLASS_METHOD_DEF(inherit, resetX) {
	this->x = 5;
}

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

void* base_vtable_construct() {
	void* root;
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, setX), CLASS_METHOD_NAME_STR(base, setX)));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, resetX), CLASS_METHOD_NAME_STR(base, resetX)));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(base, getX), CLASS_METHOD_NAME_STR(base, getX)));
	return root;
}

void* inherit_vtable_construct() {
	void* root;
	add_node(&root, new_vtable_node(base_vtable_construct()));
	add_node(&root, new_func_node(CLASS_METHOD_NAME(inherit, resetX), CLASS_METHOD_NAME_STR(inherit, resetX)));
	return root;
}

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
