/**
 * Scrivere un programma che crea N_PROC sottoprocessi.
 * Ogni processo figlio stamperà gli interi nell'intervallo da "inizio" a "fine" ognuno preceduto dal proprio PID.
 * Il processo padre attenderà la fine di tutti i suoi figli e poi uscirà.
 */

#include <ourhdr.h>

#include <pthread.h>

struct thread_d {
	int start;
	int stop;
	int id_thread;
};

#define N_PROC	5

void *conta(void *buff) {
	struct thread_d *data = buff;
	int n1 = data->start;
	int n2 = data->stop;

	for (int i = n1; i < n2; ++i)
		printf("[PID %d] - [Thread %d]: %d\n", getpid(), data->id_thread, i);

	pthread_exit(0);
}

int main () {
	int size = 10;
	pthread_t thread[N_PROC];
	struct thread_d data[N_PROC];

	for (int i = 0; i != N_PROC; ++i) {
		data[i].id_thread = i;
		data[i].start = i * size;
		data[i].stop = (i + 1) * size; 
		pthread_create(&thread[i], NULL, conta, &data[i]);
	}

	for (int i = 0; i != N_PROC; ++i) {
		pthread_join(thread[i], NULL);
	}
}