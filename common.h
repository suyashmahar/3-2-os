#ifndef N_3_2_OS_COMMON_H__
#define N_3_2_OS_COMMON_H__

#include <cassert>
#include <cstdio>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <string>
#include <ctime>
#include <unistd.h>

#define RESET    "\033[0m"
#define RED      "\033[1m\033[31m"
#define GREEN    "\033[1m\033[32m"
#define YELLOW   "\033[1m\033[33m"
#define BLUE     "\033[1m\033[34m"
#define MAGENTA  "\033[1m\033[35m"
#define CYAN     "\033[1m\033[36m"
#define WHITE    "\033[1m\033[37m"
#define DARKBLUE "\033[1m\033[40m"

typedef int dataunit;
typedef int slot_t;

void cdbgout(const char *sig, char *color, const char *msg) {
    printf("%s[%12s]%s %s\n", color, sig, RESET, msg);
}

void dbgout(char *sig, char *msg) {
    cdbgout(sig, RESET, msg);
}

typedef enum phil_state {EATING, THINKING, HUNGRY, MAX_PS} phil_state_t;
typedef enum cs_state {BUSY, AVAILABLE, MAX_CS} cs_state_t;
typedef enum smoker {WITH_PAPER, WITH_TOBACCO, WITH_MATCH, SMOKER_MAX} smoker_t;
typedef enum pusher {PUSHER_PAPER, PUSHER_TOBACCO, PUSHER_MATCH, PUSHER_MAX} pusher_t;
typedef enum agent {AGENT_A, AGENT_B, AGENT_C, AGENT_MAX} agent_t;

typedef struct philosopher {
    pthread_t tid;
    int id;
    phil_state_t lastStat;
    phil_state_t state;
    sem_t allowEating;
} philosopher_t;

typedef struct chopstick {
    int id;
    sem_t available;
    cs_state state;
} chopstick_t;

typedef struct table {
    philosopher_t *philosophers;
    chopstick_t *chopsticks;
} table_t;

typedef struct stat {
    long int type[MAX_PS];
} stat_t;

/******************************************************************/
/* Functions which allows random iteration over the elements of an
   array*/
int l_max, l_index, l_offset;
void l_init(int start, int max) {
    l_max = max;
    l_index = start;
    l_offset = rand()%l_max;
}

int l_getCurrentValue() {
    return ((long) l_index * 3 + l_offset) % (l_max);
}

int l_hasNext() {
    return l_index < l_max;
}

int l_next() {
    int answer = l_getCurrentValue();
    l_index++;
    return answer;
}
/******************************************************************/



#endif // N_3_2_OS_COMMON_H__
