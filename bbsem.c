#define _GNU_SOURCE

#include "common.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Bounded buffers with semaphores */

/* Global constants */
#define BUF_SZ 10

/* Global varaiables */
int readloc = 0;
int writeloc = 0;
slot_t buffer[BUF_SZ];

sem_t full;
sem_t empty;
sem_t mutex;

char *msg;

/* Consumers */
void i_consumer() {
    sem_wait(&full);
    sem_wait(&mutex);

    /* Read a single location from the buffer */
    asprintf(&msg, "%10d from the buffer location: %d", buffer[readloc], readloc);
    cdbgout("CONSUMER", BLUE, msg);
    free(msg);

    /* Increment the read pointer */
    readloc = (readloc+1)%BUF_SZ;

    /* Signal the appropiate semaphores */
    sem_post(&mutex);
    sem_post(&empty);    
}

void i_producer() {
    /* Produce new data */
    dataunit newData = rand();

    sem_wait(&empty);
    sem_wait(&mutex);

    /* Add an elements to the buffer */
    slot_t elemToWrite = rand();
    buffer[writeloc] = elemToWrite;

    asprintf(&msg, "%10d to the buffer location: %d", elemToWrite, writeloc);
    cdbgout("PRODUCER", GREEN, msg);
	     
    writeloc = (writeloc+1)%BUF_SZ;
    
    sem_post(&mutex);
    sem_post(&full);
}

/* Wrapper around i_consumer */
void *consumer(void *vargs) {
    while (1) {
	i_consumer();
    }
}

/* Wrapper around i_producer */
void *producer(void *vargs) {
    while (1) {
	sleep(rand()%2);
	i_producer();
    }
}

int main() {
    srand(time(NULL));

    /* Initialize all the semaphores */
    sem_init(&mutex, 0, 1);
    sem_init(&full,  0, 0);
    sem_init(&empty, 0, BUF_SZ);

    /* Create producer and consumer threads */
    pthread_t conid, proid;
    pthread_create(&proid, NULL, producer, NULL);
    pthread_create(&conid, NULL, consumer, NULL); 

    /* Wait for all the threads to exit */
    pthread_exit(NULL); 

}	
