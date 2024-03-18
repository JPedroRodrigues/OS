#define _GNU_SOURCE
#include <stdlib.h> 
#include <sys/types.h> 
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// The child thread will execute this function
void* threadFunction( void* argument ) {
	// receiving parent name
        char *childName = (char *) argument;
        printf("Child receiving string: %s\n", childName);
       
	strcpy(childName, "Cleiton");
        printf("Child exiting...%s\n", childName);             
} 
 

int main() { 
        pthread_t thread_id;

        char *name = "Klaythom";
        printf("Parent name: %s\n", name);

        // create thread
        pthread_create(&thread_id, NULL, threadFunction, (void *) &name);

        // waiting thread resolve
        pthread_join(thread_id, NULL);

        return 0;
}

