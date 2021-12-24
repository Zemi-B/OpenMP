#include <omp.h>
#include <stdio.h>

#ifndef NUM_THREADS
#define NUM_THREADS 1
#endif

#define X 512
#define Y 512

int clamp(int v, int low, int high) {
    return v < low ? low : v > high ? high : v;
}

unsigned char data[X][Y];
unsigned char ans[X][Y];

int dx[] = {0, 1, -1, 0, 0};
int dy[] = {0, 0, 0, 1, -1};

int main(void) {
    omp_set_num_threads(NUM_THREADS);
    FILE *fpin, *fpout;  // FILE型構造体
    char fin[] = "lena.pgm";
    char fout[] = "lena_out.pgm";

    fpin = fopen(fin, "r");  // ファイルを開く。失敗するとNULLを返す。
    fpout = fopen(fout, "w");

    char ch;
    for (int i = 0; i < 3; ++i) {
        while ((ch = fgetc(fpin)) != EOF) {
            fputc(ch, fpout);
            if (ch == '\n')
                break;
        }
    }

    for (int j = 0; j < Y; ++j) {
        for (int i = 0; i < X; ++i) {
            ch = fgetc(fpin);
            data[i][j] = ch;
        }
    }

    double st = omp_get_wtime();

    for (int j = 0; j < Y; ++j) {
#pragma omp parallel for
        for (int i = 0; i < X; ++i) {
            int s = 0;
            for (int k = 0; k < 5; ++k) {
                s += data[(i + dx[k] + X) % X][(j + dy[k] + Y) % Y];
            }
            ans[i][j] = clamp(s / 5, 0, 255);
        }
    }

    double en = omp_get_wtime();

    for (int j = 0; j < Y; ++j) {
        for (int i = 0; i < X; ++i) {
            fputc(ans[i][j], fpout);
        }
    }

    fclose(fpin);  // ファイルを閉じる
    fclose(fpout);

    printf("Time : %lf [s]\n", (en - st) / 10);

    return 0;
}
