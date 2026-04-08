#include <stdio.h>
#include <stdlib.h>

#define GARBOMAN_IMPL
#include "garboman.h"

void alloc_stack_print(stack_t *as) {
	int i = 0;
	for (link_elem_t *e = as->top; e != NULL; e = e->other) {
		printf("    %4d: %d@%p\n", i, *(int*)(e->data), (int*)e->data);
		i++;
	}
}

void ref_stack_print() {
	puts("refs:");
	int i = 0;
	for (link_elem_t *e = ref_stack->top; e != NULL; e = e->other) {
		printf("alloc stack %d\n", i);
		alloc_stack_print(e->data);
		i++;
	}
	puts("");
}

int main() {
	garbo_track();
	ref_stack_print();

	garbo_track();
	ref_stack_print();

	garbo_track();
	ref_stack_print();

	garbo_track();
	ref_stack_print();

	int *foo = garbo_alloc(sizeof(int));
	*foo = 42;
	int *bar = garbo_alloc(sizeof(int));
	*bar = 69;
	ref_stack_print();

	garbo_free();
	ref_stack_print();

	garbo_free();
	ref_stack_print();

	return 0;
}

