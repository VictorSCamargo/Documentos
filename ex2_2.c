#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0


typedef struct {
    int count;
	pthread_mutex_t mtx;		// mutex para proteger a regiao critica
	pthread_cond_t var_cond;	// variavel de condicao para sincronizar as duas threads
} barreira_t;

barreira_t barreira;

int inicializa_barreira(barreira_t* b, int count){
    b->count = count;
}

int espera(barreira_t* b){
	pthread_mutex_lock(&b->mtx);
    
    b->count -= 1;

    if(b->count > 0){
		printf("TRAVOU\n");
        pthread_cond_wait(&b->var_cond, &b->mtx);
    }
    else {
		printf("LIBEROU\n");
		pthread_cond_broadcast(&b->var_cond);
    }

	pthread_mutex_unlock(&b->mtx);
}


void *fazer_algo_1(void *ptr)
{
    printf("f1 fazendo algo\n");

    fflush(0);
    sleep(1);

    printf("f1 parou de fzr algo\n");
	fflush(0);

	espera(&barreira);

	printf("f1 passou barreira\n");
	fflush(0);

    return 0;
}

void *fazer_algo_2(void *ptr)
{
    printf("f2 fazendo algo\n");

    fflush(0);
    sleep(2);
    
    printf("f2 parou de fzr algo\n");
	fflush(0);

	espera(&barreira);

	printf("f2 passou barreira\n");
	fflush(0);

	return 0;
}

void *fazer_algo_3(void *ptr)
{
    printf("f3 fazendo algo\n");

    fflush(0);
    sleep(3);
    
    printf("f3 parou de fzr algo\n");
	fflush(0);

	espera(&barreira);

	printf("f3 passou barreira\n");
	fflush(0);

	return 0;
}


int main() {
   pthread_t t1, t2, t3;

   inicializa_barreira(&barreira, 3);

    pthread_create(&t1, NULL, fazer_algo_1, NULL);
    pthread_create(&t2, NULL, fazer_algo_2, NULL);
	pthread_create(&t3, NULL, fazer_algo_3, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

   return 0;
}