#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

#define error(x) printf("%s(%d)\n", x, __LINE__)

typedef char Element;

typedef struct _Node {
	Element data;
	struct _Node *next;
}Node;

typedef struct {
	Node *head;
	int size;
}Stack;

Stack stack;

Stack Stack_create() {
	Stack stack = { NULL };
	stack.size = 0;
	stack.head = NULL;
	return stack;
}

void Stack_push(Stack * stack, Element e) {
	Node *node = (Node*)malloc(sizeof(Node));
	node->data = e;
	node->next = stack->head;
	stack->head = node;
	stack->size++;
}

Element Stack_pop(Stack * stack) {
	Element e = stack->head->data;
	Node *head = stack->head;
	stack->head = head->next;
	stack->size--;
	return e;
}

int Stack_isEmpty(Stack *stack) {
	return stack->size == 0;
}

char doFilterChar(char a) {
	if ('A' <= a && a <= 'Z')
		return a + 32;
	else if ('a' > a || a > 'z')
		return 0;
	return a;
}

int main()
{
	char a[100], filteredA[100];
	int i, filterdAIndex = 0;
	int totalSize;
	fgets(a, 100, stdin);

	Stack_create();
	for (i = 0; i < 100; i++) {
		if (a[i] == '\0')
			break;
		char t = doFilterChar(a[i]);
		if (t != 0) {
			Stack_push(&stack, t);
			filteredA[filterdAIndex++] = t;
		}
	}

	totalSize = stack.size;

	i = 0;
	while (!Stack_isEmpty(&stack)) { if (filteredA[i++] != Stack_pop(&stack)) break; }
	if (totalSize == i)
		printf("It's palindrome!\n");
	else
		printf("not palindrome...\n");
	system("pause");
}