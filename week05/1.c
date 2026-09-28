#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// 두 부리스트를 반환하기 위한 구조체
typedef struct {
    Node *L1;
    Node *L2;
} ListPair;

/* 리스트 크기 계산 */
int listSize(Node *L) {
    int n = 0;
    while (L != NULL) {
        n++;
        L = L->next;
    }
    return n;
}

// L을 크기 k인 L1과 크기 L-k인 L2로 분할
ListPair partition(Node *L, int k) {
    ListPair pair;
    Node *p = L;
    int i;

    for (i = 1; i < k; i++) // L1의 마지막 노드까지 이동해서 연결끊기
        p = p->next;

    pair.L1 = L;
    pair.L2 = p->next;
    p->next = NULL;
    return pair;
}

// 정렬된 L1, L2를 합병
Node *merge(Node *L1, Node *L2) {
    Node *head, *tail;

    if (L1 == NULL)
        return L2;
    if (L2 == NULL)
        return L1;

    // 노드 위치 결정
    if (L1->data <= L2->data) {
        head = L1;
        L1 = L1->next;
    } else {
        head = L2;
        L2 = L2->next;
    }
    tail = head;

    while (L1 != NULL && L2 != NULL) {
        if (L1->data <= L2->data) {
            tail->next = L1;
            L1 = L1->next;
        } else {
            tail->next = L2;
            L2 = L2->next;
        }
        tail = tail->next;
    }

    // 남은 리스트를 그대로 연결
    tail->next = (L1 != NULL) ? L1 : L2;
    return head;
}

// 합병 정렬: 정렬된 리스트의 헤드를 반환
Node *mergeSort(Node *L) {
    int n;
    ListPair pair;

    if (L == NULL || L->next == NULL)
        return L;

    n = listSize(L);
    pair = partition(L, n / 2);
    pair.L1 = mergeSort(pair.L1);
    pair.L2 = mergeSort(pair.L2);
    return merge(pair.L1, pair.L2);
}

int main(void) {
    int n, i, x;
    Node *head = NULL, *tail = NULL, *newNode, *p, *temp;

    scanf("%d", &n);

    // 크기 n인 단일연결리스트 동적 할당 및 저장
    for (i = 0; i < n; i++) {
        scanf("%d", &x);
        newNode = (Node *)malloc(sizeof(Node));
        newNode->data = x;
        newNode->next = NULL;
        if (head == NULL)
            head = newNode;
        else
            tail->next = newNode;
        tail = newNode;
    }

    head = mergeSort(head);

    for (p = head; p != NULL; p = p->next)
        printf(" %d", p->data);
    printf("\n");

    // 메모리 해제
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}
