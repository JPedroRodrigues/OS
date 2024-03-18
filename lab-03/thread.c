#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// The child thread will execute this function
void* threadFunction(void* argument) {
    char* childName = (char*)argument;
    printf("Child receiving string: %s\n", childName);
    strcpy(childName, "Cleiton");
    printf("Child exiting...%s\n", childName);
    return NULL;
}

int main() {
    pthread_t thread_id;

    char name[] = "Klaythom"; // Modified to an array to allow modification
    printf("Parent name: %s\n", name);

    // Create thread
    pthread_create(&thread_id, NULL, threadFunction, (void*)name);

    // Wait for the thread to finish
    pthread_join(thread_id, NULL);

    printf("Parent name after thread: %s\n", name);

    return 0;
}
