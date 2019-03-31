#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

const int BUFFER_SIZE = 10;

enum fun {
  PRODUCER,
  CONSUMER,
  MAX
};

typedef struct item {
  int data;
} item_t;

/* Semphore strucutres */
typedef struct thread {
  struct thread *next; /* Pointer to next thread */
  pthread_t pid;
  int waitFlag;
} thread_t;

thread_t &thread_init(pthread_t pid, thread_t *next) {
  
}

typedef struct semaphore {
  int id;
  int value;
  thread *head; /* Linked list for processes holding on the
		   semaphore */
  thread *tail; /* Points to the last node in the list */
} semaphore_t;

void wait(semaphore_t &sem, pthread_t pid) {
  sem.value--;
  if (s.value < 0) {
    /* Block the thread if this resource is already over it's
       capacity */
    /* Create a new instance of thread_t for this thread */
    
    s.tail.next = 
    s.waitFlag = 1;
    while (s.waitFlag != 0);
  }
  return;
}

void signal(semaphore_t &sem, pthread_t pid) {
  sem.value--;
  if (s.value < 0) {
    /* Block the thread if this resource is already over it's
       capacity */
    s.waitFlag = 1;
    while (s.waitFlag != 0);
  }
  return;
}
  

item buffer[BUFFER_SIZE];

void traceUpdate(fun funType) {
  switch (funType) {
  case PRODUCER:
    printf("[%8s]: \n", "PRODUCER");
    break;
  case CONSUMER:
    printf("[%8s]: \n", "CONSUMER");
    break;
  default:
    printf("Error.\n");
    break;
  }
}

void *producer(void* vargs) {
  pthread_t *myPid = (*pthread_t)(vargs);
  
  sleep(10);
  traceUpdate(PRODUCER);
}

void *consumer(void* vargs) {
  pthread_t *myPid = (*pthread_t)(vargs);
  
  sleep(10);
  traceUpdate(CONSUMER);
}


int main() {
  pthread_t pidProd, pidCons;

  /* Create a producer */
  pthread_create(&pidProd, NULL, producer, (void*)(pidProd));
  
  /* Create a consumer */
  pthread_create(&pidCons, NULL, consumer, (void*)(pidCons));

  pthread_exit(NULL);
  printf("Done\n");
}

