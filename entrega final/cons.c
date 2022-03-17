/*
Practica 5 - Javier Diaz Mena - 12100

Escribir dos programas que implementen una variante del problema del productor/consumidor (conocido
también como “bounded-buffer problem”). En este problema, hay dos proceso que comparten un
buffer de tamaño limitado. Uno de ellos, el productor, usa el buffer para colocar unos items, y el
segundo (el cosumidor) coge estos items del buffer. El productor puede colocar un items en el
buffer solo si hay un hueco libre, si no tiene que esperar hasta que haya (al menos) uno. El
consumidor recoge un ítems en el buffer solo si hay (al menos) uno, si no tiene que esperar hasta
que haya uno. Para implementar correctamente el funcionamiento de este sistema, se usarán dos
semáforos, uno para señalar cuando hay elementos disponibles en el buffer y uno para señalar
cuando hay huecos libres en el buffer.
La variante por implementar consiste en tener un productor y dos consumidores. Los dos
consumidores serán dos instancias del mismo proceso.
Adicionalmente, se usará un tercer semáforo para mutua exclusión para evitar que los dos
procesos accedan al buffer a la vez.
*/

/* INSTRUCCIONES: Ejecutar primero el productor en una terminal y dejar que vaya llenando el buffer común. Despues
ejecutar dos consumidores en otras dos terminales independientes y observar la evolucion del buffer. */

/* CONSUMIDOR */

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"


void cogerProducto(buffer* cinta){

	// Cojo producto de la primera componente, si lo hay ahí
	
	//if ( cinta->vector[0] != 'a') {
		char prod_out = cinta->vector[0];
		printf("*************Cojo el producto %c\n", prod_out);
		//cinta->vector[0] = 'a'; // Insertamos un hueco en la primera componente
	//}
	
	// Trasladar todo el vector una componente hacia arriba si hay algun hueco
	for (int i=0; i<MAX; i++){
		cinta->vector[i] = cinta->vector[i+1];
		
		if(i == MAX-1){
			cinta->vector[i] = 'a'; // Insertamos un hueco en la ultima tras mover la cinta
		}
	}
	
	
	// Mostramos la cinta
	printf("CINTA: ");
	for (int i=0; i<MAX; i++){
		printf("%c ", cinta->vector[i]);
	}
	printf("\n");
	
}

int main (void){
	
	// Variables para memoria compartida
	int shmid;
	buffer *cinta;
	
	// Variables para los semaforos
	int semid;
	struct sembuf operacion;
	
	// Creación de la memoria compartida que representa el buffer limitado comun entre procesos
	shmid = shmget(CLAVE_SHM, sizeof(buffer), IPC_CREAT|0666);
	if(shmid== -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	
	// Fase de Attach
	cinta = (buffer*) shmat(shmid,0,0);

	// Creacion de los semaforos
	semid = semget(CLAVE_SEM,3,IPC_CREAT|0600);
	if(semid == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	
	// Consumidor no inicializa los semaforos
	printf("Valor semáforo huecos: %d\n", semctl(semid,H_DISP,GETVAL,0));
	printf("Valor semáforo productos: %d\n", semctl(semid,P_DISP,GETVAL,0));
	printf("Valor semáforo mutex: %d\n", semctl(semid,MUTEX,GETVAL,0));
	
	int i=10; // Numero de iteraciones del programa
	
	while(i>0){
		
		i--;
		printf("\nIteracion: %d\n", i);
		
		// Consultar Mutex
		printf("Consumidor: WAIT mutex\n");
		operacion.sem_num = MUTEX;
		operacion.sem_op = -1;
		semop(semid, &operacion, 1);
		
		// Espera a que haya algun producto
		printf("Consumidor: WAIT productos\n");
		operacion.sem_num = P_DISP;
		operacion.sem_op = -1;
		semop(semid, &operacion, 1);
		
		// Si hay algun producto, lo recoge
		if( semctl(semid,H_DISP,GETVAL,0) > 0  &&  semctl(semid,H_DISP,GETVAL,0) < 5 ){
			cogerProducto(cinta);
			printf("Consumidor: Ahora hay %d productos\n", semctl(semid,P_DISP,GETVAL,0));
			pausa("Consumidor", "dentro");
		}
		
		// Tras coger un producto, habra un hueco nuevo
		printf("Consumidor: SIGNAL huecos\n");
		operacion.sem_num = H_DISP;
		operacion.sem_op = 1;
		semop(semid,&operacion,1);
		
		// Liberar mutex
		printf("Consumidor: SIGNAL mutex\n");
		operacion.sem_num = MUTEX;
		operacion.sem_op = 1;
		semop(semid,&operacion,1);
		
		pausa("Consumidor", "fuera");
		
	}


}










