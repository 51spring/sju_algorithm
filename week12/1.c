#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_COUNT 100 // 최대 정점 개수 (n < 100)
#define MAX_EDGE_COUNT 1000  // 최대 간선 개수 (m < 1,000)

// 부착간선 리스트의 노드: 간선 배열의 인덱스를 저장
typedef struct IncidentNode {
    int edge;
    struct IncidentNode *next;
} IncidentNode;

// 정점 레코드
typedef struct Vertex {
    char name;              // 정점 이름 (단순 문자)
    IncidentNode *outEdges; // 진출부착간선 리스트 (헤더노드)
    IncidentNode *inEdges;  // 진입부착간선 리스트 (헤더노드)
    int inDegree;           // 진입차수
} Vertex;

// 방향간선 레코드
typedef struct Edge {
    int origin;      // 시점 정점의 인덱스
    int destination; // 종점 정점의 인덱스
} Edge;

// 인접리스트 구조의 방향그래프 레코드
typedef struct Graph {
    Vertex vertices[MAX_VERTEX_COUNT];
    Edge edges[MAX_EDGE_COUNT];
} Graph;

// 전역 데이터
Graph G;
int n, m;                          // n = 정점 수, m = 간선 수
int topOrder[MAX_VERTEX_COUNT + 1]; // topOrder[0]: DAG 여부, [1:n]: 위상순서
int Q[MAX_VERTEX_COUNT];           // 큐 (각 정점은 최대 한 번만 삽입됨)
int front, rear;

// 부착간선 리스트의 노드 하나를 생성
IncidentNode *getnode(int e) {
    IncidentNode *node = (IncidentNode *)malloc(sizeof(IncidentNode));
    if (node == NULL)
        exit(1);

    node->edge = e;
    node->next = NULL;
    return node;
}

// 헤더연결리스트 H의 첫 노드 위치에 i를 삽입 (주의 사항 2-1)
void addFirst(IncidentNode *H, int i) {
    IncidentNode *node = getnode(i);

    node->next = H->next;
    H->next = node;
}

// 이름이 vName인 정점의 인덱스를 반환, 없으면 -1 반환
int findIndex(char vName) {
    int i;

    for (i = 0; i < n; i++) {
        if (G.vertices[i].name == vName)
            return i;
    }
    return -1;
}

// 정점 vName을 인덱스 i에 삽입하고 진입차수 초기화
void insertVertex(char vName, int i) {
    G.vertices[i].name = vName;
    G.vertices[i].outEdges = getnode(-1); // 헤더노드
    G.vertices[i].inEdges = getnode(-1);  // 헤더노드
    G.vertices[i].inDegree = 0;
}

// uName -> wName 방향간선 i를 간선 배열과 양쪽 부착간선 리스트에 삽입
void insertDirectedEdge(char uName, char wName, int i) {
    int u = findIndex(uName);
    int w = findIndex(wName);

    G.edges[i].origin = u;
    G.edges[i].destination = w;

    addFirst(G.vertices[u].outEdges, i); // u의 진출간선 리스트 맨 앞에 삽입
    addFirst(G.vertices[w].inEdges, i);  // w의 진입간선 리스트 맨 앞에 삽입
    G.vertices[w].inDegree++;
}

// 표준입력으로부터 방향그래프 구축
void buildGraph(void) {
    char vName, uName, wName;
    int i;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf(" %c", &vName); // 앞의 공백/개행을 건너뛰고 문자 하나 읽기
        insertVertex(vName, i);
    }

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf(" %c %c", &uName, &wName);
        insertDirectedEdge(uName, wName, i);
    }
}

// 큐 관련 함수
void initQueue(void) {
    front = rear = 0;
}

int isEmpty(void) {
    return front == rear;
}

void enqueue(int v) {
    Q[rear++] = v;
}

int dequeue(void) {
    return Q[front++];
}

// 진입차수를 이용한 위상 정렬: O(n + m)
void topologicalSort(void) {
    int in[MAX_VERTEX_COUNT]; // in[i] = 정점 i의 (남은) 진입차수
    IncidentNode *p;
    int i, t, u, w, e;

    initQueue();

    // 정점 번호 순서대로 조사 (주의 사항 2-2)
    for (i = 0; i < n; i++) {
        in[i] = G.vertices[i].inDegree;
        if (in[i] == 0)
            enqueue(i);
    }

    t = 1; // 위상순위
    while (!isEmpty()) {
        u = dequeue();
        topOrder[t++] = u;

        for (p = G.vertices[u].outEdges->next; p != NULL; p = p->next) {
            e = p->edge;
            w = G.edges[e].destination;
            in[w]--;
            if (in[w] == 0)
                enqueue(w);
        }
    }

    // 순위가 매겨지지 않은 정점이 남아 있으면 방향싸이클 존재
    if (t <= n)
        topOrder[0] = 0;
    else
        topOrder[0] = 1;
}

// 헤더노드를 포함한 모든 부착간선 리스트의 메모리 해제
void freeList(IncidentNode *H) {
    IncidentNode *next;

    while (H != NULL) {
        next = H->next;
        free(H);
        H = next;
    }
}

void freeGraph(void) {
    int i;

    for (i = 0; i < n; i++) {
        freeList(G.vertices[i].outEdges);
        freeList(G.vertices[i].inEdges);
    }
}

int main(void) {
    int i;

    buildGraph();
    topologicalSort();

    if (topOrder[0] == 0) { // non-DAG
        printf("0\n");
    } else {                // DAG
        for (i = 1; i <= n; i++) {
            if (i > 1)
                printf(" ");
            printf("%c", G.vertices[topOrder[i]].name);
        }
        printf("\n");
    }

    freeGraph();
    return 0;
}
