#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VECTOR_INIT_SLOT_SIZE 8
#define MAX_LINE_BUFFER 2048
#define WHITESPACES " \\n\\r\\t"
typedef char VertexData;
typedef struct GraphNodeVector {
	int *ids;
	int size;
	int maxSize;
} GraphNodeVector;
void GraphNodeVector_init(GraphNodeVector *v) {
	v->ids = NULL;
	v->size = 0;
	v->maxSize = 0;
}
void GraphNodeVector_destroy(GraphNodeVector *v) {
	printf("Destroying GraphNodeVector of size %d\\n", v->size);
	if (v->ids) free(v->ids);
}
int GraphNodeVector_addNode(GraphNodeVector *v, int id) {
	if (v->size == v->maxSize) {
		if (v->maxSize == 0) {
			v->maxSize = VECTOR_INIT_SLOT_SIZE;
			v->ids = (int *)malloc(v->maxSize * sizeof(int));
		} else {
			v->maxSize = 2 * v->size;
			v->ids = (int *)realloc(v->ids, v->maxSize * sizeof(int));
		}
	}
	v->ids [v->size] = id;
	return v->size++;
}
typedef struct _GraphVertex {
	VertexData data;
	GraphNodeVector nodes;
}GraphVertex;
GraphVertex *GraphVertex_alloc(VertexData data) {
	GraphVertex *v = (GraphVertex *)malloc(sizeof(GraphVertex));
	v->data = data;
	GraphNodeVector_init(&v->nodes);
	return v;
}
void GraphVertex_destroy(GraphVertex *v) {
	printf("Destroying GraphVertex: %c\\n", v->data);
	GraphNodeVector_destroy(&v->nodes);
	free(v);
}
typedef struct _Graph {
	GraphVertex **vertices;
	int size;
	int maxSize;
}Graph;
void Graph_init(Graph *g) {
	g->vertices = NULL;
	g->size = 0;
	g->maxSize = 0;
}
void Graph_destroy(Graph *g) {
	printf("Destroying Graph of size %d\\n", g->size);
	if (g->vertices) {
		// 가지고있는 모든 포인터배열을 지우고
		for (int i = 0; i < g->size; i++) {
			GraphVertex_destroy(g->vertices [i]);
		}
		// 그 뒤에 포인터를 지운다.
		free(g->vertices);
	}
	Graph_init(g);
}
int Graph_isEmpty(Graph *g) {
	return g->size == 0;
}
int Graph_addVertex(Graph *g, GraphVertex *v) {
	if (g->size == g->maxSize) { // 하나도 할당안받았거나 가득찼을때
		if (g->maxSize == 0) {
			g->maxSize = VECTOR_INIT_SLOT_SIZE;
			g->vertices = (GraphVertex * *)malloc(g->maxSize * sizeof(GraphVertex *));
		} else {
			g->maxSize = 2 * g->size;
			g->vertices = (GraphVertex * *)realloc(g->vertices, g->maxSize * sizeof(GraphVertex *));
		}
	}
	g->vertices [g->size] = v;
	return g->size++;
}
int Graph_addEdge(Graph *g, int from, int to) {
	// 존재하지않는 버텍스 반환
	if (from < 0 || from >= g->size) return 0;
	if (to < 0 || to >= g->size) return 0;
	GraphVertex * v = g->vertices [from];
	GraphNodeVector_addNode(&v->nodes, to);
	return 1;
}
Graph graph;
void readFromFile(char *file) {
	FILE *fp = fopen(file, "r");
	if (!fp) {
		printf("File not open: %s\\n", file);
		return;
	}
	Graph_destroy(&graph);
	char buffer [MAX_LINE_BUFFER];
	while (fgets(buffer, MAX_LINE_BUFFER, fp)) {
		char *ch = strtok(buffer, WHITESPACES);
		if (!ch) {
			printf("ignoring empty line\\n");
			continue;
		}
		if (*ch == '#') {
			printf("ignoring the line starting with #\\n");
			continue;
		}
		if (strlen(ch) != 1) {
			printf("Only 1 char node can be added\\n");
			continue;
		}
		GraphVertex *v = GraphVertex_alloc(*ch);
		Graph_addVertex(&graph, v);
		printf("Added vector %c:\\n", *ch);
		while (1) {
			char *p = strtok(NULL, WHITESPACES);
			if (!p || !*p) break;
			int id;
			if (1 != sscanf(p, "%d", &id)) {
				printf(" ignoring non-number argument (%s) and thereafter\\n", p);
				break;
			}
			GraphNodeVector_addNode(&v->nodes, id);
			printf(" added node to %d\\n", id);
		}
	}
	fclose(fp);
}
int main(void) {
	Graph_init(&graph);
	while (1) {
		char buffer [MAX_LINE_BUFFER];
		if (!fgets(buffer, MAX_LINE_BUFFER, stdin)) {
			break;
		}
		// 끊는 기능
		char *cmd = strtok(buffer, WHITESPACES);
		if (!cmd || !*cmd) { // cmd 가 null이거나 가르키는 문자가 0 이면
			continue; // 무시하고 새로운 입력받기
		}
		printf("command: %s buf=%p cmd=%p buf=%s\\n", cmd, buffer, cmd, buffer);
		if (strcmp(cmd, "load") == 0) {
			char *file = strtok(NULL, WHITESPACES);
			printf("filename: %s\\n", file);
			readFromFile(file);
			continue;
		}
		if (!strcmp(cmd, "v")) {
			char *ch = strtok(NULL, WHITESPACES);
			if (strlen(ch) != 1) {
				printf("Only 1 char node can be added\\n");
				continue;
			}
			GraphVertex *v = GraphVertex_alloc(*ch);
			int index = Graph_addVertex(&graph, v);
			printf("Vertex added at index %d\\n", index);
			continue;
		}
		if (!strcmp(cmd, "e")) {
			// char *p = strtok(NULL, WHITESPACES);
			// int n1, n2;
			// int read = sscanf(p, "%d%d", &n1, &n2);
			// if (read != 2) {
			//     printf("Two indices should be provided (returned %d, p=%s)\\n", read, p);
			//     continue;
			// }
			char *p1 = strtok(NULL, WHITESPACES);
			char *p2 = strtok(NULL, WHITESPACES);
			if (!p1 || !p2) {
				printf("Two indices should be provided\\n");
				continue;
			}
			int n1, n2, read = 0;
			if (sscanf(p1, "%d", &n1) == 1) {
				read++;
			}
			if (sscanf(p2, "%d", &n2) == 1) {
				read++;
			}
			if (read != 2) {
				printf("Two indices should be provided\\n");
				continue;
			}
			if (n1 == n2) {
				printf("Two indices should differ\\n");
				continue;
			}
			int success = Graph_addEdge(&graph, n1, n2);
			printf("Adding edge from %d to %d %s\\n", n1, n2, success ? "succeeded" : "failed");
			continue;
		}
		if (!strcmp(cmd, "reset")) {
			Graph_destroy(&graph);
			continue;
		}
		if (strcmp(cmd, "q") == 0) {
			break;
		}
		printf("Unknown command: %s\\n", cmd);
	}
	return 0;
}