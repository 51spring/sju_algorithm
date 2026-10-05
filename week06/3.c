#include <stdio.h>

int findKey(int a, int b, int n) {
    int i, m;
    char answer;

    for (i = 0; i < n; i++) {
        scanf(" %c", &answer);

        m = (a + b) / 2; // 현재 범위의 중간값

        if (answer == 'Y')
            a = m + 1; // k > m
        else
            b = m; // k <= m
    }

    // 마지막에는 a와 b가 같은 값이 됨
    return a;
}

int main(void) {
    int a, b, n;

    scanf("%d %d %d", &a, &b, &n);

    printf("%d\n", findKey(a, b, n));

    return 0;
}
