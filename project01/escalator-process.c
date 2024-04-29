#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define MAXN 10000

typedef struct {
    int time;
    int direction;
} Person;

typedef struct {
    Person fQueue[MAXN];
    int lFQ;
    Person sQueue[MAXN];
    int lSQ;
} processArgs;

int escalator(Person *firstQueue, int lenFQ, Person *secondQueue, int lenSQ) {
    Person first;

    // Setting the first value;
    if (lenFQ == 0) first = secondQueue[0];
    else if (lenSQ == 0) first = firstQueue[0];
    else first = firstQueue[0].time < secondQueue[0].time ? firstQueue[0] : secondQueue[0];

    int aux1 = 0, aux2 = 0, out = 0;

    while (aux1 < lenFQ || aux2 < lenSQ) {
        
        // going from left to right
        if (first.direction == 0) {

            // follows the same direction if the time of arrival is less than the last exit time or it's immediately next to the previous one
            if ((aux1 < lenFQ && (firstQueue[aux1].time <= out) || (firstQueue[aux1].time > out && firstQueue[aux1].time < secondQueue[aux2].time)) || aux2 == lenSQ) {
                first = firstQueue[aux1];               
                ++aux1;   

            // if the time of arrival is greater than the exit time or the queue is completely used 
            } else if (firstQueue[aux1].time > out || aux1 == lenFQ) {
                // change the escalator's direction, considering a new time of arrival based on the waiting time
                first = secondQueue[aux2];
                if (out > first.time) first.time = out;
                ++aux2;

                // if the exit time is greater than the following ones
                int i = aux2;
                while (out > secondQueue[i].time && i < lenSQ) {
                    secondQueue[i].time = out;
                    ++i;
                }
            } 
        // going from right to left                               
        } else if (first.direction == 1) {
            
            // follows the same direction if the time of arrival is less than the last exit time or it's immediately next to the previous one
            if ((aux2 < lenSQ && secondQueue[aux2].time <= out || (secondQueue[aux2].time > out && secondQueue[aux2].time < firstQueue[aux1].time)) || aux1 == lenFQ) {
                first = secondQueue[aux2];      
                ++aux2;

            // if the time of arrival is greater than the exit time or the queue is completely used                                               
            } else if (secondQueue[aux2].time > out || aux2 == lenSQ) {
                // change the escalator's direction, considering a new time of arrival based on the waiting time
                first = firstQueue[aux1];
                if (out > first.time) first.time = out;
                ++aux1;

                // if the exit time is greater than the following ones
                int i = aux1;
                while (out > firstQueue[i].time && i < lenFQ) {
                    firstQueue[i].time = out;
                    ++i;
                }                
            }
        }   
        out = first.time + 10;
    }
    return out;
}


int main() {
    // n = number of people (cases); t = time of arrival; d = direction (0: left to right; 1: right to left)
    int n;
    Person person;
    scanf("%d", &n);

    Person firstQueue[MAXN];
    Person secondQueue[MAXN];

    /*
     * lenFQ = length of firstQueue queue
     * lenSQ = length of secondQueue queue
    */
    int lenFQ = 0, lenSQ = 0;

    // read all input values
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &person.time, &person.direction);
        if (person.direction == 0) firstQueue[lenFQ++] = person;
        else secondQueue[lenSQ++] = person;        
    }

    // Creating a shared memory space to pass as function args
    processArgs *pArg = (processArgs *)mmap(NULL, sizeof(processArgs), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int i = 0; i < lenFQ; i++) pArg->fQueue[i] = firstQueue[i];

    pArg->lFQ = lenFQ;

    for (int i = 0; i < lenSQ; i++) pArg->sQueue[i] = secondQueue[i];

    pArg->lSQ = lenSQ;

    // creating a child process
    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        return -1;
    } else if (pid == 0) {
        int out = escalator(pArg->fQueue, pArg->lFQ, pArg->sQueue, pArg->lSQ);

        printf("Out: %d\n", out);

        exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}