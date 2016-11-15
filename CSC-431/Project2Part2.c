#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int max, min, avg, *vals, count;

void calc_max(void) {
    max = vals[0];
    int i;
    for (i = 0; i < count; ++i) max = max > vals[i] ? max : vals[i];

    printf("The maximum value is %d\n", max);
}

void calc_min(void) {
    min = vals[0];
    int i;
    for (i = 0; i < count; ++i) min = min < vals[i] ? min : vals[i];

    printf("The minimum value is %d\n", min);
}

void calc_avg(void) {
    int sum = 0, i;
    for (i = 0; i < count; ++i) sum += vals[i];
    avg = sum / count;

    printf("The average value is %d\n", avg);
}

int main(int argc, char **argv) {

    // Allocate memory for global array
    vals = malloc( (argc-1) * sizeof(vals[0]) );
    count = argc-1;

    // Store data in global array
    for (size_t i = 0; i < count; i++) vals[i] = atoi(argv[i+1]);

    // Fork all three processes
    if (fork() == 0) calc_avg();
    else if (fork() == 0) calc_min();
    else if (fork() == 0) calc_max();

    return 0;
}
