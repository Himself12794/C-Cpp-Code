#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int max_customers = 4;
volatile int customers = 0;
sem_t mutex, customer, barber, customer_done, barber_done;
pthread_t t_barber;

void balk();
void get_hair_cut();
void cut_hair();
void* barber_thread(void*);
void* customer_thread(void*);

int main(int argc, char const *argv[]) {
    //pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
    sem_init(&mutex, 0, 1);
    sem_init(&customer, 0, 0);
    sem_init(&customer, 0, 0);
    sem_init(&customer, 0, 0);
    sem_init(&customer, 0, 0);

    if (argc >= 2) {
        max_customers = 4;
    }

    printf("Customers can now enter. Enter a customer name and hit enter\n");
    pthread_create(&t_barber, 0, barber_thread, NULL);

    for (;;) {
        char str[100];
        scanf("%s", str);
        printf("In walks %s\n", str);
        pthread_t temp;
        pthread_create(&temp, 0, customer_thread, str);
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
    }
    printf("%s: %s\n", name, "There's room, I'll wait");
    sleep(1);
    customers++;
    sem_post(&mutex);

    sem_post(&customer);
    sem_wait(&barber);
    get_hair_cut(name);
    sem_post(&customer_done);
    sem_wait(&barber_done);
    printf("%s: %s\n", name, "Thank you sir, have a nice day!");

    sem_wait(&mutex);
    customers--;
    sem_post(&mutex);

}

void get_hair_cut(char *name) {
    printf("%s: %s\n", name, "Wonderful day, isn't it?");
}

void balk(char *name) {
    printf("%s: %s\n", name, "There's no room, I'll leave");
    exit(0);
}

void *barber_thread(void *arg) {
    printf("%s\n", "Barber: Time to open shop");
    for (;;) {
        sem_wait(&customer);
        printf("%s\n", "Barber: Good morning, sir");
        sem_post(&barber);

        cut_hair();

        sem_wait(&customer_done);
        printf("%s\n", "Barber: wait customer done");
        printf("%s\n", "Barber: Ok, you're done!");
        sem_post(&barber_done);
        printf("%s\n", "Barber: Post done");
        printf("%s\n", "Barber: Next customer!");
    }

}

void cut_hair() {
    printf("%s\n", "Barber: *snip* *snip*");
    sleep(5);
}
