//Primer programa de la practica 4 - Se llama hijo por las anteriores, es quien piensa el numero secreto

/*
- Practica 4
Basándose en los programas de ejemplo proporcionados, y en las practicas anteriores, escribir
otros dos para que se cree una zona de memoria compartida para comunicación entre dos procesos.
El proceso 1 deberá adivinar un numero elegido por el proceso 2 (entre 0 y 100). Para ello, escribirá
sus intentos a través de la zona de memoria compartida. El proceso 2 contestará escribiendo en la
zona de memoria compartida si el numero es mayor, menor o si ha acertado. En caso de acierto
los proceso terminan. Para alternar los accesos, su usará la técnica de la espera activa (“busy
waiting”). Para ello, se usará una variable “turno” en la memoria compartida.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common4.h" // Fichero que contiene la información común (clave y estructura de datos)

int main(void) {
	
	int shmid;
	datos *zona_comun;
	int numSecreto;

	// Fase de reserva de memoria
	shmid = shmget(CLAVE_SHM, sizeof(datos), IPC_CREAT|0666);
	if(shmid== -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	// Fase de Attach
	zona_comun = (datos*) shmat(shmid,0,0);
	
	//Inicializacion de las variables
	zona_comun->turno = 2;
	zona_comun->respuesta = 0;
	zona_comun->intento = 128;
	
	// Inicializar la semilla para los numeros
	srandom(getpid()); 
	
	// Numero al azar entre 0 y 256
	numSecreto = (int)(256.0*random()/RAND_MAX);
	printf("Hijo: El numero secreto es el %d\n", numSecreto);
	
	// Bucle de respuestas a los intentos
	while(zona_comun->respuesta != 3 ) {
		
		while( zona_comun->turno == 1) { //Busy waiting
			
			if (zona_comun->intento > numSecreto){
				printf ("Hijo: Incorrecto, el %d es mayor que el secreto\n", zona_comun->intento);
				zona_comun->respuesta = 1;
			} else if (zona_comun->intento < numSecreto){
				printf( "Hijo: Incorrecto, el %d es menor que el secreto\n", zona_comun->intento);
				zona_comun->respuesta = 2;
			} else {
				printf("Hijo: Correcto! Has adivinado el numero secreto: el %d\n", numSecreto);
				zona_comun->respuesta = 3;
			}
			zona_comun->turno = 2;
		}
	}
	exit(EXIT_SUCCESS);
}









