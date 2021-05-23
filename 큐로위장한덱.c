#include <stdio.h>

typedef int Element;

#define MAX_QUEUE_SIZE 100
typedef struct {
	Element queue_data[MAX_QUEUE_SIZE];
	int queue_front, queue_rear;
}Queue;
Queue queue;

#define QUEUE_INDEX(idx, amount) \
    (((idx) + (amount) + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE)

void error(const char* ch) {
	printf("%s\n", ch);
}

void initQueue(Queue* queue)
{
	queue->queue_front = queue->queue_rear = 0;
}

int isEmpty(Queue* queue) {
	return queue->queue_front == queue->queue_rear;
}

int isFull(Queue* queue) {
	return QUEUE_INDEX(queue->queue_rear, 1) == queue->queue_front;
}

int getSize(Queue* queue) {
	return (queue->queue_rear - queue->queue_front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}

void addFront(Queue* queue, Element e)
{
	if (isFull(queue)) {
		error("Queue full on addFront()");
	}

	queue->queue_data[queue->queue_front] = e;
	queue->queue_front = QUEUE_INDEX(queue->queue_front, -1);
}

void addRear(Queue* queue, Element e)
{
	if (isFull(queue)) {
		error("Queue full on addRear()");
	}
	queue->queue_rear = QUEUE_INDEX(queue->queue_rear, 1);
	queue->queue_data[queue->queue_rear] = e;
}

Element deleteFront(Queue* queue)
{
	if (isEmpty(queue)) {
		error("Queue empty on deleteFront()");
	}
	queue->queue_front = QUEUE_INDEX(queue->queue_front, 1);
	return queue->queue_data[queue->queue_front];
}

Element deleteRear(Queue* queue)
{
	if (isEmpty(queue)) {
		error("Queue empty on deleteRear()");
	}
	Element r = queue->queue_data[queue->queue_rear];
	queue->queue_rear = QUEUE_INDEX(queue->queue_rear, -1);
	return r;
}

Element getFront(Queue* queue)
{
	if (isEmpty(queue)) {
		error("Queue empty on getFront()");
	}
	int front = QUEUE_INDEX(queue->queue_front, 1);
	Element r = queue->queue_data[front];
	return r;
}

Element getRear(Queue* queue)
{
	if (isEmpty(queue)) {
		error("Queue empty on getRear()");
	}
	Element r = queue->queue_data[queue->queue_rear];
	return r;
}


void input() {
	while (1) {
		int i;
		printf("양의 정수를 입력하세요(종료: -1): ");
		scanf("%d%*c", &i);
		if (i == -1) break;
		addRear(&queue, i);
	}
	while (!isEmpty(&queue)) {
		printf("%d->", deleteFront(&queue));
	}
	printf("NULL\n");
}

void main() {
	initQueue(&queue);
	input();
	system("pause");
}