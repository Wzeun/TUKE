#include <stdio.h>

double calc(double k, int n, int s[], int v[]) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += (double)s[i] / (v[i] + k);
    }
    return sum;
}

int main() {
    int n;
    double t;

    scanf("%d %lf", &n, &t);

    int s[1005], v[1005];

    int min_v = 1000000;

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &s[i], &v[i]);
        if (v[i] < min_v) min_v = v[i];
    }

    double left = -min_v + 1e-9; // важно!
    double right = 1e6;

    for (int i = 0; i < 100; i++) {
        double mid = (left + right) / 2.0;

        if (calc(mid, n, s, v) > t)
            left = mid;
        else
            right = mid;
    }

    printf("%.9f\n", (left + right) / 2.0);

    return 0;
}