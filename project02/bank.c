/**
 * Name                                 RA
 * Enzo Guarnieri                       10410074
 * João Pedro Rodrigues Vieira          10403595
 * Sabrina Midori F. T. de Carvalho     10410220
 * 
 * Professor: Eduardo Santos
 * Course: Operational Systems
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h> // Adicionando a inclusão da biblioteca pthread para lidar com threads

typedef struct c {
    int balance;
} conta;

conta from, to;
int value;

pthread_mutex_t mutex_from; // Mutex to protect 'from' account

// Transfer function
void *transferencia(void *arg) {
    pthread_mutex_lock(&mutex_from); // Locks the "from" account

    // Verifies if "from" has sufficient balance
    if (from.balance >= value) { 
        from.balance -= value;
        to.balance += value;
    }

    printf("\nTransfer made with success!\n");
    printf("- C1's balance: %d\n", from.balance);
    printf("- C2's balance: %d\n\n", to.balance);

    pthread_mutex_unlock(&mutex_from); // Unlocks "from" account
}


int main() {
    pthread_mutex_init(&mutex_from, NULL); // Initialize mutex

    // Accounts start with $10,000.00
    from.balance = 10000;
    to.balance = 10000;
    value = 10;

    printf("Transfering %d from \"C1\" to account \"C2\"...\n");
    printf("C1's balance: $%d.00\n", from.balance);
    printf("C2's balance: $%d.00\n", to.balance);

    int n = from.balance / value;
    pthread_t *threads = (pthread_t *) malloc(n * sizeof(pthread_t)); 

    // Creating transfer threads
    for (int i = 0; i < n; i++) {
        if (pthread_create(&threads[i], NULL, transferencia, NULL) != 0) {
            perror("pthread_create");
            exit(2);
        }
    }

    // Waiting threads to resolve
    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(3);
        }
    }

    printf("|======================================================|\n");
    printf("|           Transfers finished with success!           |\n");
    printf("|======================================================|\n");
    printf("| Final balance from \"C1\": %d\n", from.balance);
    printf("| Final balance from \"C2\": %d\n", to.balance);
    printf("|======================================================|\n");

    // Destroy mutex
    pthread_mutex_destroy(&mutex_from);

    free(threads);

    return 0;
}