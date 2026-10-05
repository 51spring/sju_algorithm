#include <stdio.h>
#include <stdlib.h>

int binarySearch(int *L, int l, int r, int k) {
    int m;

    if (l > r)
        return r;

    m = (l + r) / 2;

    if (L[m] == k)
        return m;
    else if (L[m] > k)
        return binarySearch(L, l, m - 1, k);
    else
        return binarySearch(L, m + 1, r, k);
}

int main(void) {
    int n, k, i;
    int *L;

    scanf("%d %d", &n, &k);

    L = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++)
        scanf("%d", &L[i]);

    printf(" %d\n", binarySearch(L, 0, n - 1, k));

    free(L);
    return 0;
}
