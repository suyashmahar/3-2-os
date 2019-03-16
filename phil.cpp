#include "common.h"

#include <semaphore.h>
#include <deque>
#include <sstream>

/* Global constants */
const int MAX_TICKS = 10;
const int PHILOSOPHER_COUNT = 5;
const int HUNGER_PROBABILITY_MAX = 100;
const int HUNGER_PROBABILITY = 5; /* Out of HUNGER_PROBABILITY_MAX */
const int EATING_PROBABILITY_MAX = 100;
const int EATING_PROBABILITY = 75; /* Out of EATING_PROBABILITY_MAX */

/* Global Variables */
philosopher_t philosophers[PHILOSOPHER_COUNT];
chopstick_t   chopsticks  [PHILOSOPHER_COUNT];

sem_t cs_mutex;
sem_t simComplete;
sem_t cyclesLeft;
sem_t print;
sem_t qmutex;

std::deque<philosopher_t> hungryq;

void print_table(table_t *table, int philosopher_count = PHILOSOPHER_COUNT) {
	sem_wait(&print);
	for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
		switch (table->philosophers[i].state) {
			case THINKING:{
				printf("T");
				break;
			}
			case HUNGRY: {
				printf("H");
				break;
			}
			case EATING: {
				printf("E");
				break;
			}
		}
		int cs_avail;
		sem_getvalue(&table->chopsticks[i].available, &cs_avail);
		cs_state_t state = cs_avail == 1 ? AVAILABLE : BUSY;
		printf("%c", state == BUSY ? '.' : '|');
	}
	printf(" -> %d", hungryq.size());
	printf("\n");
	sem_post(&print);
}

void i_philosopher(long phil) {
	/* Philosophers are hungry if the generated random number is
       greater than HUNGER_PROBABILITY */
	if (philosophers[phil].state == THINKING) {
		phil_state_t newState = (rand()%HUNGER_PROBABILITY_MAX > HUNGER_PROBABILITY)
								? THINKING : HUNGRY;
		if (newState == HUNGRY) {
			/* Add the current philospher to the hungry queue */

			sem_wait(&qmutex);
			hungryq.push_back(philosophers[phil]);
			sem_post(&qmutex);

			/* Wait for the signal to allow eating */
			sem_wait(&philosophers[phil].allowEating);

			/* Get the lock on the chopsticks */
			int right_cs = phil;
			int left_cs = (phil+PHILOSOPHER_COUNT-1)%PHILOSOPHER_COUNT;
			sem_wait(&chopsticks[left_cs].available);
			sem_wait(&chopsticks[right_cs].available);

			/* Change the state of the philosopher to eating */
			philosophers[phil].state = newState;

			/* Print trace */
			table_t table;
			table.philosophers = philosophers;
			table.chopsticks = chopsticks;
			print_table(&table, PHILOSOPHER_COUNT);

			sleep(rand()%10);
			philosophers[phil].state = THINKING;

			/* Make the chopsticks available again */
			sem_post(&chopsticks[left_cs].available);
			sem_post(&chopsticks[right_cs].available);
		}
	}
}

void *philosopher(void *vargs) {
	long phil = (long)(vargs);
	while (true) {
		sleep(rand()%2);
		i_philosopher(phil);
		sem_post(&cyclesLeft);
	}
}

int main() {
	/* Initialize all the philosophers and chopsticks */
	sem_init(&simComplete, 0, 0);
	sem_init(&cyclesLeft, 0, 1000);
	sem_init(&print, 0, 1);
	sem_init(&qmutex, 0, 1);

	for (long phil = 0; phil < PHILOSOPHER_COUNT; phil++){
		pthread_create(&philosophers[phil].tid, NULL, philosopher, (void*)phil);
		philosophers[phil].id = phil;
		philosophers[phil].lastStat = EATING;
		philosophers[phil].state = THINKING;
		sem_init(&philosophers[phil].allowEating, 0, 0);
	}

	for (int cs = 0; cs < PHILOSOPHER_COUNT; cs++){
		chopsticks[cs].id = cs;
		sem_init(&chopsticks[cs].available, 0, 1);
	}

	/* Try to empty queue when the simulation is running */
	while (sem_trywait(&simComplete)) {
		/* Iterate over all the elements in the queue */
		std::deque<philosopher_t>::iterator it;
		for (it = hungryq.begin(); it != hungryq.end(); /*No increment here*/) {
			long phil = it[0].id;
			int right_cs = phil;
			int left_cs = (phil+PHILOSOPHER_COUNT-1)%PHILOSOPHER_COUNT;

			int l_cs_availability, r_cs_availability;
			sem_getvalue(&chopsticks[left_cs].available, &l_cs_availability);
			sem_getvalue(&chopsticks[right_cs].available, &r_cs_availability);

			if (l_cs_availability & r_cs_availability) {
				sem_post(&philosophers[phil].allowEating);
				sem_wait(&qmutex);
				hungryq.erase(it); /* Allows the philosopher to be asynchronously removed from the hungry queue */
				sem_post(&qmutex);
			} else {
				it++;
			}
		}
	}

	return 0;
}
