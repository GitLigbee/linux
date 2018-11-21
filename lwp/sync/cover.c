#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define LOOP 50000
int couter = 0;

void* doit(void *);

int main(void)
{
    pthread_t tida, tidb;
    pthread_create(&tida, NULL, &doit, NULL);
    pthread_create(&tidb, NULL, &doit, NULL);

    pthread_join(tida, NULL);
    pthread_join(tidb, NULL);
    printf("counter: %d\n", counter);
    return 0;
}

void* doit(void* argv)
{
    int i, val;

    for(i=0; i< LOOP; i++) {
        val = couter;
        couter = val + 1;
    }
    printf("%x: %d\n", (unsigned int)pthread_self(), val);
    return NULL;
}