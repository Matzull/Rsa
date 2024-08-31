#include <stdio.h>
#include <math.h>
#include <gmp.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 16
#define DEBUG_LEVEL 1
typedef struct {
    int thread_id;
    mpz_t* N;
    mpz_t* p;
    mpz_t* q;
    bool* found;
    pthread_mutex_t* mutex;
} thread_args_t;

void* fermat_factor(void* arg) {
    thread_args_t *args = (thread_args_t*) arg;
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
    while (1) {
        // b2 = a^2 - N
        mpz_pow_ui(b2, a, 2);
        mpz_sub(b2, b2, N);
        
        if (mpz_perfect_square_p(b2)) {
            mpz_sqrt(b, b2);
            mpz_sub(*(args->p), a, b);  // p = a - b
            mpz_add(*(args->q), a, b);  // q = a + b
            printf("Counter: %lld\n", counter);
            pthread_mutex_lock(args->mutex);
            *(args->found) = true;
            pthread_mutex_unlock(args->mutex);
            break;
        }
        if(counter % 100000 == 0){
            pthread_mutex_lock(args->mutex);
            if (*(args->found)) {
                pthread_mutex_unlock(args->mutex);
                break;
            }
            pthread_mutex_unlock(args->mutex);
            #if DEBUG_LEVEL == 2
            if (counter % 100000 == 0){
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

int main() {
    // The following number is the product of two large primes p and q, 
    // as demonstrated in the computerphile video https://www.youtube.com/watch?v=-ShwJqAalOk
    // the factors p and q are not sufficiently diferent to be secure for RSA encryption
    char N_str[] = "5261933844650100908430030083398098838688018147149529533465444719385566864"\
                   "6057815764873053567170748825058827015852977657893237262583560356927698974"\
                   "2062085877476369411763440802891827039485240416907267155109632123843099381"\
                   "1080749636806153881798472848720411673994908247486124703888115308603904735"\
                   "9594570579252255031976258206705220504941967031540863160621237879347775205"\
                   "9989474514726032706017433610165829502227501305181632161704692732100632275"\
                   "2178354002696596328204277122466231388232487691224076847557856202947748540"\
                   "2637917671281959271795882387994709876695581194225524705059568582176549046"\
                   "28177286026365989987106877656917";
    // char N_str[] = "1975672302399635500389830489";

    pthread_t threads[NUM_THREADS];
    thread_args_t thread_args[NUM_THREADS];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    bool found = false;
    mpz_t N, p, q;
    mpz_inits(N, p, q, NULL);
    mpz_set_str(N, N_str, 10);
    printf("Initializing %d threads\n", NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i].thread_id = i;
        thread_args[i].found = &found;
        thread_args[i].mutex = &mutex;
        thread_args[i].N = &N;
        thread_args[i].p = &p;
        thread_args[i].q = &q;
        pthread_create(&threads[i], NULL, fermat_factor, (void*)&thread_args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Threads finished\n");
    
    gmp_printf("Factors found:\n p = %Zd\n q = %Zd\n", p, q);

    mpz_clears(N, p, q, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
