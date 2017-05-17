#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CHAIRS 5

int costumersWaiting = 0;
int barbeiroOcupado = 0;

pthread_cond_t loja_cheia = PTHREAD_COND_INITIALIZER;
pthread_cond_t loja_vazia = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 

void *barbeiro(void *args) {
	while(1) {
		pthread_mutex_lock(&mutex);

		barbeiroOcupado = 0;

		if (costumersWaiting == 0) {
			printf("Nenhum cliente esperando, vou dormir!!\n");
			pthread_cond_wait(&loja_vazia, &mutex);
		}

		barbeiroOcupado = 1;
		costumersWaiting--;
		printf("Cortando o cabelo de alguem!\n");

		if (costumersWaiting == 0) {
			pthread_cond_signal(&loja_cheia);
		}

		pthread_mutex_unlock(&mutex);
	}
}


void *cliente(void *args) {
	while(1) {
		pthread_mutex_lock(&mutex);

		if (costumersWaiting != NUM_CHAIRS) {
			printf("Tem uma cadeira livre! Vou esperar!\n");
			costumersWaiting++;

			if (costumersWaiting == 1) {
				pthread_cond_signal(&loja_vazia);
			} 

			if (barbeiroOcupado == 1) {
				pthread_cond_wait(&loja_cheia, &mutex);
			}
				
		} else {
			printf("Ta cheio! Vou embora!\n");			
		}

		pthread_mutex_unlock(&mutex);
		
	}
}


int main() {
	pthread_t barber;
	pthread_t costumer;

	pthread_create(&barber, NULL, barbeiro, NULL);
	pthread_create(&costumer, NULL, cliente, NULL);

	pthread_exit(NULL);

	return 0;
}