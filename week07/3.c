#include <stdio.h>
#include <stdlib.h>

// AVL 트리 노드 : 외부노드는 높이 0, 내부노드는 높이 1 이상
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

// 외부노드 w에 키 k를 저장하고 두 개의 외부 자식을 생성
void expand_external(Node *w, int k) {
    w->key = k;
    w->lChild = get_node();
    w->rChild = get_node();
    w->lChild->parent = w;
    w->rChild->parent = w;
    w->height = 1;
}

// 키 k가 있으면 내부노드, 없으면 해당 위치의 외부노드 반환
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

// 높이를 다시 계산하고, 변경되었으면 1을 반환
int update_height(Node *w) {
    int h = 1 + max(w->lChild->height, w->rChild->height);
    if (w->height == h)
        return 0;
    w->height = h;
    return 1;
}

// 양쪽 자식의 높이 차가 1 이하면 균형 상태
int is_balanced(Node *w) {
    int diff = w->lChild->height - w->rChild->height;
    return -1 <= diff && diff <= 1;
}

// 왼쪽 회전 후 새 부트리의 루트 반환
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

    update_height(z);
    update_height(y);
    return y;
}

// 오른쪽 회전 후 새 부트리의 루트 반환
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

// LL, LR, RR, RL 중 알맞은 회전을 수행
Node *restructure(Node *x, Node *y, Node *z) {
    if (y == z->lChild) {
        if (x == y->lChild)
            return rotate_right(z); // LL
        else {
            rotate_left(y); // LR
            return rotate_right(z);
        }
    } else {
        if (x == y->rChild)
            return rotate_left(z); // RR
        else {
            rotate_right(y); // RL
            return rotate_left(z);
        }
    }
}

// 삽입 후 최초로 불균형해진 노드에서 한 번 개조
void search_and_fix_after_insertion(Node *w) {
    Node *z = w->parent;
    Node *y, *x;

    while (z != NULL) {
        update_height(z);
        if (!is_balanced(z)) {
            y = (w->key < z->key) ? z->lChild : z->rChild;
            x = (w->key < y->key) ? y->lChild : y->rChild;
            restructure(x, y, z);
            return;
        }
        z = z->parent;
    }
}

void insert_item(int k) {
    Node *w = tree_search(root, k);

    if (is_internal(w))
        return;

    expand_external(w, k);
    search_and_fix_after_insertion(w);
}

// 오른쪽 부트리에서 왼쪽으로 끝까지 이동해 중위순회 후계자 탐색
Node *in_order_succ(Node *w) {
    w = w->rChild;
    while (is_internal(w->lChild))
        w = w->lChild;
    return w;
}

// 외부노드 z와 그 부모노드를 삭제하고 형제노드로 대체
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
        root = zs;
    else if (w == g->lChild)
        g->lChild = zs;
    else
        g->rChild = zs;

    free(z);
    free(w);
    return zs;
}

// 삭제 후에는 루트까지 여러 노드가 불균형해질 수 있으므로 계속 검사
void search_and_fix_after_removal(Node *z) {
    Node *y, *x, *b;
    int lh, rh;

    while (z != NULL) {
        update_height(z);

        if (!is_balanced(z)) {
            // y : 더 높은 부트리의 루트
            if (z->lChild->height > z->rChild->height)
                y = z->lChild;
            else
                y = z->rChild;

            // x : y의 더 높은 자식
            lh = y->lChild->height;
            rh = y->rChild->height;
            if (lh > rh)
                x = y->lChild;
            else if (lh < rh)
                x = y->rChild;
            else {
                // 높이가 같으면 y가 붙어 있는 방향의 자식을 선택
                // 삭제에서는 같은 높이일 때 바깥쪽 자식을 선택해야 단일회전
                if (y == z->lChild)
                    x = y->lChild;
                else
                    x = y->rChild;
            }

            b = restructure(x, y, z);
            z = b->parent; // 회전한 부트리의 부모부터 다시 검사
        } else
            z = z->parent;
    }
}

// 키 k를 삭제하고 성공하면 1, 키가 없으면 0 반환
int remove_element(int k) {
    Node *w = tree_search(root, k);
    Node *z, *y, *zs;

    if (is_external(w))
        return 0;

    z = w->lChild;
    if (is_internal(z))
        z = w->rChild;

    if (is_external(z)) {
        // 자식 중 하나가 외부노드인 경우
        zs = reduce_external(z);
    } else {
        // 자식 둘 다 내부노드인 경우 후계자의 키로 대체
        y = in_order_succ(w);
        w->key = y->key;
        zs = reduce_external(y->lChild);
    }

    // 삭제된 위치부터 조상 노드들의 높이균형을 복구
    search_and_fix_after_removal(zs->parent);
    return 1;
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

    root = get_node(); // 빈 AVL 트리 초기화

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
