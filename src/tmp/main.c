#include <omp.h>
#include <stdio.h>

int main(void) {
    printf("max thread : %d\n", omp_get_max_threads());

    return 0;
}
