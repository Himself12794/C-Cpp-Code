#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int max, min, avg, *vals, count;

void* calc_max(void* arg) {
    max = vals[0];
    int i;
    for (i = 0; i < count; ++i) max = max > vals[i] ? max : vals[i];
}

void* calc_min(void* arg) {
    min = vals[0];
    int i;
    for (i = 0; i < count; ++i) min = min < vals[i] ? min : vals[i];
}

void* calc_avg(void* arg) {
    int sum = 0, i;
    for (i = 0; i < count; ++i) sum += vals[i];
    avg = sum / count;
}

int main(int argc, char **argv) {

    // Allocate memory for global array
    vals = malloc( (argc-1) * sizeof(vals[0]) );
    count = argc-1;

    // Store data in global array
    for (size_t i = 0; i < count; i++) vals[i] = atoi(argv[i+1]);

    pthread_t max_thread;
    pthread_t min_thread;
    pthread_t avg_thread;

    // Start all three threads
    pthread_create(&avg_thread, NULL, calc_avg, NULL);
    pthread_create(&min_thread, NULL, calc_min, NULL);
    pthread_create(&max_thread, NULL, calc_max, NULL);

    // Wait for all three threads and output results
    pthread_join(avg_thread, NULL);
    printf("The average value is %d\n", avg);

    pthread_join(min_thread, NULL);
    printf("The minimum value is %d\n", min);

    pthread_join(max_thread, NULL);
    printf("The maximum value is %d\n", max);

    return 0;
}
