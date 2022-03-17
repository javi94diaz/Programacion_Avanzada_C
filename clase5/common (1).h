/* Datos comunes */
/* Definición de los nombres de los semáforos */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SEM_1 0
#define SEM_2 1
#define CLAVE 1


// función que implementa una pausa de duración casual entre 3 y 5 segundos
void pausa() {
	int pausa;
	printf("%f", random()/(float)RAND_MAX );
	pausa=3000+(int)(2000*(random()/(float)RAND_MAX)); // entre 3000 y 5000	microsegundos
	usleep(pausa*1000); // entre 3000 y 5000 milisegundos
}