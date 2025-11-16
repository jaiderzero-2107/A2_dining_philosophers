#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "philosopher.h"

#define NUM_PHILOSOPHERS 5
#define MEALS_PER_PHILOSOPHER 5

pthread_mutex_t waiter_lock;         // The "waiter" mutex that protects the state of the chopsticks
pthread_cond_t can_eat_cond;          // The "waiting room" for hungry philosophers
philosopher_state states[NUM_PHILOSOPHERS]; // State of each philosopher (thinking, hungry, eating)
int chopsticks[NUM_PHILOSOPHERS];    // Representation of chopsticks (0 = free, 1 = in use)

int main() {
    pthread_t philosopher_threads[NUM_PHILOSOPHERS];
    philosopher_info info[NUM_PHILOSOPHERS];

    srand(time(NULL));

    pthread_mutex_init(&waiter_lock, NULL);
    pthread_cond_init(&can_eat_cond, NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        states[i] = THINKING;
        chopsticks[i] = 0; // All chopsticks start as free
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        info[i].id = i;
        info[i].meals_to_eat = MEALS_PER_PHILOSOPHER;
        // The waiter manages the chopsticks
        pthread_create(&philosopher_threads[i], NULL, philosopher_lifecycle, &info[i]);
    }

    printf("Simulation has started.\n\n");

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    printf("\nAll philosophers are full. Simulation is over.\n");

    pthread_mutex_destroy(&waiter_lock);
    pthread_cond_destroy(&can_eat_cond);

    return 0;
}