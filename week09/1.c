#include <stdio.h>
#include <stdlib.h>

// 분리연쇄법 해시테이블의 노드
// 각 버켓은 단일연결리스트의 헤드 포인터
typedef struct Node {
    int key;
    struct Node *next;
} Node;

Node **table; // 해시테이블 (버켓 배열)
int M;        // 해시테이블의 크기

// 노드 하나를 생성
Node *get_node(int k) {
    Node *w = (Node *)malloc(sizeof(Node));
    if (w == NULL)
        exit(1);

    w->key = k;
    w->next = NULL;
    return w;
}

// 해시함수 h(x) = x % M
int h(int x) { return x % M; }

// 크기 M인 빈 해시테이블 초기화
void init_bucket_array(void) {
    int i;

    table = (Node **)malloc(sizeof(Node *) * M);
    if (table == NULL)
        exit(1);

    for (i = 0; i < M; i++)
        table[i] = NULL;
}

// 키 k를 해당 버켓 리스트의 맨 앞에 삽입
void insert_item(int k) {
    int v = h(k);
    Node *w = get_node(k);

    w->next = table[v];
    table[v] = w;
}

// 키 k가 있으면 리스트에서의 순위(1부터), 없으면 0 반환
int find_element(int k) {
    Node *w = table[h(k)];
    int rank = 1;

    while (w != NULL) {
        if (w->key == k)
            return rank;
        w = w->next;
        rank++;
    }
    return 0;
}

// 키 k를 삭제하고 리스트에서의 순위를 반환, 없으면 0 반환
int remove_element(int k) {
    int v = h(k);
    Node *w = table[v];
    Node *prev = NULL;
    int rank = 1;

    while (w != NULL) {
        if (w->key == k) {
            if (prev == NULL)
                table[v] = w->next; // 맨 앞 노드를 삭제하는 경우
            else
                prev->next = w->next;

            free(w);
            return rank;
        }
        prev = w;
        w = w->next;
        rank++;
    }
    return 0;
}

// 0번 주소부터 마지막 주소까지 저장된 키를 방문 순서대로 인쇄
void print_table(void) {
    Node *w;
    int i;

    for (i = 0; i < M; i++) {
        for (w = table[i]; w != NULL; w = w->next)
            printf(" %d", w->key);
    }
    printf("\n");
}

// 모든 버켓 리스트와 버켓 배열 메모리 해제
void free_table(void) {
    Node *w, *next;
    int i;

    for (i = 0; i < M; i++) {
        w = table[i];
        while (w != NULL) {
            next = w->next;
            free(w);
            w = next;
        }
    }
    free(table);
}

int main(void) {
    char cmd;
    int k;

    scanf("%d", &M);
    init_bucket_array();

    while (scanf(" %c", &cmd) == 1) {
        if (cmd == 'e')
            break;
        else if (cmd == 'p')
            print_table();
        else {
            scanf("%d", &k);

            if (cmd == 'i')
                insert_item(k);
            else if (cmd == 's')
                printf("%d\n", find_element(k));
            else if (cmd == 'd')
                printf("%d\n", remove_element(k));
        }
    }

    free_table();
    return 0;
}
