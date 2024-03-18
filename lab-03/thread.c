#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// Funcao a ser executada pelo child
void* threadFunction(void* argument) {
    char* childName = (char*)argument;
    
    // Recebendo a string do parent
    printf("Child receiving string: %s\n", childName);

    // Mudando a string recebida
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
