#include <stdio.h>
#include <stdlib.h>

int binarySearch(int *L, int l, int r, int k) {
    int m;

    // 탐색 범위를 벗어나면 k보다 작은 값 중 가장 큰 위치 반환
    if (l > r)
        return r;

    m = (l + r) / 2; // 중간 위치

    if (L[m] == k)
        return m;
    else if (L[m] > k)
        return binarySearch(L, l, m - 1, k); // 왼쪽 탐색
    else
        return binarySearch(L, m + 1, r, k); // 오른쪽 탐색
}

int main(void) {
    int n, k, i;
    int *L;

    scanf("%d %d", &n, &k);

    // 크기 n인 배열 동적 할당
    L = (int *)malloc(sizeof(int) * n);

    for (i = 0; i < n; i++)
        scanf("%d", &L[i]);

    printf(" %d\n", binarySearch(L, 0, n - 1, k));

    free(L);
    return 0;
}
