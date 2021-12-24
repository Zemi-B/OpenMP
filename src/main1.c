#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

#define X 3731
#define Y 5716

int clamp(int v, int low, int high) {
    return v < low ? low : v > high ? high : v;
}

unsigned char data[X][Y];
unsigned char ans[X][Y];

int dx[] = {0, 1, -1, 0, 0};
int dy[] = {0, 0, 0, 1, -1};

void filter() {
#ifdef PARALLEL_X
#pragma omp parallel for
#endif
    for (int i = 0; i < X; ++i) {
#ifdef PARALLEL_Y
#pragma omp parallel for
#endif
        for (int j = 0; j < Y; ++j) {
            int s = 0;
            for (int k = 0; k < 5; ++k) {
                s += data[(i + dx[k] + X) % X][(j + dy[k] + Y) % Y];
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

    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
            ch = fgetc(fpin);
            data[i][j] = ch;
        }
    }

    double st = omp_get_wtime();

    for (int i = 0; i < 10; ++i) {
        filter();
    }

    double en = omp_get_wtime();

    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
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
