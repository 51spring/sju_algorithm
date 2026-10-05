#include <stdio.h>
#include <stdlib.h>

int binarySearch(int *L, int n, int k) {
    int l = 0;
    int r = n - 1;
    int m;

    while (l <= r) {
        m = (l + r) / 2;

        if (L[m] < k)
            l = m + 1;
        else
            r = m - 1;
    }

    return l;
}

int main(void) {
    int n, k, i;
    int *L;

    scanf("%d %d", &n, &k);

    L = (int *)malloc(sizeof(int) * n);
    for (i = 0; i < n; i++)
        scanf("%d", &L[i]);

    printf(" %d\n", binarySearch(L, n, k));

    free(L);
    return 0;
}
