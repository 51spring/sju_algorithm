#include <stdio.h>
#include <stdlib.h>

void insertionSort(int arr[], int n) {
    int key, j;
    for (int i = 1; i < n; i++) {
        key = arr[i];
        for (j = i - 1; j >= 0 && arr[j] > key; j--) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = key;
    }
}

int main() {
    int n;
    int *arr;

    scanf("%d", &n);
    arr = (int *)malloc(sizeof(int) * n);

    if (arr == NULL) {
        printf("malloc fail\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    insertionSort(arr, n);

    for (int j = 0; j < n; j++) {
        printf(" %d", arr[j]);
    }

    printf("\n");
    free(arr);
    return 0;
}