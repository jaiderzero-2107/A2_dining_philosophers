#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <pthread.h>

typedef enum {
    THINKING,
    HUNGRY,
    EATING
} philosopher_state;

// Philosophers struct
typedef struct {
    int id;
    int meals_to_eat;
} philosopher_info;

extern pthread_mutex_t waiter_lock;
extern pthread_cond_t can_eat_cond;
extern philosopher_state states[];

void *philosopher_lifecycle(void *arg);

#endif //PHILOSOPHER_H