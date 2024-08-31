#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <pthread.h>
#include <stdbool.h>
#include "config.h"

typedef struct
{
    int thread_id;
    mpz_t *N;
    mpz_t *p;
    mpz_t *q;
    bool *found;
    pthread_mutex_t *mutex;
} thread_args_t;

void *fermat_factor(void *arg)
{
    thread_args_t *args = (thread_args_t *)arg;
    int i = args->thread_id;

    mpz_t N, a, b, b2;
    mpz_inits(a, b, b2, NULL);

    mpz_set(N, *(args->N));

    long long int counter = 0;

    // Initialize a = ceil(sqrt(N))
    mpz_sqrt(a, N);
    mpz_add_ui(a, a, 1);
    mpz_add_ui(a, a, i);
#if DEBUG_LEVEL
    if (i == 0)
    {
        gmp_printf("Starting with a = %Zd\n", a);
    }
#endif
    while (1)
    {
        // b2 = a^2 - N
        mpz_pow_ui(b2, a, 2);
        mpz_sub(b2, b2, N);

        if (mpz_perfect_square_p(b2))
        {
            mpz_sqrt(b, b2);
            mpz_sub(*(args->p), a, b); // p = a - b
            mpz_add(*(args->q), a, b); // q = a + b
            pthread_mutex_lock(args->mutex);
            *(args->found) = true;
            pthread_mutex_unlock(args->mutex);
            break;
        }
        if (counter % 100000 == 0)
        {
            pthread_mutex_lock(args->mutex);
            if (*(args->found))
            {
                pthread_mutex_unlock(args->mutex);
                break;
            }
            pthread_mutex_unlock(args->mutex);
#if DEBUG_LEVEL == 2
            if (counter % 100000 == 0)
            {
                printf("Thread %d: Iteración %lld\n", i, counter);
                fflush(stdout);
            }
#endif
        }
        counter++;
        mpz_add_ui(a, a, NUM_THREADS);
    }
    pthread_exit(NULL);
}

int main()
{
    char N_str[] = NUMBER_TO_FACTOR;
    pthread_t threads[NUM_THREADS];
    thread_args_t thread_args[NUM_THREADS];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    bool found = false;
    mpz_t N, p, q;
    mpz_inits(N, p, q, NULL);
    mpz_set_str(N, N_str, 10);
    printf("Initializing %d threads\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_args[i].thread_id = i;
        thread_args[i].found = &found;
        thread_args[i].mutex = &mutex;
        thread_args[i].N = &N;
        thread_args[i].p = &p;
        thread_args[i].q = &q;
        pthread_create(&threads[i], NULL, fermat_factor, (void *)&thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("Threads finished\n");

    gmp_printf("Factors found:\n p = %Zd\n q = %Zd\n", p, q);

    mpz_clears(N, p, q, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
