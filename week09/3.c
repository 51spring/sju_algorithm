#include <stdio.h>
#include <stdlib.h>

// 개방주소법 해시테이블 (이중해싱)
// 키 값이 0인 버켓은 빈 버켓
int *table; // 해시테이블
int M;      // 해시테이블의 크기
int n;      // 최대 삽입 개수
int q;      // 두 번째 해시함수에 쓰이는 M보다 조금 작은 소수

// 첫 번째 해시함수 h(x) = x % M
int h(int x) { return x % M; }

// 두 번째 해시함수 h'(x) = q - (x % q)
int h2(int x) { return q - (x % q); }

// 크기 M인 빈 해시테이블 초기화
void init_bucket_array(void) {
    int i;

    table = (int *)malloc(sizeof(int) * M);
    if (table == NULL)
        exit(1);

    for (i = 0; i < M; i++)
        table[i] = 0;
}

// 이중해싱으로 i번째 조사할 주소 반환
int get_next_bucket(int v, int i, int k) { return (v + i * h2(k)) % M; }

// 키 k를 삽입하고 충돌 횟수만큼 C를 인쇄한 뒤 저장된 주소를 인쇄
void insert_item(int k) {
    int v = h(k);
    int i = 0;
    int b;

    while (i < M) {
        b = get_next_bucket(v, i, k);
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
        b = get_next_bucket(v, i, k);
        if (table[b] == 0)
            return -1; // 빈 버켓을 만나면 탐색 실패
        else if (table[b] == k)
            return b;
        i++;
    }
    return -1;
}

// 0번 주소부터 마지막 주소까지 해시테이블 인쇄
void print_table(void) {
    int i;

    for (i = 0; i < M; i++)
        printf(" %d", table[i]);
    printf("\n");
}

int main(void) {
    char cmd;
    int k, b;

    scanf("%d %d %d", &M, &n, &q);
    init_bucket_array();

    while (scanf(" %c", &cmd) == 1) {
        if (cmd == 'e') {
            print_table(); // 종료 전 해시테이블 인쇄
            break;
        } else if (cmd == 'p')
            print_table();
        else {
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
    }

    free(table);
    return 0;
}