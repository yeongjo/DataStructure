#include <stdio.h>
#include <stdlib.h>

typedef int Element;

void printElement(Element e) {
	printf("%d \t", e);
}

typedef struct _Vector {
	Element *p;
	int size;
	int maxsize;
}Vector;

void Vector_init(Vector *v) {
	v->maxsize = 16;
	v->p = (Element*)malloc(sizeof(Element) * v->maxsize);
	v->size = 0;
}

void Vector_append(Vector *v, Element e) {
	if (v->size == v->maxsize) {
		v->maxsize *= 2;
		v->p = (Element*)realloc(v->p, sizeof(Element) * v->maxsize);
	}
	v->p[v->size] = e;
	v->size++;
}

void Vector_insertAt(Vector* v, Element e, int index) {
	if (v->size <= index) {
		return;
	}
	if (v->size == v->maxsize) {
		v->maxsize *= 2;
		v->p = (Element*)realloc(v->p, sizeof(Element) * v->maxsize);
	}
	for (int i = v->size - 1; i >= index; i--) {
		v->p[i + 1] = v->p[i];
	}
	v->p[index] = e;
	v->size++;
}

void Vector_deleteAt(Vector *v, int index) {
	if (v->size <= index) {
		return;
	}
	for (int i = index + 1; i < v->size; i++) {
		v->p[i - 1] = v->p[i];
	}
	v->size--;
}

Element Vector_getAt(Vector *v, int index) {
	if (index < 0 || index >= v->size) return 0;
	return v->p[index];
}

void Vector_setAt(Vector* v, Element e, int index) {
	if (index < 0 || index >= v->size) return;
	v->p[index] = e;
}

void Vector_print(Vector *v) {
	printf("[%d %d]", v->size, v->maxsize);
	for (int i = 0; i < v->size; i++) {
		printElement(v->p[i]);
	}
	printf("\n");
}

void Vector_destroy(Vector *v) {
	v->maxsize = 0;
	v->size = 0;
	free(v->p);
	v->p = NULL;
}

int main() {
	Vector v;
	Vector_init(&v);
	for (size_t i = 0; i < 3; i++) {
		Vector_append(&v, 10*(i+1));
	}
	Vector_print(&v);

	Vector_insertAt(&v, -50, 1);
	Vector_print(&v);

	Vector_deleteAt(&v, 2);
	Vector_print(&v);

	Vector_setAt(&v, -100, 2);
	Vector_print(&v);

	printf("%d\n",Vector_getAt(&v, 2));
	Vector_print(&v); 

	Vector_destroy(&v);
	return 0;
}