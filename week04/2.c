#include <stdio.h>

void downHeap(int Heap[], int n, int i) {
    int left = 2 * i;
    int right = 2 * i + 1;
    int bigger;
    int temp;

    if (left > n)
        return;

    bigger = left;

    if (right <= n && Heap[right] > Heap[left])
        bigger = right;

    if (Heap[i] >= Heap[bigger])
        return;

    temp = Heap[i];
    Heap[i] = Heap[bigger];
    Heap[bigger] = temp;

    downHeap(Heap, n, bigger);
}

void buildHeap(int Heap[], int n) {
    int i;

    for (i = n / 2; i >= 1; i--)
        downHeap(Heap, n, i);
}

void inPlaceHeapSort(int Heap[], int n) {
    int i;
    int temp;

    buildHeap(Heap, n);

    for (i = n; i >= 2; i--) {
        temp = Heap[1];
        Heap[1] = Heap[i];
        Heap[i] = temp;

        downHeap(Heap, i - 1, 1);
    }
}

void printArray(int Heap[], int n) {
    int i;

    for (i = 1; i <= n; i++)
        printf(" %d", Heap[i]);

    printf("\n");
}

int main(void) {
    int Heap[100];
    int n;
    int i;

    scanf("%d", &n);

    for (i = 1; i <= n; i++)
        scanf("%d", &Heap[i]);

    inPlaceHeapSort(Heap, n);

    printArray(Heap, n);

    return 0;
}
