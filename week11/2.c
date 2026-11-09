#include <stdio.h>
#include <stdlib.h>
 
#define MAX_VERTEX_COUNT 100 // 최대 정점 개수
#define MAX_EDGE_COUNT 1000  // 최대 간선 개수
 
// 정점과 간선의 라벨
#define FRESH 0
#define VISITED 1
#define TREE 2
#define CROSS 3
 
// 정점 레코드
typedef struct Vertex {
    int name;  // 정점 번호
    int label; // Fresh, Visited
} Vertex;
 
// 간선 레코드
typedef struct Edge {
    int v1; // 양 끝 정점의 인덱스
    int v2;
    int label; // Fresh, Tree, Cross
} Edge;
 
// 인접행렬 구조의 그래프 레코드
typedef struct Graph {
    Vertex vertices[MAX_VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
    int adjacency_matrix[MAX_VERTEX_COUNT]
                        [MAX_VERTEX_COUNT]; // 간선 인덱스, 간선이 없으면 -1
    int n;                                  // 정점 개수
    int m;                                  // 간선 개수
} Graph;
 
// 정점 번호가 name인 정점의 인덱스를 반환, 없으면 -1 반환
int find_vertex(Graph *g, int name) {
    int i;
 
    for (i = 0; i < g->n; i++) {
        if (g->vertices[i].name == name)
            return i;
    }
    return -1;
}
 
// 간선 e에서 정점 v의 반대쪽 정점 인덱스를 반환
int opposite(Graph *g, int v, int e) {
    if (g->edges[e].v1 == v)
        return g->edges[e].v2;
    else
        return g->edges[e].v1;
}
 
// 정점 번호 a와 b를 잇는 간선을 간선 배열과 인접행렬에 삽입
void insert_edge(Graph *g, int a, int b) {
    int v = find_vertex(g, a);
    int w = find_vertex(g, b);
    int e = g->m++;
 
    g->edges[e].v1 = v;
    g->edges[e].v2 = w;
    g->edges[e].label = FRESH;
 
    g->adjacency_matrix[v][w] = e;
    g->adjacency_matrix[w][v] = e;
}
 
// 정점 n개와 간선이 없는 그래프로 초기화
void init_graph(Graph *g, int n) {
    int i, j;
 
    g->n = n;
    g->m = 0;
    for (i = 0; i < n; i++) {
        g->vertices[i].name = i + 1;
        g->vertices[i].label = FRESH;
    }
 
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adjacency_matrix[i][j] = -1;
}
 
// 정점 s에서 출발하는 너비우선탐색 (방문 순서대로 정점 번호를 인쇄)
void bfs(Graph *g, int s) {
    int queue[MAX_VERTEX_COUNT]; // 각 정점은 큐에 한 번만 삽입
    int front = 0, rear = 0;
    int v, w, e, i;
 
    queue[rear++] = s;
    g->vertices[s].label = VISITED;
 
    while (front < rear) {
        v = queue[front++];
        printf("%d\n", g->vertices[v].name);
 
        // 정점 인덱스 순서 = 정점 번호 순서이므로 작은 정점부터 조사
        for (i = 0; i < g->n; i++) {
            e = g->adjacency_matrix[v][i];
            if (e == -1 || g->edges[e].label != FRESH)
                continue;
 
            w = opposite(g, v, e);
            if (g->vertices[w].label == FRESH) {
                g->edges[e].label = TREE;
                g->vertices[w].label = VISITED;
                queue[rear++] = w;
            } else {
                g->edges[e].label = CROSS;
            }
        }
    }
}
 
int main(void) {
    Graph *g = (Graph *)malloc(sizeof(Graph)); // 인접행렬이 커서 힙에 할당
    int n, m, s, a, b, i;
 
    if (g == NULL)
        return 1;
 
    scanf("%d %d %d", &n, &m, &s);
    init_graph(g, n);
 
    for (i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        insert_edge(g, a, b);
    }
 
    bfs(g, find_vertex(g, s));
 
    free(g);
    return 0;
}
