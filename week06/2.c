#include <stdio.h>
#include <stdlib.h>

int binarySearch(int *L, int n, int k) {
    int l = 0;
    int r = n - 1;
    int m;

    while (l <= r) {
        m = (l + r) / 2; // 중간 위치

        if (L[m] < k)
            l = m + 1; // k 이상인 값을 오른쪽에서 탐색
        else
            r = m - 1; // 더 작은 위치에 가능한 값이 있는지 탐색
    }

    // k 이상인 값 중 가장 작은 값의 위치
    return l;
}

int main(void) {
    int n, k, i;
    int *L;

    scanf("%d %d", &n, &k);

    // 크기 n인 배열 동적 할당
    L = (int *)malloc(sizeof(int) * n);

    for (i = 0; i < n; i++)
        scanf("%d", &L[i]);

    printf(" %d\n", binarySearch(L, n, k));

    free(L);
    return 0;
}
