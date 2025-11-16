#include "philosopher.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_PHILOSOPHERS 5

// Checks if a philosopher CAN eat (if their neighbors are not eating)
void test_can_eat(int id) {
    int left = (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    if (states[id] == HUNGRY && states[left] != EATING && states[right] != EATING) {
        states[id] = EATING;
        printf(">>> Philosopher %d has started EATING! <<<\n", id + 1);
    }
}

// Philosopher tries to pick up the chopsticks (asks the waiter for permission)
void pickup_chopsticks(int id) {
    pthread_mutex_lock(&waiter_lock);

    states[id] = HUNGRY;
    printf("Philosopher %d is hungry.\n", id + 1);
    
    // Tries to eat. If they can't, they will wait.
    test_can_eat(id);
    while (states[id] != EATING) {
        printf("Philosopher %d is waiting for chopsticks...\n", id + 1);
        pthread_cond_wait(&can_eat_cond, &waiter_lock);
    }

    pthread_mutex_unlock(&waiter_lock);
}

// Philosopher returns the chopsticks (notifies the waiter)
void return_chopsticks(int id) {
    pthread_mutex_lock(&waiter_lock);

    states[id] = THINKING;
    printf("Philosopher %d finished eating.\n\n", id + 1);

    // Let the neighbors know they can try to eat now
    int left = (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
    int right = (id + 1) % NUM_PHILOSOPHERS;
    test_can_eat(left);
    test_can_eat(right);

    pthread_cond_broadcast(&can_eat_cond);

    pthread_mutex_unlock(&waiter_lock);
}

// The philosopher's lifecycle
void *philosopher_lifecycle(void *arg) {
    philosopher_info *info = (philosopher_info *)arg;

    for (int i = 0; i < info->meals_to_eat; i++) {
        // 1. THINKING
        printf("Philosopher %d is thinking.\n", info->id + 1);
        usleep(100000 + (rand() % 500000));

        // 2. TRYING TO EAT
        pickup_chopsticks(info->id);

        // 3. EATING
        usleep(100000 + (rand() % 300000));

        // 4. RETURNING THE CHOPSTICKS
        return_chopsticks(info->id);
    }

    printf("--- Philosopher %d is full and has left the table. ---\n", info->id + 1);
    return NULL;
}