#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

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

// 오름차순 정렬 함수
void sortArray(int arr[], int n) {
    int temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// sortArray에서 정렬한 배열 역순으로 뒤집는 함수
void reverseArray(int arr[], int n) {
    int temp;

    for (int i = 0; i < n / 2; i++) {
        temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
}

// 랜덤 배열 생성
void createArray(int A[], int B[], int n) {
    int value;
    for (int i = 0; i < n; i++) {
        value = (int)(((long)rand() << 15 | rand()) % 1000000) + 1;
        A[i] = value;
        B[i] = value;
    }
}

// 선택정렬 실행시간 측정
double measureSelection(int arr[], int n, LARGE_INTEGER ticksPerSec) {
    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    selectionSort(arr, n);
    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart) /
           (double)ticksPerSec.QuadPart * 1000.0;
}

// 삽입정렬 실행시간 측정
double measureInsertion(int arr[], int n, LARGE_INTEGER ticksPerSec) {
    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    insertionSort(arr, n);
    QueryPerformanceCounter(&end);

    return (double)(end.QuadPart - start.QuadPart) /
           (double)ticksPerSec.QuadPart * 1000.0;
}

int main() {
    int n;
    int *A;
    int *B;
    double selectionTime, insertionTime;
    LARGE_INTEGER ticksPerSec;

    scanf("%d", &n);

    A = (int *)malloc(sizeof(int) * n);
    B = (int *)malloc(sizeof(int) * n);

    if (A == NULL || B == NULL) {
        printf("malloc fail\n");
        return 1;
    }

    // 난수 시드 설정
    srand(time(NULL));
    // 타이머 초당 카운트 수
    QueryPerformanceFrequency(&ticksPerSec);

    // A 실험 (정렬X 배열)
    createArray(A, B, n);
    selectionTime = measureSelection(A, n, ticksPerSec);
    insertionTime = measureInsertion(B, n, ticksPerSec);
    printf("\n\nA 실험 결과\n%d번 실행\n선택정렬: %.12f\n삽입정렬: %12f\n\n", n,
           selectionTime, insertionTime);

    // B 실험 (정렬 O 배열)
    createArray(A, B, n);
    sortArray(A, n);
    sortArray(B, n);

    selectionTime = measureSelection(A, n, ticksPerSec);
    insertionTime = measureInsertion(B, n, ticksPerSec);
    printf("\n\nB 실험 결과\n%d번 실행\n선택정렬: %.12f\n삽입정렬: %12f\n\n", n,
           selectionTime, insertionTime);

    // C 실험 (역순 정렬 배열)
    createArray(A, B, n);
    sortArray(A, n);
    sortArray(B, n);
    reverseArray(A, n);
    reverseArray(B, n);

    selectionTime = measureSelection(A, n, ticksPerSec);
    insertionTime = measureInsertion(B, n, ticksPerSec);
    printf("\n\nC 실험 결과\n%d번 실행\n선택정렬: %.12f\n삽입정렬: %12f\n\n", n,
           selectionTime, insertionTime);

    free(A);
    free(B);
    return 0;
}
