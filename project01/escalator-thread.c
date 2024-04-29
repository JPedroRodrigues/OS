#include <stdio.h>
#include <pthread.h>
#define MAXN 10000

int out = 0;

typedef struct {
    int time;
    int direction;
} Person;


typedef struct {
    Person fQueue[MAXN];
    int lFQ;
    Person sQueue[MAXN];
    int lSQ;
} ThreadArgs;


void* escalator(void* args) {
    ThreadArgs* eArgs = (ThreadArgs *) args;
    Person first;

    // Setting the first value;
    if (eArgs->lFQ == 0) first = eArgs->sQueue[0];
    else if (eArgs->lSQ == 0) first = eArgs->fQueue[0];
    else first = eArgs->fQueue[0].time < eArgs->sQueue[0].time ? eArgs->fQueue[0] : eArgs->sQueue[0];

    int aux1 = 0, aux2 = 0;

    while (aux1 < eArgs->lFQ || aux2 < eArgs->lSQ) {
        
        // going from left to right
        if (first.direction == 0) {

            // follows the same direction if the time of arrival is less than the last exit time or it's immediately next to the previous one
            if ((aux1 < eArgs->lFQ && (eArgs->fQueue[aux1].time <= out) || (eArgs->fQueue[aux1].time > out && eArgs->fQueue[aux1].time < eArgs->sQueue[aux2].time)) || aux2 == eArgs->lSQ) {
                first = eArgs->fQueue[aux1];               
                ++aux1;   

            // if the time of arrival is greater than the exit time or the queue is completely used 
            } else if (eArgs->fQueue[aux1].time > out || aux1 == eArgs->lFQ) {
                // change the escalator's direction, considering a new time of arrival based on the waiting time
                first = eArgs->sQueue[aux2];
                if (out > first.time) first.time = out;
                ++aux2;

                // if the exit time is greater than the following ones
                int i = aux2;
                while (out > eArgs->sQueue[i].time && i < eArgs->lSQ) {
                    eArgs->sQueue[i].time = out;
                    ++i;
                }
            } 
        // going from right to left                               
        } else if (first.direction == 1) {
            
            // follows the same direction if the time of arrival is less than the last exit time or it's immediately next to the previous one
            if ((aux2 < eArgs->lSQ && eArgs->sQueue[aux2].time <= out || (eArgs->sQueue[aux2].time > out && eArgs->sQueue[aux2].time < eArgs->fQueue[aux1].time)) || aux1 == eArgs->lFQ) {
                first = eArgs->sQueue[aux2];      
                ++aux2;

            // if the time of arrival is greater than the exit time or the queue is completely used                                               
            } else if (eArgs->sQueue[aux2].time > out || aux2 == eArgs->lSQ) {
                // change the escalator's direction, considering a new time of arrival based on the waiting time
                first = eArgs->fQueue[aux1];
                if (out > first.time) first.time = out;
                ++aux1;

                // if the exit time is greater than the following ones
                int i = aux1;
                while (out > eArgs->fQueue[i].time && i < eArgs->lFQ) {
                    eArgs->fQueue[i].time = out;
                    ++i;
                }                
            }
        }   
        out = first.time + 10;
    }
    return NULL;
}


int main() {
    pthread_t thread_id;

    // n = number of people (cases); t = time of arrival; d = direction (0: left to right; 1: right to left)
    int n;
    Person person;
    scanf("%d", &n);

    Person firstQueue[MAXN];
    Person secondQueue[MAXN];

    /**
     * lenFQ = length of fQueue queue
     * lenSQ = length of sQueue queue
    */
    int lenFQ = 0, lenSQ = 0;

    // read all input values and gets the first one
    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &person.time, &person.direction);
        if (person.direction == 0) firstQueue[lenFQ++] = person;
        else secondQueue[lenSQ++] = person;        
    }

    // Setting up the args to thread
    ThreadArgs threadArgs;

    for (int i = 0; i < lenFQ; i++) threadArgs.fQueue[i] = firstQueue[i];

    threadArgs.lFQ = lenFQ;

    for (int i = 0; i < lenSQ; i++) threadArgs.sQueue[i] = secondQueue[i];

    threadArgs.lSQ = lenSQ;

    pthread_create(&thread_id, NULL, escalator, (void *) &threadArgs);

    pthread_join(thread_id, NULL);

    printf("Out: %d\n", out);

    return 0;
}