#include <stdlib.h>

typedef struct link_elem_s link_elem_t;
typedef struct link_elem_s {
	link_elem_t *other;
	void *data;
} link_elem_t;

typedef struct {
	link_elem_t *top;
} stack_t;

void fail(char *r);

stack_t* stack_new();
void stack_push(stack_t *s, void *d);
void alloc_stack_free(stack_t *s);

void garbo_track();
void *garbo_alloc(size_t bytes);
void garbo_free();

#ifdef GARBOMAN_IMPL

void fail(char *r) {
	fputs(r, stderr);
	exit(EXIT_FAILURE);
}

stack_t* stack_new() {
	stack_t *s = calloc(1, sizeof(stack_t));
	if (!s) fail("stack_new: allocation failed");
	return s;
}

void stack_push(stack_t *s, void *d) {
	if (!s || !d) fail("stack_push: stack or data invalid");

	link_elem_t *le = malloc(sizeof(link_elem_t));
	if (!le) fail("stack_push: allocation of link element failed");

	le->data = d;
	le->other = s->top;
	s->top = le;
}

void alloc_stack_free(stack_t *s) {
	if (!s) fail("alloc_stack_free: stack invalid");

	link_elem_t *e = s->top, *o;
	while (e) {
		o = e->other;
		free(e->data);
		free(e);
		e = o;
	}

	free(s);
}

stack_t *ref_stack;

void garbo_track() {
	if (!ref_stack) ref_stack = stack_new();
	stack_push(ref_stack, stack_new());
}

void* garbo_alloc(size_t bytes) {
	if (!ref_stack) fail("garbo_alloc: reference stack invalid");
	if (!ref_stack->top) fail("garbo_alloc: reference stack empty");

	void *r = malloc(bytes);
	if (!r) fail("garbo_alloc: allocation failed");

	stack_push((stack_t*)ref_stack->top->data, r);
	return r;
}

void garbo_free() {
	if (!ref_stack) return;
	if (!ref_stack->top) return;
	
	link_elem_t *o = ref_stack->top->other;
	alloc_stack_free(ref_stack->top->data);
	free(ref_stack->top);
	ref_stack->top = o;
}

#endif

