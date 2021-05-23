#include <stdio.h>

typedef struct {
	int degree;
	float coef;
}Term;

typedef Term Element;

typedef struct _Node {
	Element data;
	struct _Node *next;
}Node;

typedef struct {
	Node *head;
	int size;
}LList;

typedef LList SparsePoly;

LList LList_create() {
	LList list = { NULL };
	list.size = 0;
	list.head = NULL;
	return list;
}

void LList_pushRear(LList * list, Element e) {
	Node *node = (Node*)malloc(sizeof(Node));
	Node *t = list->head;
	node->data = e;
	node->next = NULL;
	if (t) {
		while (t->next) {
			t = t->next;
		}
		t->next = node;
	} else
		list->head = node;

	list->size++;
}

void LList_push(LList * list, Element e) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = e;
	node->next = list->head;
	list->head = node;
	list->size++;
}

Element LList_pop(LList * list) {
	Element e = list->head->data;
	Node *head = list->head;
	list->head = head->next;
	list->size--;
	return e;
}

void LList_removeElement(LList *list, Element e) {
	Node *t1;
	Node *prev = NULL;
	int t1Count;
	for (t1 = list->head, t1Count = 0; t1; t1 = t1->next, ++t1Count) {
		if (t1->data.coef == e.coef && t1->data.degree == e.degree) {
			if (!prev) { // 이전것이 없다면
				list->head = t1->next;
			} else { // 이전것이 있다면
				prev->next = t1->next;
			}
			return;
		}
		prev = t1;
	}
}

int LList_isEmpty(LList *list) {
	return list->size == 0;
}

void LList_print(LList *list) {
	Node *t = list->head;
	while (t) {
		printf("%.1fx^%d", t->data.coef, t->data.degree);
		t = t->next;
		if (t)
			printf("\t + ");
	}
	printf("\n");
}

void print_poly(const SparsePoly* p, const char str[]) {
	printf("\t%s", str);
	LList_print(p);
}

void read_poly(SparsePoly *list) {
	int a;
	float b;
	*list = LList_create();
	while (1) {
		printf("계수 차수 입력(종료:-1 -1): ");
		scanf("%f %d", &b, &a);
		if (a == -1 && b == -1.f)
			break;
		Term t = { a, b };
		LList_pushRear(list, t);
	}
	print_poly(list, "입력 다항식: ");
}

void add_poly(SparsePoly* dest, const SparsePoly* a, const SparsePoly* b) {
	Node *t1;
	Node *t2;
	int t1Count;
	int t2Count;
	Element t1AddedArr[40] = { 0 };
	Element t2AddedArr[40] = { 0 };
	
	Term d;

	*dest = LList_create();

	for (t1 = a->head, t1Count = 0; t1; t1 = t1->next, ++t1Count) {
		for (t2 = b->head, t2Count = 0; t2; t2 = t2->next, ++t2Count) {
			if (!t1 || !t2) break;

			int degree1 = t1->data.degree;
			int degree2 = t2->data.degree;

			if (degree1 == degree2) {
				d.coef = t1->data.coef + t2->data.coef;
				d.degree = degree1;
				if(t2AddedArr[t2Count].coef)
					LList_removeElement(a, t2AddedArr[t2Count]);
				if (t1AddedArr[t1Count].coef)
					LList_removeElement(a, t1AddedArr[t1Count]);
				t2AddedArr[t2Count] = t1AddedArr[t1Count] = d;
				LList_pushRear(dest, d);
			} else {
				if (!t1AddedArr[t1Count].coef) {
					d.coef = t1->data.coef;
					d.degree = degree1;
					t1AddedArr[t1Count] = d;
					LList_pushRear(dest, d);
				}
				if (!t2AddedArr[t2Count].coef) {
					d.coef = t2->data.coef;
					d.degree = degree2;
					t2AddedArr[t2Count] = d;
					LList_pushRear(dest, d);
				}
			}
		}
	}
}


void main() {
	SparsePoly a, b, c;

	read_poly(&a);
	read_poly(&b);

	add_poly(&c, &a, &b);
	print_poly(&a, " A = ");
	print_poly(&b, " B = ");
	print_poly(&c, "A+B= ");
	system("pause");
}
