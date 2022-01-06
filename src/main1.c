#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

#define X 5716
#define Y 3731

int clamp(int v, int low, int high) {
    return v < low ? low : v > high ? high : v;
}

unsigned char data[Y][X];
unsigned char ans[Y][X];

int dx[] = {0, 1, -1, 0, 0};
int dy[] = {0, 0, 0, 1, -1};

void filter() {
#pragma omp parallel for
    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            int s = 0;
            for (int k = 0; k < 5; ++k) {
                s += data[(i + dx[k] + Y) % Y][(j + dy[k] + X) % X];
            }
            ans[i][j] = clamp(s / 5, 0, 255);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        puts("ERROR!");
        return 0;
    }
    omp_set_num_threads(NUM_THREADS);
    FILE *fpin, *fpout;
    char *fin = argv[1];
    char *fout = argv[2];

    fpin = fopen(fin, "r");
    fpout = fopen(fout, "w");

    char ch;
    for (int i = 0; i < 3; ++i) {
        while ((ch = fgetc(fpin)) != EOF) {
            fputc(ch, fpout);
            if (ch == '\n')
                break;
        }
    }

    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            ch = fgetc(fpin);
            data[i][j] = ch;
        }
    }

    double st = omp_get_wtime();

    for (int i = 0; i < 10; ++i) {
        filter();
    }

    double en = omp_get_wtime();

    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            fputc(ans[i][j], fpout);
        }
    }

    fclose(fpin);
    fclose(fpout);

#ifdef DEBUG
    printf("%lf\n", (en - st) / 10);
#else
    printf("Time : %lf [s]\n", (en - st) / 10);
#endif

    return 0;
}
