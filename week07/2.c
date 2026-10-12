#include <stdio.h>
#include <stdlib.h>

// AVL 트리의 노드 (외부노드의 높이는 0)
typedef struct Node {
    int key;
    int height;
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

    w->height = 0;
    w->parent = NULL;
    w->lChild = NULL;
    w->rChild = NULL;
    return w;
}

int is_external(Node *w) { return w->lChild == NULL && w->rChild == NULL; }

int is_internal(Node *w) { return !is_external(w); }

int max(int a, int b) { return a > b ? a : b; }

// 외부노드를 내부노드로 바꾸고 높이를 1로 설정
void expand_external(Node *w, int k) {
    w->key = k;
    w->lChild = get_node();
    w->rChild = get_node();
    w->lChild->parent = w;
    w->rChild->parent = w;
    w->height = 1;
}

// 키 k가 존재하면 해당 내부노드, 없으면 외부노드 반환
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

int find_element(int k) { return is_internal(tree_search(root, k)); }

// 자식 노드의 높이를 이용해 w의 높이를 갱신
int update_height(Node *w) {
    int h = 1 + max(w->lChild->height, w->rChild->height);
    if (w->height == h)
        return 0;
    w->height = h;
    return 1;
}

// 양쪽 부트리의 높이 차가 1 이하면 균형 상태
int is_balanced(Node *w) {
    int diff = w->lChild->height - w->rChild->height;
    return -1 <= diff && diff <= 1;
}

// z를 기준으로 왼쪽 회전 : z의 오른쪽 자식 y가 새 부트리의 루트
Node *rotate_left(Node *z) {
    Node *y = z->rChild;
    Node *t = y->lChild;
    Node *g = z->parent;

    y->parent = g;
    if (g == NULL)
        root = y;
    else if (z == g->lChild)
        g->lChild = y;
    else
        g->rChild = y;

    y->lChild = z;
    z->parent = y;
    z->rChild = t;
    t->parent = z;

    update_height(z); // 아래로 내려간 z부터 높이 갱신
    update_height(y);
    return y;
}

// z를 기준으로 오른쪽 회전 : z의 왼쪽 자식 y가 새 부트리의 루트
Node *rotate_right(Node *z) {
    Node *y = z->lChild;
    Node *t = y->rChild;
    Node *g = z->parent;

    y->parent = g;
    if (g == NULL)
        root = y;
    else if (z == g->lChild)
        g->lChild = y;
    else
        g->rChild = y;

    y->rChild = z;
    z->parent = y;
    z->lChild = t;
    t->parent = z;

    update_height(z);
    update_height(y);
    return y;
}

// x, y, z의 위치관계에 따라 LL, LR, RR, RL 회전 수행
// 개조된 부트리의 새 루트를 반환
Node *restructure(Node *x, Node *y, Node *z) {
    if (y == z->lChild) {
        if (x == y->lChild)
            return rotate_right(z); // LL : 단일 오른쪽 회전
        else {
            rotate_left(y);         // LR : 왼쪽 회전 후
            return rotate_right(z); // 오른쪽 회전
        }
    } else {
        if (x == y->rChild)
            return rotate_left(z); // RR : 단일 왼쪽 회전
        else {
            rotate_right(y);       // RL : 오른쪽 회전 후
            return rotate_left(z); // 왼쪽 회전
        }
    }
}

// 삽입 위치에서 루트 방향으로 올라가며 최초의 불균형 노드를 수리
void search_and_fix_after_insertion(Node *w) {
    Node *z = w->parent;
    Node *y, *x;

    while (z != NULL) {
        update_height(z);
        if (!is_balanced(z)) {
            // y, x는 새로 삽입한 w까지 내려가는 경로의 자식 노드
            y = (w->key < z->key) ? z->lChild : z->rChild;
            x = (w->key < y->key) ? y->lChild : y->rChild;
            restructure(x, y, z);
            return; // 삽입 후에는 최초의 불균형 한 번만 수리하면 됨
        }
        z = z->parent;
    }
}

// 키 k를 삽입한 후 AVL 높이균형을 복구
void insert_item(int k) {
    Node *w = tree_search(root, k);

    if (is_internal(w))
        return; // 중복 키는 삽입하지 않음

    expand_external(w, k);
    search_and_fix_after_insertion(w);
}

// 전위순회 : 루트 -> 왼쪽 -> 오른쪽
void pre_order(Node *w) {
    if (is_external(w))
        return;

    printf(" %d", w->key);
    pre_order(w->lChild);
    pre_order(w->rChild);
}

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

    root = get_node(); // 외부노드 하나로 된 빈 AVL 트리

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
            }
        }
    }

    free_tree(root);
    return 0;
}
