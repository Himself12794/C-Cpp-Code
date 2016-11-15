#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int max_customers = 4;
unsigned char joke_mask = 0x00;
volatile int customers = 0;
sem_t mutex, customer, barber, customer_done, barber_done, joke_mutex;

char *jokes[8] = {
    "What do you call a cow that can't give milk?",
    "What's long brown and sticky?",
    "What's blue and smells like red paint?",
    "So a horse walks into a barbershop.",
    "How do you keep a blonde busy?",
    "What do you call 500 lawyers at the bottom of the ocean?",
    "Why do French eat snails so much?",
    "How do you confuse a blonde?"};

char *punchlines[8] = {
    "An udder failure.",
    "A stick.",
    "Blue paint.",
    "And the barber says, why the long face?",
    "Put her in a circular room and tell her to stand in the corner.",
    "A good start.",
    "They can't stand fast food.",
    "Paint yourself green and throw forks at her."};

void balk();
void get_hair_cut();
int get_rand_joke();
void cut_hair();
void* barber_thread(void*);
void* customer_thread(void*);

int main(int argc, char const *argv[]) {
    //pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
    sem_init(&mutex, 0, 1);
    sem_init(&joke_mutex, 0, 1);
    sem_init(&customer, 0, 0);
    sem_init(&barber, 0, 0);
    sem_init(&customer_done, 0, 0);
    sem_init(&barber_done, 0, 0);

    if (argc >= 2) {
        max_customers = 4;
    }

    char *strs[7] = {"Bob", "Fred", "George", "Jim", "John", "Frank", "Philip"};
    pthread_t custs[7], t_barber;

    printf("%s\n", "Barber: Time to open shop");
    pthread_create(&t_barber, 0, barber_thread, NULL);

    for (int i = 0; i < 7; i++) {
        printf("In walks %s\n", strs[i]);
        pthread_create(&(custs[i]), 0, customer_thread, strs[i]);
        sleep(1);
    }

    for (int i = 0; i < 7; i++) {
        pthread_join(custs[i], NULL);
    }

    pthread_join(t_barber, NULL);
    return 0;
}

void *customer_thread(void *arg) {
    char *name = (char *)arg;
    sem_wait(&mutex);
    if (customers == max_customers) {
        sem_post(&mutex);
        balk(name);
    } else {
        printf("%s: %s\n", name, "There's room, I'll wait");
        sleep(1);
        customers++;
        sem_post(&mutex);

        sem_post(&customer);
        sem_wait(&barber);
        get_hair_cut(name);
        sem_post(&customer_done);
        sem_wait(&barber_done);

        sem_wait(&mutex);
        customers--;
        sem_post(&mutex);
    }
    printf("%s leaves\n", name);
}

void get_hair_cut(char *name) {
    int j = get_rand_joke();
    printf("\n");
    printf("%s: %s\n", name, jokes[j]);
    sleep(1);
    printf("%s: %s\n", name, punchlines[j]);
    printf("\n");
}

void balk(char *name) {
    printf("%s: %s\n", name, "There's no room, I'll leave");
}

void *barber_thread(void *arg) {
    for (;;) {

        sem_wait(&customer);
        printf("%s\n", "Barber: Next customer!");
        printf("%s\n", "Barber: Good morning, sir");
        sem_post(&barber);

        cut_hair();

        sem_wait(&customer_done);
        printf("%s\n", "Barber: Very funny sir!");
        sem_post(&barber_done);
        printf("%s\n", "Barber: You're done!");
    }

}

void cut_hair() {
    printf("%s\n", "Barber: *snip* *snip*");
    sleep(5);
}

long random_at_most(long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

int get_rand_joke() {
    sem_wait(&joke_mutex);
    if (joke_mask == 0xFF) {
        joke_mask = 0x00;
    }
    int r;
    srand(time(NULL));
    while ( ( joke_mask >> (r = rand() % 8) ) & 0x01 );

    joke_mask = joke_mask | (0x01 << r);
    sem_post(&joke_mutex);

    return r;
}
