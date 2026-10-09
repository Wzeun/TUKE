#include <stdio.h>
#include <math.h>

#define MAXM 10005
#define PI acos(-1.0)


double sphere_part(int i, double z, double R[], double Z[]) {
    double r = R[i] / 1000.0;
    double zc = Z[i] / 1000.0;

    double bottom = zc - r;
    double top = zc + r;

    if (z <= bottom) return 0.0;
    if (z >= top) return (4.0/3.0) * PI * r * r * r;

    double h = z - bottom;
    return PI * h * h * (r - h / 3.0);
}


double volume(double z, int M, double R[], double Z[]) {
    double vol = 100.0 * 100.0 * z;

    for (int i = 0; i < M; i++) {
        vol -= sphere_part(i, z, R, Z);
    }

    return vol;
}

int main() {
    int M, S;
    scanf("%d %d", &M, &S);

    double R[MAXM], X[MAXM], Y[MAXM], Z[MAXM];

    for (int i = 0; i < M; i++) {
        scanf("%lf %lf %lf %lf", &R[i], &X[i], &Y[i], &Z[i]);
    }

    double total = volume(100.0, M, R, Z);
    double slice = total / S;

    double prev = 0.0;

    for (int i = 0; i < S; i++) {
        double left = prev, right = 100.0;

        for (int it = 0; it < 60; it++) {
            double mid = (left + right) / 2.0;

            if (volume(mid, M, R, Z) - volume(prev, M, R, Z) < slice)
                left = mid;
            else
                right = mid;
        }

        double z = (left + right) / 2.0;
        printf("%.9f\n", z - prev);

        prev = z;
    }

    return 0;
}