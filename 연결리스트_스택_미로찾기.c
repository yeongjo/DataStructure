#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//jool lim
#define MAP_SIZE 1000
#define VISITED '@'
#define DEADEND 'O'

//char m->map[MAP_SIZE][MAP_SIZE + 1];

int isNotPrintMap;


typedef struct {
	int x, y;
	int way;
} Location2D;

Location2D Location2D_create(int x, int y) {
	Location2D loc;
	loc.x = x;
	loc.y = y;
	return loc;
}

typedef Location2D Element;

typedef struct MazeMap {
	int width;
	int height;
	int **map;
	Location2D enter;
	Location2D exit;
}Maze;

typedef struct _Node {
	Element data;
	struct _Node *next;
}Node;

typedef struct {
	Node *head;
	int size;
}Stack;

Stack Stack_create() {
	Stack stack = { NULL };
	stack.size = 0;
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
void printElement(Element e)
{
	printf("(%d, %d) ", e.x, e.y);
}
void Stack_print(Stack * stack) {
	printf("\n");
	Node *node = stack->head;
	while (node) {
		printElement(node->data);
		node = node->next;
	}
	printf("\n");
}


void testStack() {
	Stack stack = Stack_create();
	Stack_push(&stack, Location2D_create(1, 2));
	Stack_print(&stack);
	printElement(Stack_pop(&stack));
}

Location2D createLocation2D(int x, int y)
{
	Location2D loc;
	loc.x = x, loc.y = y;
	return loc;
}

int isValid(Maze* m, int x, int y)
{
	// printf("isValid: %d, %d:\n", x, y);
	if (x < 0 || x >= MAP_SIZE) return 0;
	if (y < 0 || y >= MAP_SIZE) return 0;
	char ch = m->map[y][x];
	// printf("%c valid: %d\n", ch, ((ch == ' ' || ch == 'x') ? 1 : 0));
	return (ch == ' ' || ch == 'x') ? 1 : 0;
}

void error(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

int isEmpty(Stack* stack) {
	return stack->head == NULL;
}

int getSize(Stack* stack) {
	return stack->size;
}
Element *getRearRef(Stack* stack)
{
	if (isEmpty(stack)) {
		error("Deque empty on getRear()");
	}
	return &stack->head->data;
}

Location2D findStartPosition(Maze *m)
{
	for (int y = 0; y < MAP_SIZE; y++) {
		for (int x = 0; x < MAP_SIZE; x++) {
			if (m->map[y][x] == 'e') {
				return createLocation2D(x, y);
			}
		}
	}
	error("Cannot find start position");
	return createLocation2D(0, 0); // not excuted
}

int colored = 0;

void print_maze(Maze *m) {
	if (isNotPrintMap)
		return;
	int prevColor = 0;
	for (int y = 0; y < MAP_SIZE; y++) {
		if (m->map[y][0] == -1) {
			break;
		}
		// printf("%s\n", m->map[y]);
		for (int x = 0; m->map[y][x] != -1; x++) {
			int tile = m->map[y][x];
			switch (tile) {
			case 0:
				break;
			case 1:
				break;
			case 5:
				break;
			case 9:
				break;
			}
			char ch;
			if (!colored) {
				putchar(ch);
				continue;
			}
			int color = ch == VISITED ? 32 : ch == DEADEND ? 31 : 0;
			if (color == prevColor) {
				putchar(ch);
				continue;
			}
			prevColor = color;
			if (color) {
				printf("\x1b[%d;1m%c", color, ch);
			}
			else {
				printf("\x1b[0m%c", ch);
			}
		}
		if (prevColor) {
			printf("\x1b[0m");
			prevColor = 0;
		}
		putchar('\n');
	}
	printf("\n");
}

int doBackTrack(Maze *m)
{
	Stack stack = Stack_create();
	Stack_push(&stack, findStartPosition(m));
	// addRear(createLocation2D(0, 1));
	int t = 0;
	while (!isEmpty(&stack)) {
		// if (line < 100) {
		// printDeque();
		// print_maze(m);
		// }
		Location2D *p = getRearRef(&stack);
		printf("try %d: ", ++t);
		printElement(*p);
		printf("\n");
		int x = p->x, y = p->y;
		if (m->map[y][x] == 'x') {
			print_maze(m);
			printf("Found: ");
			printElement(*p);
			return 1;
		}
		if (isValid(m, x, y)) {
			m->map[y][x] = VISITED;
		}
		int size = getSize(&stack);
		if (p->way < 1) {
			if (isValid(m, x, y - 1)) {
				Stack_push(&stack, createLocation2D(x, y - 1));
				continue;
			}
			p->way++;
		}
		if (p->way < 2) {
			if (isValid(m, x, y + 1)) {
				Stack_push(&stack, createLocation2D(x, y + 1));
				continue;
			}
			p->way++;
		}
		if (p->way < 3) {
			if (isValid(m, x - 1, y)) {
				Stack_push(&stack, createLocation2D(x - 1, y));
				continue;
			}
			p->way++;
		}
		if (p->way < 4) {
			if (isValid(m, x + 1, y)) {
				Stack_push(&stack, createLocation2D(x + 1, y));
				continue;
			}
			p->way++;
		}

		m->map[y][x] = DEADEND;
		Stack_pop(&stack);

		// if (line < 100) {
		//     print_maze(m);
		// }
	}

	return 0;
}

//int BFS()
//{
//	Stack stack = Stack_create();
//	Stack_push(&stack, findStartPosition(m));
//	// addRear(createLocation2D(0, 1));
//	while (!isEmpty(&stack)) {
//		Stack_print(&stack);
//		Location2D loc = Stack_pop(&stack);
//		// printElement(loc);
//		// printf("\n");
//		int x = loc.x, y = loc.y;
//		if (m->map[y][x] == 'x') return 1;
//		m->map[y][x] = '.';
//		if (isValid(m, x, y - 1)) Stack_push(&stack, createLocation2D(x, y - 1));
//		if (isValid(m, x, y + 1)) Stack_push(&stack, createLocation2D(x, y + 1));
//		if (isValid(m, x - 1, y)) Stack_push(&stack, createLocation2D(x - 1, y));
//		if (isValid(m, x + 1, y)) Stack_push(&stack, createLocation2D(x + 1, y));
//		print_maze(m);
//	}
//
//	return 0;
//}


void getInput() {
	
}

void init_maze(Maze *m, int w, int h) {
	int i;
	m->width = w;
	m->height = h;
	m->map = (int**)malloc(sizeof(int*)*h);
	for (i = 0; i < h; i++)
	{
		m->map[i] = (int*)malloc(sizeof(int)*w);
	}
}

void reset_maze(Maze* m) {
	int i;
	for (i = 0; i < m->height; i++)
	{
		free(m->map[i]);
	}
	free(m->map);
	m->map = NULL;
	m->width = m->height = 0;
}

void load_maze(Maze *m, const char *fname) {
	FILE *fp_stdin = fopen(fname, "r");
	int w, h;
	int line = isNotPrintMap = 0;
	char ch[50];
	fgets(ch, MAP_SIZE, fp_stdin);
	char* t = strtok(ch, " ");
	w = atoi(t);
	t = strtok(NULL, " ");
	h = atoi(t);

	while (fgets(ch, MAP_SIZE, fp_stdin)) {
		ch[strlen(ch) - 1] = '\0';
		int i = 0;
		while (ch) {
			m->map[line][i++] = (*ch)++ - 48;
		}
		m->map[line][i] = 0;
		line++;
	}
	if (line > 100)
		isNotPrintMap = 1;

	init_maze(m, w, h);
}

// 깊이 우선탐색
void search_exit(Maze *m) {
	int state = 0;
	Stack stack = Stack_create();
	Stack_push(&stack, findStartPosition(m));
	// addRear(createLocation2D(0, 1));
	while (!isEmpty(&stack)) {
		Stack_print(&stack);
		Location2D loc = Stack_pop(&stack);
		// printElement(loc);
		// printf("\n");
		int x = loc.x, y = loc.y;
		if (m->map[y][x] == 'x') state = 1;
		m->map[y][x] = '.';
		if (isValid(m, x, y - 1)) Stack_push(&stack, createLocation2D(x, y - 1));
		if (isValid(m, x, y + 1)) Stack_push(&stack, createLocation2D(x, y + 1));
		if (isValid(m, x - 1, y)) Stack_push(&stack, createLocation2D(x - 1, y));
		if (isValid(m, x + 1, y)) Stack_push(&stack, createLocation2D(x + 1, y));
		print_maze(m);
	}

	if (state == 0) {

	}
}

int main(void)
{
	int a = 0;

	Maze maze;
	load_maze(&maze, "maze.txt");
	print_maze(&maze);
	search_exit(&maze);
	print_maze(&maze);

	getInput();
	// testDeque();
	//a = DFS();
	// a = BFS();
	print_maze(&maze);
	//if (a)
	//	printf("성공적으로 도착!\n");
	//else
	//	printf("길찾기 실패\n");
	//doBackTrack();
	system("pause");
	return 0;
}
