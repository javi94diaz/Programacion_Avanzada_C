// Fichero comun de la practica 5

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define H_DISP 0
#define P_DISP 1
#define MUTEX 2
#define CLAVE_SEM 1 // Clave de acceso a los semaforos
#define CLAVE_SHM ((key_t) 1001) // Clave de acceso a la zona de memoria compartida
#define MAX 5

// Estructura de datos que se comparte en la zona de memoria común
typedef struct {
	char vector[MAX];
} buffer;

void pausa(char* quien, char* donde) {
	int pausa;
	pausa = 3000 + (int)(2000*(random()/(float)RAND_MAX)); // entre 3000 y 5000	microsegundos
	printf("%s: Estoy haciendo cosas %s...\n", quien, donde);
	usleep(pausa*1000); // entre 3000 y 5000 milisegundos
}