#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int **a, *b, *c;
int lin, col, t;


void **mult(void *args) {
    for (int i = 0; i < lin; i++) {
        c[i] = 0;
        for (int j = 0; j < col; j++) {
            c[i] += a[i][j] * b[j];
        }
    }
}


int main() {
    lin = col = 6;
    t = 3;
    
    a = (int **) malloc(lin * sizeof(int *));
    for (int i = 0; i < lin; i++) a[i] = (int *) malloc(col * sizeof(int));

    b = (int *) malloc(col * sizeof(int));

    c = (int *) malloc(lin * sizeof(int ));

    pthread_t *threads = (pthread_t *) malloc(t * sizeof(pthread_t));

    // Freeing everything that I can
    for (int i = 0; i < lin; i++) free(a[i]);
    free(a);
    free(b);
    free(c);
    free(threads);
    return 0;
}