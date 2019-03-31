#include "common.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Multiple consumers and producers bounded buffers with semaphores */

/* Global constants */
#define BUF_SZ 10
#define PRO_CNT 5
#define CON_CNT 10
#define SIM_LEN 10 // # items produced by producers combined

/* Global varaiables */
int    readloc = 0;
int    writeloc = 0;
slot_t buffer[BUF_SZ];

sem_t full;
sem_t empty;
sem_t mutex;
sem_t simCyclesLeft;
sem_t simComplete;

char *msg;
char *identifier;

pthread_t consumers[CON_CNT];
pthread_t producers[PRO_CNT];

long itemsProduced = 0l;

/* Statistics */
long producerHit[PRO_CNT];
long consumerHit[CON_CNT];

/* Method for handling simulation */
//void exit_sim() {
//    /* Set all semphores to prevent all threads from executing any
//       further */
//    sem_wait(&simComplete);
//}

/* Consumers */
void i_consumer(long tid) {
    sem_wait(&full);
    sem_wait(&mutex);

    /* Update the stats */
    consumerHit[tid]++;
    
    /* Read a single location from the buffer */
    asprintf(&msg, "%10d from the buffer location: %d", buffer[readloc], readloc);
    asprintf(&identifier, "%s%02li", "CONSUMER", tid); 
    cdbgout(identifier, BLUE, msg);
    free(msg);

    /* Increment the read pointer */
    readloc = (readloc+1)%BUF_SZ;

    /* Signal the appropiate semaphores */
    sem_post(&mutex);
    sem_post(&empty);    
}

void i_producer(long tid) {
    sem_wait(&empty);
    sem_wait(&mutex);

    /* Update the stats */
    producerHit[tid]++;
    
    /* Add an elements to the buffer */
    slot_t elemToWrite = rand();
    buffer[writeloc] = elemToWrite;

    asprintf(&msg, "%10d to the buffer location: %d", elemToWrite, writeloc);
    asprintf(&identifier, "%s%02li", "PRODUCER", tid); 
    cdbgout(identifier, GREEN, msg);
    free(msg);
    free(identifier);
    
    writeloc = (writeloc+1)%BUF_SZ;
    
    sem_post(&mutex);
    sem_post(&full);
}

/* Wrapper around i_consumer */
void *consumer(void *vargs) {    
    long tid = (long)(vargs);
    while (1) {
	/* Exit simulation if no cycles are left */
	int ret = sem_trywait(&simCyclesLeft);
	if (ret != 0) {

	    asprintf(&identifier, "%s%02li", "CONSUMER", tid); 
	    cdbgout(identifier, BLUE, "Exiting");
    	    
	    sem_post(&simComplete);
	    return 0;
	} else {
	    i_consumer(tid);
	}
    }
}

/* Wrapper around i_producer */
void *producer(void *vargs) {
    long tid = (long)(vargs);
    while (1) {
	/* Exit simulation if no cycles are left */
	if (sem_trywait(&simCyclesLeft) != 0) {
	    asprintf(&identifier, "%s%02li", "PRODUCER", tid); 
	    cdbgout(identifier, GREEN, "Exiting");

	    sem_post(&simComplete);
	    return 0;
	} else {
	    /* sleep(rand()%2); */
	    i_producer(tid);
	}
    }
}

int main() {
    srand(time(NULL));

    /* Initialize all the semaphores */
    sem_init(&simCyclesLeft, 0, SIM_LEN);
    
    sem_init(&mutex, 0, 1);
    sem_init(&full,  0, 0);
    sem_init(&empty, 0, BUF_SZ);

    /* Create producer and consumer threads */
    for (long prod = 0; prod < PRO_CNT; prod++) {
	pthread_create(&producers[prod], NULL, producer, (void*)prod);
    }

    for (long con = 0; con < CON_CNT; con++) {
	pthread_create(&consumers[con], NULL, consumer, (void*)con); 
    }

    /* Wait for all the threads to exit */
    sem_wait(&simComplete);
    printf(" *** Simulation completed, cancelling child threads ***\n");
    for (int prod = 0; prod < PRO_CNT; prod++) {
	pthread_cancel(producers[prod]);
    }
    for (int con = 0;  con < CON_CNT;  con++) {
	pthread_cancel(consumers[con]);
    }    

    /* Calculate the statistics */
    printf(" *** Printing hits of each producer and consumer ***\n");
    long prodTotal = 0, conTotal = 0;
    printf("Producers:\n");
    for (int prod = 0; prod < PRO_CNT; prod++) {
	printf("%4d: %li\n", prod, producerHit[prod]);
	prodTotal += producerHit[prod];
    }
    
    printf("Consumers:\n");
    for (int con = 0; con < CON_CNT; con++) {
	printf("%4d: %li\n", con, consumerHit[con]);
	conTotal += consumerHit[con];
    }

    printf("Producer total: %li\n", prodTotal);
    printf("Consumer total: %li\n", conTotal);
    

    pthread_exit(NULL); 
    
}	
