//Segundo programa de la practica 4 - se llama padre por las anteriores, es quien intenta adivinar el numero

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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "common4.h" // Fichero que contiene la información común (clave y estructura de datos)

int main(void) {

	int shmid;
	datos *zona_comun;
	int res;
	int numIntentos = 0;
	
	// Fase de reserva de memoria
	shmid = shmget(CLAVE_SHM, sizeof(datos), IPC_CREAT|0666);
	if(shmid== -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	// Fase de Attach
	zona_comun = (datos*) shmat(shmid,0,0);

	// Bucle de intentos de adivinar el numero
	while(zona_comun->respuesta != 3) {
		
		while( zona_comun->turno == 2){ // Busy waiting
			
			switch (zona_comun->respuesta){
			case 0:
				zona_comun->intento = 128;
				printf("Padre: Intento inicial con %d\n", zona_comun->intento);
				numIntentos = 1;
				break;
			case 1: //si el numero es mayor q el secreto
				numIntentos++;
				zona_comun->intento--;
				printf("Padre: Intento uno menor, el %d\n", zona_comun->intento);
				break;
			case 2: // si el numero es menor q el secreto
				numIntentos++;
				zona_comun->intento++;
				printf("Padre: Intento uno mayor, el %d\n", zona_comun->intento);
				break;
			default:
				break;
			}
			zona_comun->turno=1;
		}
	}
	// Al acertar el numero
	printf("He adivinado el numero en %d intentos\n", numIntentos);
	
	// Borrado de la zona de memoria compartida
		shmctl(shmid, IPC_RMID,0);
		if(res==-1) {
			perror("shmctl");
			exit(EXIT_FAILURE);
		}
	exit(EXIT_SUCCESS);
}





