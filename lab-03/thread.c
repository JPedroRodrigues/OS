#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// The child thread will execute this function
void* threadFunction(void* argument) {
    char* childName = (char*)argument;
    
    // receiving parent string
    printf("Child receiving string: %s\n", childName);

    // changing the received string
    strcpy(childName, "Cleiton");
    printf("Child exiting...%s\n", childName);

    return NULL;
}


int main() {
    pthread_t thread_id;

    char name[] = "Klaythom";
    printf("Parent name: %s\n", name);

    // Criacao da thread
    pthread_create(&thread_id, NULL, threadFunction, (void*)name);

    // Esperando a execucao da thread
    pthread_join(thread_id, NULL);

    printf("Parent name after thread: %s\n", name);

    return 0;
}
