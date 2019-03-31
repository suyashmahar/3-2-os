#include "common.h"

const int CHAIRS = 5;

sem_t waitingCust;
sem_t barberSem;
sem_t mutex;

int cyclesLeftInt = 10;

void i_barber() {
    while (cyclesLeftInt) {
        sem_wait(&waitingCust);

        sleep(rand()%10);

        char *msg;
        int waitingCustCount;
        sem_getvalue(&waitingCust, &waitingCustCount);
        asprintf(&msg, "Gave a haircut, q.length=%d", waitingCustCount);
        cdbgout("BARBER", BLUE, msg);
    }
}
void i_cust() {
    int waitingCustCount;
    sem_getvalue(&waitingCust, &waitingCustCount);

    if (waitingCustCount < CHAIRS) {
        /* Wait only if chairs are available */
        sem_post(&waitingCust);
        cdbgout("CUSTOMER", GREEN, "Entered the shop!");
    }
}

void *cust(void *vargs) {
    i_cust();
    return nullptr;
}

void *barber(void *vargs) {
    i_barber();
    return nullptr;
}

int main() {
    pthread_t tid;

    sem_init(&waitingCust, 0, 0); /* 0 initial waiting customers */
    sem_init(&mutex, 0, 1);
    sem_init(&barberSem, 0, 1); /* 1 Barber available */

    pthread_create(&tid, NULL, &barber, NULL);
    while (cyclesLeftInt-- > 0) {
        /* Create new customers */
        pthread_create(&tid, NULL, &cust, NULL);
        sleep(rand()%10);
    }
}
