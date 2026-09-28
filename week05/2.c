#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

// l과 r 사이의 위치 하나를 무작위로 선택
int findPivot(int *L, int l, int r) { return l + rand() % (r - l + 1); }

void inPlacePartition(int *L, int l, int r, int k, int *a, int *b) {
    int pivot = L[k];
    int lt = l; // L[l ~ lt-1] < pivot
    int i = l;  // L[lt ~ i-1] == pivot
    int gt = r; // L[gt+1 ~ r] > pivot

    while (i <= gt) {
        if (L[i] < pivot) {
            swap(&L[i], &L[lt]);
            lt++;
            i++;
        } else if (L[i] > pivot) {
            swap(&L[i], &L[gt]);
            gt--;
        } else {
            i++;
        }
    }
    *a = lt;
    *b = gt;
}

void inPlaceQuickSort(int *L, int l, int r) {
    int k, a, b;

    if (l >= r)
        return;

    k = findPivot(L, l, r);
    inPlacePartition(L, l, r, k, &a, &b);
    inPlaceQuickSort(L, l, a - 1);
    inPlaceQuickSort(L, b + 1, r);
}

int main(void) {
    int n, i;
    int *L;

    srand((unsigned int)time(NULL));

    scanf("%d", &n);
    L = (int *)malloc(sizeof(int) * n); // 크기 n인 배열 동적 할당
    for (i = 0; i < n; i++)
        scanf("%d", &L[i]);

    inPlaceQuickSort(L, 0, n - 1);

    for (i = 0; i < n; i++)
        printf(" %d", L[i]);
    printf("\n");

    free(L);
    return 0;
}
