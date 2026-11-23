#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_COUNT 100 // 최대 정점 개수 (1 <= n <= 100)
#define MAX_EDGE_COUNT 1000  // 최대 간선 개수 (1 <= m <= 1,000)
#define INF INT_MAX          // 아직 트리와 연결되지 않은 정점의 거리

// 부착간선 리스트의 노드: 간선 배열의 인덱스를 저장
typedef struct IncidentNode {
    int edge;
    struct IncidentNode *next;
} IncidentNode;

// 정점 레코드 (정점 번호 1 ~ n을 그대로 배열 인덱스로 사용)
typedef struct Vertex {
    int name;                    // 정점 번호
    IncidentNode *incidentEdges; // 부착간선 리스트 (헤더노드)
    int distance; // d(v): 트리에서 v로 가는 가장 가벼운 간선의 무게
    int locator;  // 힙 H에서의 위치 (0이면 큐에 없음 = 트리에 포함됨)
} Vertex;

// 무방향간선 레코드
typedef struct Edge {
    int endpoint1; // 끝 정점 1
    int endpoint2; // 끝 정점 2
    int weight;    // 무게
} Edge;

// 인접리스트 구조의 무방향그래프 레코드
typedef struct Graph {
    Vertex vertices[MAX_VERTEX_COUNT + 1];
    Edge edges[MAX_EDGE_COUNT];
} Graph;

// 전역 데이터
Graph G;
int n, m; // n = 정점 수, m = 간선 수
int H[MAX_VERTEX_COUNT +
      1];     // 우선순위 큐: 정점 번호를 저장하는 최소힙 (키 = distance)
int heapSize; // 힙에 남아 있는 원소 수

// 부착간선 리스트의 노드 하나를 생성
IncidentNode *getnode(int e) {
    IncidentNode *node = (IncidentNode *)malloc(sizeof(IncidentNode));
    if (node == NULL)
        exit(1);

    node->edge = e;
    node->next = NULL;
    return node;
}

// 헤더연결리스트 L의 첫 노드 위치에 i를 삽입
void addFirst(IncidentNode *L, int i) {
    IncidentNode *node = getnode(i);

    node->next = L->next;
    L->next = node;
}

// 정점 v를 삽입
void insertVertex(int v) {
    G.vertices[v].name = v;
    G.vertices[v].incidentEdges = getnode(-1); // 헤더노드
}

// u - w 무방향간선 i를 간선 배열과 양 끝 정점의 부착간선 리스트에 삽입
void insertUndirectedEdge(int u, int w, int weight, int i) {
    G.edges[i].endpoint1 = u;
    G.edges[i].endpoint2 = w;
    G.edges[i].weight = weight;

    addFirst(G.vertices[u].incidentEdges, i);
    addFirst(G.vertices[w].incidentEdges, i);
}

// 간선 e에서 정점 v의 반대쪽 끝 정점을 반환
int opposite(int v, int e) {
    if (G.edges[e].endpoint1 == v)
        return G.edges[e].endpoint2;
    return G.edges[e].endpoint1;
}

// 표준입력으로부터 무방향그래프 구축
void buildGraph(void) {
    int u, w, weight;
    int i;

    scanf("%d %d", &n, &m);
    for (i = 1; i <= n; i++)
        insertVertex(i);

    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &w, &weight);
        insertUndirectedEdge(u, w, weight, i);
    }
}

// 힙 관련 함수 (H[1:heapSize], 위치가 바뀔 때마다 locator 갱신)
int key(int i) { return G.vertices[H[i]].distance; }

int isEmpty(void) { return heapSize == 0; }

// 힙의 i, j 위치 원소를 교환하고 locator 갱신
void swapElements(int i, int j) {
    int tmp = H[i];

    H[i] = H[j];
    H[j] = tmp;
    G.vertices[H[i]].locator = i;
    G.vertices[H[j]].locator = j;
}

// i 위치 원소를 위로 올리며 힙 순서 복구
void upHeap(int i) {
    while (i > 1 && key(i / 2) > key(i)) {
        swapElements(i, i / 2);
        i /= 2;
    }
}

// i 위치 원소를 아래로 내리며 힙 순서 복구
void downHeap(int i) {
    int smaller;

    while (2 * i <= heapSize) {
        smaller = 2 * i; // 왼쪽 자식
        if (smaller + 1 <= heapSize && key(smaller + 1) < key(smaller))
            smaller++; // 오른쪽 자식이 더 작으면 오른쪽

        if (key(i) <= key(smaller))
            break;

        swapElements(i, smaller);
        i = smaller;
    }
}

// 정점 v를 큐에 삽입: O(log n)
void insertItem(int v) {
    heapSize++;
    H[heapSize] = v;
    G.vertices[v].locator = heapSize;
    upHeap(heapSize);
}

// 키가 가장 작은 정점을 삭제하여 반환: O(log n)
int removeMin(void) {
    int v = H[1];

    swapElements(1, heapSize);
    heapSize--;
    downHeap(1);

    G.vertices[v].locator = 0; // 큐에서 빠짐 = 트리에 포함됨
    return v;
}

// 정점 v의 키가 작아졌을 때 힙 위치 조정: O(log n)
void replaceKey(int v, int k) {
    G.vertices[v].distance = k;
    upHeap(G.vertices[v].locator); // 키가 줄기만 하므로 upHeap만 필요
}

// Prim-Jarnik 알고리즘: O(m log n)
// MST에 추가되는 정점을 차례로 인쇄하고 총무게를 반환
int primJarnikMST(void) {
    IncidentNode *p;
    int i, u, z, e, total;

    for (i = 1; i <= n; i++)
        G.vertices[i].distance = INF;
    G.vertices[1].distance = 0; // 출발정점은 1 (주의 사항)

    heapSize = 0;
    for (i = 1; i <= n; i++)
        insertItem(i);

    total = 0;
    while (!isEmpty()) {
        u = removeMin();
        printf(" %d", u); // MST에 추가되는 정점
        total += G.vertices[u].distance;

        // u가 트리에 들어왔으므로 큐에 남은 이웃 z의 distance 갱신
        for (p = G.vertices[u].incidentEdges->next; p != NULL; p = p->next) {
            e = p->edge;
            z = opposite(u, e);
            if (G.vertices[z].locator != 0 &&
                G.edges[e].weight < G.vertices[z].distance)
                replaceKey(z, G.edges[e].weight);
        }
    }
    printf("\n");

    return total;
}

// 헤더노드를 포함한 부착간선 리스트의 메모리 해제
void freeList(IncidentNode *L) {
    IncidentNode *next;

    while (L != NULL) {
        next = L->next;
        free(L);
        L = next;
    }
}

void freeGraph(void) {
    int i;

    for (i = 1; i <= n; i++)
        freeList(G.vertices[i].incidentEdges);
}

int main(void) {
    buildGraph();
    printf("%d\n", primJarnikMST());

    freeGraph();
    return 0;
}
