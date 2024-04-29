#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int **a, *b, *c;
int row, col, t;


void print(int *a, int n) {
    printf("[%d", a[0]);
    for (int i = 1; i < n; i++) printf(", %d", a[i]);
    printf("]\n");
}


void *mult(void *args) {
    int *index = (int *) args;  

    for (int i = *index; i < *index + 1; i++) {
        c[i] = 0;
        for (int j = 0; j < col; j++) c[i] += a[i][j] * b[j];
    }

    return NULL;
}


int main() {
    row = 2;    // Number of rows
    col = 3;    // Number of columns
    t = row;    // Number of threads
    
    // Dynamically allocating memory for matrix A  
    a = (int **) malloc(row * sizeof(int *));
    for (int i = 0; i < row; i++) a[i] = (int *) malloc(col * sizeof(int));
    
    // Dynamically allocating memory for array B
    b = (int *) malloc(col * sizeof(int));
    
    // Dynamically allocating memory for array C (result)
    c = (int *) malloc(row * sizeof(int ));

    // Adding values to matrix A
    a[0][0] = 0;
    a[0][1] = 3;
    a[0][2] = 5;
    a[1][0] = 5;
    a[1][1] = 5;
    a[1][2] = 2;

    // Adding values to array B
    b[0] = 3;
    b[1] = 4;
    b[2] = 3;

    // Threads array and function calls
    pthread_t *threads = (pthread_t *) malloc(t * sizeof(pthread_t));
    int *thread_ids = (int *) malloc(t * sizeof(int));

    for (int i = 0; i < t; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, mult, (void *) &thread_ids[i]);
    }

    for (int i = 0; i < t; i++) pthread_join(threads[i], NULL);

    // Printing the resulting array C
    print(c, row);

    // Freeing everything that I can
    for (int i = 0; i < row; i++) free(a[i]);
    free(a);
    free(b);
    free(c);
    free(threads);

    return 0;
}