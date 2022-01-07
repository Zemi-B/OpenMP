#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

#define X 5716
#define Y 3731
#define W 50

int clamp(int v, int low, int high) {
    return v < low ? low : v > high ? high : v;
}

unsigned char data[Y][X];
unsigned char ans[Y - W][X - W];

void filter() {
#pragma omp parallel
    for (int i = 0; i < Y - W; ++i) {
#pragma omp for
        for (int j = 0; j < X - W; ++j) {
            int s = 0;
            for (int dx = 0; dx < W; ++dx) {
                for (int dy = 0; dy < W; ++dy) {
                    s += data[i + dx][j + dy];
                }
            }
            ans[i][j] = clamp(s / W / W, 0, 255);
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
            if (i != 1)
                fputc(ch, fpout);
            if (ch == '\n')
                break;
        }
        if (i == 1) {
            fprintf(fpout, "%d %d\n", X - W, Y - W);
        }
    }

    for (int i = 0; i < Y; ++i) {
        for (int j = 0; j < X; ++j) {
            ch = fgetc(fpin);
            data[i][j] = ch;
        }
    }

    double st = omp_get_wtime();

    filter();

    double en = omp_get_wtime();

    for (int i = 0; i < Y - W; ++i) {
        for (int j = 0; j < X - W; ++j) {
            fputc(ans[i][j], fpout);
        }
    }

    fclose(fpin);
    fclose(fpout);

#ifdef DEBUG
    printf("%lf\n", (en - st));
#else
    printf("Time : %lf [s]\n", (en - st));
#endif

    return 0;
}
