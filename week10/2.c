#include <stdio.h>

#define VERTEX_COUNT 6 // 정점 개수 
#define MAX_EDGE_COUNT 21 // 루프를 포함한 간선 개수

// 정점 레코드
typedef struct Vertex {
    int name; // 정점 번호
} Vertex;

// 간선 레코드
typedef struct Edge {
    int v1; // 양 끝 정점의 인덱스
    int v2;
    int weight; // 가중치 0이면 비어 있는 칸
} Edge;

// 인접행렬 구조의 그래프 레코드
// adjacency_matrix[v][w]: 정점 v와 w를 잇는 간선의 인덱스, 간선이 없으면 -1
typedef struct Graph {
    Vertex vertices[VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
    int adjacency_matrix[VERTEX_COUNT][VERTEX_COUNT];
} Graph;

// 정점 번호가 name인 정점의 인덱스를 반환, 없으면 -1 반환
int find_vertex(Graph *g, int name) {
    int i;

    for (i = 0; i < VERTEX_COUNT; i++) {
        if (g->vertices[i].name == name)
            return i;
    }
    return -1;
}

// 간선 배열에서 비어 있는 칸의 인덱스를 반환
int get_empty_edge(Graph *g) {
    int i;

    for (i = 0; i < MAX_EDGE_COUNT; i++) {
        if (g->edges[i].weight == 0)
            return i;
    }
    return -1;
}

// 정점 v와 w를 잇는 가중치 weight의 간선을 간선 배열과 인접행렬에 삽입
void insert_edge(Graph *g, int v, int w, int weight) {
    int e = get_empty_edge(g);

    g->edges[e].v1 = v;
    g->edges[e].v2 = w;
    g->edges[e].weight = weight;

    g->adjacency_matrix[v][w] = e;
    g->adjacency_matrix[w][v] = e;
}

// 인접행렬의 두 원소를 -1로 치환한 후 간선 배열의 칸을 비움
// 배열 첨자 조정이 필요 없도록 칸을 비워 두고 다음 삽입 때 재사용
void remove_edge(Graph *g, int e) {
    int v = g->edges[e].v1;
    int w = g->edges[e].v2;

    g->adjacency_matrix[v][w] = -1;
    g->adjacency_matrix[w][v] = -1;

    g->edges[e].weight = 0;
}

// 간선 (a, b)의 가중치를 w로 변경
// 간선이 없으면 새로 생성하고, w가 0이면 간선을 삭제
void modify_edge(Graph *g, int a, int b, int w) {
    int v = find_vertex(g, a);
    int u = find_vertex(g, b);
    int e;

    if (v == -1 || u == -1) { // 정점 a 또는 b가 없으면 -1을 출력
        printf("-1\n");
        return;
    }

    e = g->adjacency_matrix[v][u];
    if (w == 0) {
        if (e != -1)
            remove_edge(g, e);
    } else if (e != -1) {
        g->edges[e].weight = w;
    } else {
        insert_edge(g, v, u, w);
    }
}

// 정점 a의 인접 정점과 간선 가중치를 정점 번호의 오름차순으로 인쇄
void print_adjacent(Graph *g, int a) {
    int v = find_vertex(g, a);
    int i, e;

    if (v == -1) {
        printf("-1\n"); // 정점 a가 없으면 -1을 출력
        return;
    }

    for (i = 0; i < VERTEX_COUNT; i++) {
        e = g->adjacency_matrix[v][i];
        if (e != -1)
            printf(" %d %d", g->vertices[i].name, g->edges[e].weight);
    }
    printf("\n");
}

// 정점 6개와 간선이 없는 그래프로 초기화
void init_graph(Graph *g) {
    int i, j;

    for (i = 0; i < VERTEX_COUNT; i++) {
        g->vertices[i].name = i + 1;
        for (j = 0; j < VERTEX_COUNT; j++)
            g->adjacency_matrix[i][j] = -1;
    }

    for (i = 0; i < MAX_EDGE_COUNT; i++)
        g->edges[i].weight = 0;
}

// 무방향 가중 그래프 생성
void build_graph(Graph *g) {
    modify_edge(g, 1, 2, 1);
    modify_edge(g, 1, 3, 1);
    modify_edge(g, 1, 4, 1);
    modify_edge(g, 1, 6, 2);
    modify_edge(g, 2, 3, 1);
    modify_edge(g, 3, 5, 4);
    modify_edge(g, 5, 5, 4);
    modify_edge(g, 5, 6, 3);
}

int main(void) {
    Graph g;
    char cmd;
    int a, b, w;

    init_graph(&g);
    build_graph(&g);

    while (scanf(" %c", &cmd) == 1) {
        if (cmd == 'q')
            break;
        else if (cmd == 'a') {
            scanf("%d", &a);
            print_adjacent(&g, a);
        } else if (cmd == 'm') {
            scanf("%d %d %d", &a, &b, &w);
            modify_edge(&g, a, b, w);
        }
    }

    return 0;
}
