#include <stdio.h>
#include <stdlib.h>

void selectionSort(int arr[], int n) {
    int maxIndex, temp;
    for (int i = n - 1; i > 0; i--) {
        maxIndex = 0;

        for (int j = 1; j <= i; j++) {
            if (arr[j] > arr[maxIndex]) {
                maxIndex = j;
            }
        }
        temp = arr[i];
        arr[i] = arr[maxIndex];
        arr[maxIndex] = temp;
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

    selectionSort(arr, n);

    for (int j = 0; j < n; j++) {
        printf(" %d", arr[j]);
    }

    printf("\n");
    free(arr);
    return 0;
}