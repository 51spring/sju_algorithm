#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_COUNT 100 // 최대 정점 개수 (1 <= n <= 100)
#define MAX_EDGE_COUNT 1000  // 최대 간선 개수 (1 <= m <= 1,000)

// Kruskal은 인접 정보 없이 간선만 차례로 보면 되므로 간선리스트 구조 사용 (주의
// 사항 2)

// 무방향간선 레코드
typedef struct Edge {
    int endpoint1; // 끝 정점 1
    int endpoint2; // 끝 정점 2
    int weight;    // 무게
} Edge;

// 간선리스트 구조의 무방향그래프 레코드 (정점은 번호 1 ~ n 자체로 충분)
typedef struct Graph {
    Edge edges[MAX_EDGE_COUNT];
} Graph;

// 전역 데이터
Graph G;
int n, m; // n = 정점 수, m = 간선 수
int H[MAX_EDGE_COUNT +
      1];     // 우선순위 큐: 간선 인덱스를 저장하는 최소힙 (키 = weight)
int heapSize; // 힙에 남아 있는 원소 수
int parent[MAX_VERTEX_COUNT +
           1]; // 분리집합: parent[v] == v 이면 v가 집합의 대표
int setSize[MAX_VERTEX_COUNT + 1]; // 대표 정점이 가진 집합의 크기

// 표준입력으로부터 간선리스트 구축
void buildGraph(void) {
    int i;

    scanf("%d %d", &n, &m);
    for (i = 0; i < m; i++)
        scanf("%d %d %d", &G.edges[i].endpoint1, &G.edges[i].endpoint2,
              &G.edges[i].weight);
}

// 힙 관련 함수 (H[1:heapSize])
int key(int i) { return G.edges[H[i]].weight; }

int isEmpty(void) { return heapSize == 0; }

// 힙의 i, j 위치 원소를 교환
void swapElements(int i, int j) {
    int tmp = H[i];

    H[i] = H[j];
    H[j] = tmp;
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

// 모든 간선을 넣고 상향식으로 힙 생성: O(m)
void buildHeap(void) {
    int i;

    heapSize = m;
    for (i = 1; i <= m; i++)
        H[i] = i - 1; // 간선 배열 인덱스 0 ~ m-1

    for (i = heapSize / 2; i >= 1; i--)
        downHeap(i);
}

// 무게가 가장 작은 간선을 삭제하여 반환: O(log m)
int removeMin(void) {
    int e = H[1];

    H[1] = H[heapSize];
    heapSize--;
    downHeap(1);
    return e;
}

// 분리집합 관련 함수
// 각 정점을 원소 하나짜리 집합으로 초기화
void makeSets(void) {
    int v;

    for (v = 1; v <= n; v++) {
        parent[v] = v;
        setSize[v] = 1;
    }
}

// v가 속한 집합의 대표 반환 (경로 압축)
int findSet(int v) {
    if (parent[v] != v)
        parent[v] = findSet(parent[v]);
    return parent[v];
}

// 대표가 a, b인 두 집합을 합침 (작은 집합을 큰 집합 밑으로)
void unionSets(int a, int b) {
    if (setSize[a] < setSize[b]) {
        parent[a] = b;
        setSize[b] += setSize[a];
    } else {
        parent[b] = a;
        setSize[a] += setSize[b];
    }
}

// Kruskal 알고리즘: O(m log m)
// MST에 추가되는 간선의 무게를 차례로 인쇄하고 총무게를 반환
int kruskalMST(void) {
    int e, u, w, count, total;

    makeSets();
    buildHeap();

    count = 0; // MST에 들어간 간선 수 (n - 1개가 되면 종료)
    total = 0;
    while (count < n - 1 && !isEmpty()) {
        e = removeMin();
        u = findSet(G.edges[e].endpoint1);
        w = findSet(G.edges[e].endpoint2);

        // 두 끝점이 다른 집합이면 싸이클이 생기지 않으므로 채택
        if (u != w) {
            unionSets(u, w);
            printf(" %d", G.edges[e].weight); // MST에 추가되는 간선 무게
            total += G.edges[e].weight;
            count++;
        }
    }
    printf("\n");

    return total;
}

int main(void) {
    buildGraph();
    printf("%d\n", kruskalMST());
    return 0;
}
