#include <stdio.h>
#include <stdlib.h>

// 이진탐색트리의 노드
// 외부노드는 왼쪽, 오른쪽 자식이 모두 NULL인 노드
// 내부노드는 항상 왼쪽, 오른쪽에 자식 노드를 가지고 있음
typedef struct Node {
    int key;
    struct Node *parent;
    struct Node *lChild;
    struct Node *rChild;
} Node;

Node *root; // 트리의 루트 노드

// 외부노드 하나를 생성
Node *get_node(void) {
    Node *w = (Node *)malloc(sizeof(Node));
    if (w == NULL)
        exit(1);

    w->parent = NULL;
    w->lChild = NULL;
    w->rChild = NULL;
    return w;
}

// w가 외부노드이면 1, 내부노드이면 0 반환
int is_external(Node *w) { return w->lChild == NULL && w->rChild == NULL; }

int is_internal(Node *w) { return !is_external(w); }

// 외부노드 w를 키 k를 가진 내부노드로 확장
void expand_external(Node *w, int k) {
    w->key = k;
    w->lChild = get_node();
    w->rChild = get_node();
    w->lChild->parent = w;
    w->rChild->parent = w;
}

// 키 k가 있으면 해당 내부노드, 없으면 삽입될 위치의 외부노드 반환
Node *tree_search(Node *w, int k) {
    while (is_internal(w)) {
        if (k == w->key)
            return w;
        else if (k < w->key)
            w = w->lChild;
        else
            w = w->rChild;
    }
    return w;
}

// 키 k를 탐색하고 존재 여부를 반환
int find_element(int k) { return is_internal(tree_search(root, k)); }

// 키 k를 트리에 삽입
void insert_item(int k) {
    Node *w = tree_search(root, k);

    if (is_internal(w))
        return; // 중복 키가 있으면 삽입하지 않음

    expand_external(w, k);
}

// w의 중위순회 후계자를 반환 (오른쪽 부트리의 최솟값)
Node *in_order_succ(Node *w) {
    w = w->rChild;
    while (is_internal(w->lChild))
        w = w->lChild;
    return w;
}

// 외부노드 z와 그 부모를 제거하고 형제노드로 대체
// 대체된 형제노드의 주소를 반환
Node *reduce_external(Node *z) {
    Node *w = z->parent;
    Node *zs, *g;

    if (z == w->lChild)
        zs = w->rChild;
    else
        zs = w->lChild;

    g = w->parent;
    zs->parent = g;

    if (g == NULL)
        root = zs; // 삭제한 노드가 루트인 경우
    else if (w == g->lChild)
        g->lChild = zs;
    else
        g->rChild = zs;

    free(z);
    free(w);
    return zs;
}

// 키 k를 삭제하고 성공하면 1, 존재하지 않으면 0 반환
int remove_element(int k) {
    Node *w = tree_search(root, k);
    Node *z, *y;

    if (is_external(w))
        return 0; // 삭제할 키가 없음

    z = w->lChild;
    if (is_internal(z))
        z = w->rChild;

    if (is_external(z)) {
        // 자식 중 하나가 외부노드인 경우
        reduce_external(z);
    } else {
        // 자식 둘 다 내부노드이면 후계자의 키를 복사한 뒤 후계자 제거
        y = in_order_succ(w);
        w->key = y->key;
        reduce_external(y->lChild);
    }
    return 1;
}

// 전위순회 : 루트 -> 왼쪽 부트리 -> 오른쪽 부트리
void pre_order(Node *w) {
    if (is_external(w))
        return;

    printf(" %d", w->key);
    pre_order(w->lChild);
    pre_order(w->rChild);
}

// 내부노드와 외부노드 모두 메모리 해제
void free_tree(Node *w) {
    if (is_internal(w)) {
        free_tree(w->lChild);
        free_tree(w->rChild);
    }
    free(w);
}

int main(void) {
    char cmd;
    int k;

    root = get_node(); // 외부노드 하나로 이루어진 빈 트리 초기화

    while (scanf(" %c", &cmd) == 1) {
        if (cmd == 'q')
            break;
        else if (cmd == 'p') {
            pre_order(root);
            printf("\n");
        } else {
            scanf("%d", &k);

            if (cmd == 'i')
                insert_item(k);
            else if (cmd == 's') {
                if (find_element(k))
                    printf("%d\n", k);
                else
                    printf("X\n");
            } else if (cmd == 'd') {
                if (remove_element(k))
                    printf("%d\n", k);
                else
                    printf("X\n");
            }
        }
    }

    free_tree(root);
    return 0;
}
