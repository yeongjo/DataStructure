#include <stdio.h>

#define MAX_DEQUE_SIZE 100

#define error(x) printf("%s(%d)\n", x, __LINE__)

typedef char Element;

typedef struct{
	Element data[MAX_DEQUE_SIZE];
	int front, rear;
}Deque;


void init_deque(Deque* deque) { deque->front = 0; deque->rear = -1; }
int size(Deque* deque) { return deque->rear + 1 - deque->front; }
int is_empty(Deque* deque) { return deque->rear + 1 == deque->front; }
int is_rearFull(Deque* deque) { return (deque->rear == MAX_DEQUE_SIZE - 1); }
int is_Full(Deque* deque) { return deque->rear == MAX_DEQUE_SIZE - 1 && deque->front == 0; }

void push(Deque* deque, Element e) {
	if (is_Full(deque))
		error("덱 포화 에러");
	if (is_rearFull(deque)) {
		for (size_t i = deque->rear; i >= deque->front; i--) {
			deque->data[i - deque->front] = deque->data[i];
		}
		deque->rear -= deque->front;
		deque->front = 0;
	}
	deque->data[++deque->rear] = e;
}

Element popFront(Deque* deque) {
	if (is_empty(deque)) {
		error("덱 공백 에러");
		return 0;
	}
	Element t = deque->data[deque->front++];
	return t;
}

Element popRear(Deque* deque) {
	if (is_empty(deque)) {
		error("덱 공백 에러");
		return 0;
	}
	return deque->data[deque->rear--];
}

Element peek(Deque* deque) {
	if (is_empty(deque))
		error("덱 공백 에러");
	return deque->data[deque->rear];
}

void print_deque(Deque* deque, int msg) {
	int i;
	printf("[%d][%d : %d]= ", msg, deque->front, deque->rear);
	for (i = deque->front; i <= deque->rear; i++)
		printf("%c ", deque->data[i]);
	//printf(deque->data);
	printf("\n");
}


void stealJobFromOther(Deque* dequqs, int count, int myidx) {
	if (size(&dequqs[myidx]) != 0) {
		return;
	}
	for (size_t i = 0; i < count; i++)
	{
		if (i == myidx) continue;
		if (size(&dequqs[i]) > 1) {
			push(&dequqs[myidx], popRear(&dequqs[i]));
		}
	}
}

void checkEmptyDequeThenSteal(Deque* deques, int count) {
	for (size_t i = 0; i < count; i++)
	{
		stealJobFromOther(deques, count, i);
	}
}

void startJob(Deque* deque, int count) {
	int isEnd = 0;
	int jobCount = 0;
	while(!isEnd){
		int maxCount = 0;
		++jobCount;
		for (size_t i = 0; i < count; i++)
		{
			maxCount = (size(&deque[i]) > maxCount) ? size(&deque[i]) : maxCount;
			if (maxCount == 0) {
				isEnd = 1;
				break;
			}

			if (size(&deque[i]) > 0) {
				printf("[%d] 일하기 전 \t", jobCount);
				print_deque(&deque[i], i);
				popFront(&deque[i]);
				printf("[%d] 일한 뒤 \t", jobCount);
			}
			else
				printf("[%d] 일할게 없음\t", jobCount);
			print_deque(&deque[i], i);
		}
		if (isEnd) break;
		checkEmptyDequeThenSteal(deque, count);
	}
	
}

int main()
{
	Deque deque[2];

	init_deque(&deque[0]);
	init_deque(&deque[1]);

	push(&deque[0], '1');
	push(&deque[0], '2');
	push(&deque[0], '3');
	push(&deque[0], '4');
	push(&deque[0], '5');
	push(&deque[0], '6');

	push(&deque[1], '4');

	startJob(deque, 2);

	system("pause");
}