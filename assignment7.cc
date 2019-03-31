#include "common.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const int MAX_TICKS = 10;
const int PHILOSOPHER_COUNT = 5;
const int HUNGER_PROBABILITY_MAX = 100;
const int HUNGER_PROBABILITY = 5; /* Out of HUNGER_PROBABILITY_MAX */
const int EATING_PROBABILITY_MAX = 100;
const int EATING_PROBABILITY = 75; /* Out of EATING_PROBABILITY_MAX */


long int ticks;
long int tick(table_t *table, long int &ticks, stat_t *philosopherStats) {    
    ticks++;
    for (int phil = 0; phil < PHILOSOPHER_COUNT; phil++) {	
	/* Philosophers are hungry if the generated random number is
	   greater than HUNGER_PROBABILITY */
	if (table->philosophers[phil].state == THINKING) {
	    phil_state_t newState = (rand()%HUNGER_PROBABILITY_MAX > HUNGER_PROBABILITY)
		? THINKING : HUNGRY;
	    table->philosophers[phil].state = newState;
	} else if (table->philosophers[phil].state == EATING) {
	    phil_state_t newState = (rand()%EATING_PROBABILITY_MAX > EATING_PROBABILITY)
		? THINKING : EATING;
	    if (newState == THINKING) {
		table->philosophers[phil].state = newState;
		
		int right_cs = phil;
		int left_cs = (phil+PHILOSOPHER_COUNT-1)%PHILOSOPHER_COUNT;
		table->chopsticks[left_cs].state  = AVAILABLE;
		table->chopsticks[right_cs].state = AVAILABLE;
	    }
	}
    }

    /* Manage the philosophers */
    l_init(0, PHILOSOPHER_COUNT);
    int phil = 0;
    do {
	phil=l_next();
	/* Update the stats */
	philosopherStats[phil].type[table->philosophers[phil].state]++;
	
	if (table->philosophers[phil].state == HUNGRY) {
	    int right_phil = phil+1;
	    int left_phil = (phil+PHILOSOPHER_COUNT-1)%PHILOSOPHER_COUNT;
	    
	    int right_cs = phil;
	    int left_cs = (phil+PHILOSOPHER_COUNT-1)%PHILOSOPHER_COUNT;
	    
	    /* Check if the philosopher's chopsticks are available */
	    if ((table->chopsticks[left_cs].state == AVAILABLE)
		&& (table->chopsticks[right_cs].state == AVAILABLE)) {

		/* If the philosopher's neighbours aren't waiting
		   let's change the state to EATING */
		table->philosophers[phil].state = EATING;
		    
		/* printf("%d,%d,%d\n", left_cs, phil, right_cs); */
		table->chopsticks[left_cs].state  = BUSY;
		table->chopsticks[right_cs].state = BUSY;
	    }
	}
    } while (l_hasNext());
}

void print_table(table_t *table, int philosopher_count = PHILOSOPHER_COUNT) {
    for (int i = 0; i < PHILOSOPHER_COUNT; i++) {
	/* if (table->philosophers[i].state == THINKING) { */
	/*     printf("Thinking\n"); */
	/* } else if (table->philosophers[i].state == EATING) { */
	/*     printf("Eating\n"); */
	/* } */

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
	printf("%c", table->chopsticks[i].state == BUSY ? '.' : '|');
    }
    printf("\n");
}

void update_trace(table_t *table, long int &ticks) {
    printf("%04ld: ", ticks);
    print_table(table);
}

void print_stats(stat_t *stats) {
    printf("%4s: %6s %6s %6s\n", "id", "eat", "think", "hungry");
    for (int phil = 0; phil < PHILOSOPHER_COUNT; phil++) {
	printf("%4d: %6ld %6ld %6ld\n", phil, stats[phil].type[0], stats[phil].type[1], stats[phil].type[2]);
    }
}

int main() {
    srand(time(NULL));

    stat_t *philosopherStats;
    philosopherStats = (stat*)malloc(sizeof(stat_t)*PHILOSOPHER_COUNT);
	
    /* Initialize the table */
    table_t *table = (table_t*)malloc(sizeof(table_t));
    
    table->philosophers = (philosopher_t*)malloc(sizeof(philosopher_t)*PHILOSOPHER_COUNT);
    for (int phil = 0; phil < PHILOSOPHER_COUNT; phil++) {
	table->philosophers[phil].id = phil;
	table->philosophers[phil].state = THINKING;
    }
    
    table->chopsticks = (chopstick_t*)malloc(sizeof(chopstick_t)*PHILOSOPHER_COUNT);
    for (int cs = 0; cs < PHILOSOPHER_COUNT; cs++) {
	table->chopsticks[cs].id = cs;
	table->chopsticks[cs].state = AVAILABLE;
    }
    printf("Starting simulation...\n");

    int count = 0;
    while (count++ < MAX_TICKS) {
	update_trace(table, ticks);
	tick(table, ticks, philosopherStats);
    }
    printf("\nStats...\n");
    print_stats(philosopherStats);
}
