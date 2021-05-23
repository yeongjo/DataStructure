#include <stdio.h>

#define MAX_Deque_SIZE 100

#define error(x) printf("%s(%d)\n", x, __LINE__)

typedef char Element;
Element data[MAX_Deque_SIZE];
int front, rear;


void init_deque() { front = 0; rear = -1; }
int size() { return rear + 1 - front; }
int is_empty() { return rear == -1; }
int is_rearFull() { return (rear == MAX_Deque_SIZE - 1); }
int is_Full() { return rear == MAX_Deque_SIZE - 1 && front == 0; }

void push(Element e) {
	if (is_Full())
		error("덱 포화 에러");
	if (is_rearFull()) {
		for (size_t i = rear; i >= front; i--) {
			data[i - front] = data[i];
		}
		rear -= front;
		front = 0;
	}
	data[++rear] = e;
}

Element popFront() {
	if (is_empty()) {
		error("덱 공백 에러");
		return 0;
	}
	Element t = data[front++];
	return t;
}

Element popRear() {
	if (is_empty()) {
		error("덱 공백 에러");
		return 0;
	}
	return data[rear--];
}

Element peek() {
	if (is_empty())
		error("덱 공백 에러");
	return data[rear];
}

void print_stack() {
	int i;
	printf("[%d : %d]= ", front, rear);
	//for (i = 0; i < size(); i++)
		//printf("%c", data[i]);
	printf(data);
	printf("\n");
}

char doFilterChar(char a) {
	// 대문자면 소문자로
	if ('A' <= a && a <= 'Z')
		return a + 32;
	// 문자가 아니면 0 리턴
	else if('a' > a || a > 'z')
		return 0;
	return a;
}

void test() {
	init_deque();
	push('a');
	push('s');
	push('d');
	push('f');
	print_stack();
	popFront();
	popFront();
	popFront();
	print_stack();
	push('g');
	push('h');
	print_stack();
}

int main()
{
	//test();

	char a[100] , filteredA[100];
	int i, filterdAIndex = 0;
	int totalSize;
	gets(a);

	init_deque();
	for (i = 0; i < 100; i++) {
		if (a[i] == '\0')
			break;
		char t = doFilterChar(a[i]);
		// 필터되서 나온 문자가 정상적 소문자라면 스택에 넣어라
		if (t != 0) {
			push(t);
			filteredA[filterdAIndex++] = t;
		}
	}

	totalSize = size();

	i = 0;
	// 스택에서 나온 문자와 a에 있는 원본과 다르다면 중간에 끊는다.
	while (size() > 1) {
		if (popFront() != popRear()) {
			i = 1; break; 
		} 
	}
	
	if (1 == i)
		printf("not palindrome...\n");
	else
		printf("It's palindrome!\n");
	system("pause");
}