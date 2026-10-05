#include <stdio.h>

int findKey(int a, int b, int n) {
    int i, m;
    char answer;

    for (i = 0; i < n; i++) {
        scanf(" %c", &answer);

        m = (a + b) / 2;

        if (answer == 'Y')
            a = m + 1;
        else
            b = m;
    }

    return a;
}

int main(void) {
    int a, b, n;

    scanf("%d %d %d", &a, &b, &n);

    printf("%d\n", findKey(a, b, n));

    return 0;
}
