#include "common.h"

#include <assert.h>

sem_t paper, tobacco, match;
sem_t paperSem, tobaccoSem, matchSem;
sem_t agentSem;
sem_t mutex;
sem_t simComplete;

bool isPaperAvail = false, isMatchAvail = false, isTobaccoAvail = false;

/* Controls the length of the simulation */
int cyclesLeftInt = 10;

void i_smoker(smoker_t smoker) {

	switch (smoker) {
		case (WITH_PAPER):
			sem_wait(&paperSem);
			cdbgout("SMOKER_P", RED, "smoked");
			break;
		case (WITH_TOBACCO):
			sem_wait(&tobaccoSem);
			cdbgout("SMOKER_T", RED, "smoked");
			break;
		case (WITH_MATCH):
			sem_wait(&matchSem);
			cdbgout("SMOKER_M", RED, "smoked");
			break;
		default:
			assert(false && "Unkown condition reached");
	}
	sleep((unsigned int)(rand()%10));
	sem_post(&agentSem);
}

void i_pusher(pusher_t pusher) {

	switch (pusher) {
		case (PUSHER_TOBACCO): {
			sem_wait(&tobacco);
			sem_wait(&mutex);

			if (isPaperAvail) {
				isPaperAvail = false;
				sem_post(&matchSem);
			} else if (isMatchAvail) {
				isMatchAvail = false;
				sem_post(&paperSem);
			} else {
				isTobaccoAvail = true;
			}

			sem_post(&mutex);
			cdbgout("PUSHER_T", BLUE, "Pushed");
			break;
		}
		case (PUSHER_PAPER): {
			sem_wait(&paper);
			sem_wait(&mutex);

			if (isTobaccoAvail) {
				isTobaccoAvail = false;
				sem_post(&matchSem);
			} else if (isMatchAvail) {
				isMatchAvail = false;
				sem_post(&tobaccoSem);
			} else {
				isPaperAvail = true;
			}

			sem_post(&mutex);
			cdbgout("PUSHER_P", BLUE, "pushed");
			break;
		}
		case (PUSHER_MATCH):
			sem_wait(&match);
			sem_wait(&mutex);

			if (isTobaccoAvail) {
				isTobaccoAvail = false;
				sem_post(&paperSem);
			} else if (isPaperAvail) {
				isPaperAvail = false;
				sem_post(&tobaccoSem);
			} else {
				isMatchAvail = true;
			}

			sem_post(&mutex);
			cdbgout("PUSHER_M", BLUE, "pushed");
			break;
		default: {
			assert(false && "Unkown condition reached");
		}
	}

	sleep(rand()%10);
	return;
}

void i_agent(agent_t agent) {
	switch (agent) {
		case (AGENT_A):
			sem_wait(&agentSem);

			sem_post(&tobacco);
			sem_post(&paper);
			break;
		case(AGENT_B): {
			sem_wait(&agentSem);

			sem_post(&paper);
			sem_post(&match);
			break;
		}
		case(AGENT_C): {
			sem_wait(&agentSem);

			sem_post(&tobacco);
			sem_post(&match);
			break;
		}
		default: {
			assert(false && "Unkown state reached");
		}
	}
}

void *agent(void *vargs) {
	while(true) {
		agent_t agent = (agent_t)(long)(vargs);

		i_agent(agent);

		char *sig, *msg;
		asprintf(&sig, "AGENT", agent);
		asprintf(&msg, "%s executed",
				 agent == AGENT_A ? "Agent A" : agent == AGENT_B ? "Agent B" : "Agent C");
		cdbgout(sig, GREEN, msg);

		/* Only one of the agent reaches this line at a time, this avoids interleaving of instructions*/
		if (cyclesLeftInt-- == 0) {
			sem_post(&simComplete);
			break;
		}
	}
	return nullptr;
}

void *smoker(void *vargs) {
	while (cyclesLeftInt) {
		smoker_t smoker = (smoker_t)(long)(vargs);
		i_smoker(smoker);
	}
	return nullptr;
}

void *pusher(void *vargs) {
	while (cyclesLeftInt) {
		pusher_t pusher  = (pusher_t)(long)(vargs);
		i_pusher(pusher);
	}
	return nullptr;
}

int main() {
	sem_init(&simComplete, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&agentSem, 0, 1);
	sem_init(&paper, 0, 0);
	sem_init(&tobacco, 0, 0);
	sem_init(&match, 0, 0);
	sem_init(&paperSem, 0, 0);
	sem_init(&tobaccoSem, 0, 0);
	sem_init(&matchSem, 0, 0);

	/* Create all the agents */
	pthread_t agentId[AGENT_MAX];
	for (int i = 0; i < AGENT_MAX; i++) {
		pthread_create(&agentId[i], NULL, agent, (void*)(long)i);
	}

	/* Create all the pushers */
	pthread_t pusherId[PUSHER_MAX];
	for (int i = 0; i < PUSHER_MAX; i++) {
		pthread_create(&pusherId[i], NULL, pusher, (void*)(long)i);
	}

	/* Create all the smokers */
	pthread_t smokerId[SMOKER_MAX];
	for (int i = 0; i < SMOKER_MAX; i++) {
		pthread_create(&smokerId[i], NULL, smoker, (void*)(long)i);
	}

	sem_wait(&simComplete);
	cdbgout("SIMULATION", GREEN, "Simulation completed!");

	return 0;
}
