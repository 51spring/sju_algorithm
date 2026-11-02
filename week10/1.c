#include <stdio.h>
#include <stdlib.h>

#define VERTEX_COUNT 6    // 정점 개수 (변경되지 않음)
#define MAX_EDGE_COUNT 21 // 루프를 포함한 최대 간선 개수

// 부착간선 리스트의 노드: 간선 배열의 인덱스를 저장
typedef struct IncidentNode {
    int edge;
    struct IncidentNode *next;
} IncidentNode;

// 정점 레코드
typedef struct Vertex {
    int name;                     // 정점 번호
    IncidentNode *incident_edges; // 부착간선 인덱스의 헤더연결리스트 (인접 정점
                                  // 번호의 오름차순)
} Vertex;

// 간선 레코드
typedef struct Edge {
    int v1; // 양 끝 정점의 인덱스
    int v2;
    int weight; // 가중치 0이면 비어 있는 칸
} Edge;

// 인접리스트 구조의 그래프 레코드
typedef struct Graph {
    Vertex vertices[VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
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

    for (i = 0; i < VERTEX_COUNT; i++) {
        if (g->vertices[i].name == name)
            return i;
    }
    return -1;
}

// 간선 e에서 정점 v의 반대쪽 정점 인덱스를 반환 (루프이면 v 자신)
int opposite(Graph *g, int v, int e) {
    if (g->edges[e].v1 == v)
        return g->edges[e].v2;
    else
        return g->edges[e].v1;
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

// 정점 v와 w를 잇는 간선의 인덱스를 반환, 없으면 -1 반환
int find_edge(Graph *g, int v, int w) {
    IncidentNode *p;

    for (p = g->vertices[v].incident_edges->next; p != NULL; p = p->next) {
        if (opposite(g, v, p->edge) == w)
            return p->edge;
    }
    return -1;
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

// 정점 v의 부착간선 리스트에서 간선 e의 노드를 삭제
void remove_incident_edge(Graph *g, int v, int e) {
    IncidentNode *prev = g->vertices[v].incident_edges; // 헤더노드에서 시작
    IncidentNode *w = prev->next;

    while (w != NULL) {
        if (w->edge == e) {
            prev->next = w->next;
            free(w);
            return;
        }
        prev = w;
        w = w->next;
    }
}

// 정점 v와 w를 잇는 가중치 weight의 간선을 간선 배열과 부착간선 리스트에 삽입
void insert_edge(Graph *g, int v, int w, int weight) {
    int e = get_empty_edge(g);

    g->edges[e].v1 = v;
    g->edges[e].v2 = w;
    g->edges[e].weight = weight;

    insert_incident_edge(g, v, e);
    if (v != w) // 루프는 부착간선 리스트에 한 번만 삽입
        insert_incident_edge(g, w, e);
}

// 양 끝 정점의 부착간선 리스트에서 간선 e를 삭제한 후 간선 배열의 칸을 비움
// 배열 첨자 조정이 필요 없도록 칸을 비워 두고 다음 삽입 때 재사용
void remove_edge(Graph *g, int e) {
    int v = g->edges[e].v1;
    int w = g->edges[e].v2;

    remove_incident_edge(g, v, e);
    if (v != w) // 루프는 부착간선 리스트에 한 번만 존재
        remove_incident_edge(g, w, e);

    g->edges[e].weight = 0;
}

// 간선 (a, b)의 가중치를 w로 변경
// 간선이 없으면 새로 생성, w가 0이면 간선을 삭제
void modify_edge(Graph *g, int a, int b, int w) {
    int v = find_vertex(g, a);
    int u = find_vertex(g, b);
    int e;

    if (v == -1 || u == -1) { // 정점 a 또는 b가 없으면 -1을 출력
        printf("-1\n");
        return;
    }

    e = find_edge(g, v, u);
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
    IncidentNode *p;

    if (v == -1) {
        printf("-1\n"); // 정점 a가 없으면 -1을 출력
        return;
    }

    for (p = g->vertices[v].incident_edges->next; p != NULL; p = p->next)
        printf(" %d %d", g->vertices[opposite(g, v, p->edge)].name,
               g->edges[p->edge].weight);
    printf("\n");
}

// 정점 6개와 간선이 없는 그래프로 초기화
void init_graph(Graph *g) {
    int i;

    for (i = 0; i < VERTEX_COUNT; i++) {
        g->vertices[i].name = i + 1;
        g->vertices[i].incident_edges = get_node(-1); // 헤더노드
    }

    for (i = 0; i < MAX_EDGE_COUNT; i++)
        g->edges[i].weight = 0;
}

// 무방향 가중그래프 생성
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

// 헤더노드를 포함한 모든 부착간선 리스트의 메모리 해제
void free_graph(Graph *g) {
    IncidentNode *w, *next;
    int i;

    for (i = 0; i < VERTEX_COUNT; i++) {
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

    free_graph(&g);
    return 0;
}
