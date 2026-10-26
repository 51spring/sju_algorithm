#include <stdio.h>
#include <stdlib.h>

// 개방주소법 해시테이블 (선형조사법)
// 키 값이 0인 버켓은 빈 버켓
int *table; // 해시테이블
int M;      // 해시테이블의 크기
int n;      // 최대 삽입 개수

// 해시함수 h(x) = x % M
int h(int x) { return x % M; }

// 크기 M인 빈 해시테이블 초기화
void init_bucket_array(void) {
    int i;

    table = (int *)malloc(sizeof(int) * M);
    if (table == NULL)
        exit(1);

    for (i = 0; i < M; i++)
        table[i] = 0;
}

// 선형조사법으로 i번째 조사할 주소 반환
int get_next_bucket(int v, int i) { return (v + i) % M; }

// 키 k를 삽입하고 충돌 횟수만큼 C를 인쇄한 뒤 저장된 주소를 인쇄
void insert_item(int k) {
    int v = h(k);
    int i = 0;
    int b;

    while (i < M) {
        b = get_next_bucket(v, i);
        if (table[b] == 0) {
            table[b] = k;
            printf("%d\n", b);
            return;
        }
        printf("C"); // 충돌 발생
        i++;
    }
}

// 키 k가 있으면 저장된 주소, 없으면 -1 반환
int find_element(int k) {
    int v = h(k);
    int i = 0;
    int b;

    while (i < M) {
        b = get_next_bucket(v, i);
        if (table[b] == 0)
            return -1; // 빈 버켓을 만나면 탐색 실패
        else if (table[b] == k)
            return b;
        i++;
    }
    return -1;
}

int main(void) {
    char cmd;
    int k, b;

    scanf("%d %d", &M, &n);
    init_bucket_array();

    while (scanf(" %c", &cmd) == 1) {
        if (cmd == 'e')
            break;

        scanf("%d", &k);

        if (cmd == 'i')
            insert_item(k);
        else if (cmd == 's') {
            b = find_element(k);
            if (b == -1)
                printf("-1\n");
            else
                printf("%d %d\n", b, table[b]);
        }
    }

    free(table);
    return 0;
}