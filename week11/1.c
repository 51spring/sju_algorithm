#include <stdio.h>
#include <stdlib.h>
 
#define MAX_VERTEX_COUNT 100 // 최대 정점 개수
#define MAX_EDGE_COUNT 1000  // 최대 간선 개수
 
// 정점과 간선의 라벨
#define FRESH 0
#define VISITED 1
#define TREE 2
#define BACK 3
 
// 부착간선 리스트의 노드: 간선 배열의 인덱스를 저장
typedef struct IncidentNode {
    int edge;
    struct IncidentNode *next;
} IncidentNode;
 
// 정점 레코드
typedef struct Vertex {
    int name;                     // 정점 번호
    int label;                    // Fresh, Visited
    IncidentNode *incident_edges; // 부착간선 인덱스의 헤더연결리스트 (인접 정점
                                  // 번호의 오름차순)
} Vertex;
 
// 간선 레코드
typedef struct Edge {
    int v1; // 양 끝 정점의 인덱스
    int v2;
    int label; // Fresh, Tree, Back
} Edge;
 
// 인접리스트 구조의 그래프 레코드
typedef struct Graph {
    Vertex vertices[MAX_VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
    int n; // 정점 개수
    int m; // 간선 개수
} Graph;
 
// 부착간선 리스트의 노드 하나를 생성
IncidentNode *get_node(int e) {
    IncidentNode *w = (IncidentNode *)malloc(sizeof(IncidentNode));
    if (w == NULL)
        exit(1);
 
    w->edge = e;
    w->next = NULL;
    return w;
}
 
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
 
// 정점 v의 부착간선 리스트에 간선 e를 인접 정점 번호의 오름차순 위치에 삽입
void insert_incident_edge(Graph *g, int v, int e) {
    IncidentNode *prev = g->vertices[v].incident_edges; // 헤더노드에서 시작
    IncidentNode *w = get_node(e);
    int target = g->vertices[opposite(g, v, e)].name;
 
    while (prev->next != NULL &&
           g->vertices[opposite(g, v, prev->next->edge)].name < target)
        prev = prev->next;
 
    w->next = prev->next;
    prev->next = w;
}
 
// 정점 번호 a와 b를 잇는 간선을 간선 배열과 부착간선 리스트에 삽입
void insert_edge(Graph *g, int a, int b) {
    int v = find_vertex(g, a);
    int w = find_vertex(g, b);
    int e = g->m++;
 
    g->edges[e].v1 = v;
    g->edges[e].v2 = w;
    g->edges[e].label = FRESH;
 
    insert_incident_edge(g, v, e);
    if (v != w) // 루프는 부착간선 리스트에 한 번만 삽입
        insert_incident_edge(g, w, e);
}
 
// 정점 n개와 간선이 없는 그래프로 초기화
void init_graph(Graph *g, int n) {
    int i;
 
    g->n = n;
    g->m = 0;
    for (i = 0; i < n; i++) {
        g->vertices[i].name = i + 1;
        g->vertices[i].label = FRESH;
        g->vertices[i].incident_edges = get_node(-1); // 헤더노드
    }
}
 
// 정점 v에서 출발하는 깊이우선탐색 (방문 순서대로 정점 번호를 인쇄)
void rdfs(Graph *g, int v) {
    IncidentNode *p;
    int e, w;
 
    g->vertices[v].label = VISITED;
    printf("%d\n", g->vertices[v].name);
 
    // 부착간선 리스트가 인접 정점 번호의 오름차순이므로 작은 정점부터 조사
    for (p = g->vertices[v].incident_edges->next; p != NULL; p = p->next) {
        e = p->edge;
        if (g->edges[e].label == FRESH) {
            w = opposite(g, v, e);
            if (g->vertices[w].label == FRESH) {
                g->edges[e].label = TREE;
                rdfs(g, w);
            } else {
                g->edges[e].label = BACK;
            }
        }
    }
}
 
// 헤더노드를 포함한 모든 부착간선 리스트의 메모리 해제
void free_graph(Graph *g) {
    IncidentNode *w, *next;
    int i;
 
    for (i = 0; i < g->n; i++) {
        w = g->vertices[i].incident_edges;
        while (w != NULL) {
            next = w->next;
            free(w);
            w = next;
        }
    }
}
 
int main(void) {
    Graph g;
    int n, m, s, a, b, i;
 
    scanf("%d %d %d", &n, &m, &s);
    init_graph(&g, n);
 
    for (i = 0; i < m; i++) {
        scanf("%d %d", &a, &b);
        insert_edge(&g, a, b);
    }
 
    rdfs(&g, find_vertex(&g, s));
 
    free_graph(&g);
    return 0;
}
