/*
Basándose en los programas de ejemplo proporcionados, y en la practicas anteriores, escribir dos
programas que implementen una variante del problema del productor/consumidor (conocido
también como “bounded-buffer problem”). En este problema, hay dos proceso que comparten un
buffer de tamaño limitado. Uno de ellos, el productor, usa el buffer para colocar unos ítems, y el
segundo (el cosumidor) coge estos ítems del buffer. El productor puede colocar un ítems en el
buffer solo si hay un hueco libre, si no tiene que esperar hasta que haya (al menos) uno. El
consumidor recoge un ítems en el buffer solo si hay (al menos) uno, si no tiene que esperar hasta
que haya uno. Para implementar correctamente el funcionamiento de este sistema, se usarán dos
semáforos, uno para señalar cuando hay elementos disponibles en el buffer y uno para señalar
cuando hay huecos libres en el buffer.
La variante por implementar consiste en tener un productor y DOS consumidores. Los dos
consumidores serán dos instancias del mismo proceso.
Adicionalmente, se usará un tercer semáforo para mutua exclusión para evitar que los dos
procesos accedan al buffer a la vez.
*/

// INSTRUCCIONES: Ejecutar primero el productor (prod5) y dejar que vaya llenando el buffer común. Despues
// ejecutar dos consumidores (cons5) en dos terminales independientes y observar la evolucion del buffer.

/* PRODUCTOR */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "common5.h"


void dejarProducto(buffer* cinta){

	// Genera un producto (caracter aleatorio entre A y Z)
	int inf = 65;
	int sup = 90;
	char prod_in = rand ()%(sup-inf+1) + inf;
	
	printf("**************Introduzco el producto %c\n", prod_in);
	cinta->vector[MAX-1] = prod_in;
	
	/*
	// Trasladar todo el vector una componente hacia arriba si hay algun hueco
	for (int i=0; i<MAX; i++){
		cinta->vector[i] = cinta->vector[i+1];
		
		if(i == MAX-1 && huecos!=0){
			cinta->vector[i] = 'a'; // Insertamos un hueco en la ultima tras mover la cinta
		}
	}
	*/
	
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
	int res;
	buffer *cinta;
	
	// Variables para los semaforos
	int semid;
	struct sembuf operacion;
	
	srandom(getpid());
	
	// Creacion de la memoria compartida que representa el buffer comun
	shmid = shmget(CLAVE_SHM, sizeof(buffer), IPC_CREAT|0666);
	if(shmid== -1) {
		perror("shmget AQUI FALLA");
		exit(EXIT_FAILURE);
	}
	// Fase de Attach
	cinta = (buffer*) shmat(shmid,0,0);
	
	// Rellenamos la cinta inicialmente
	for (int i=0; i<MAX; i++){
		cinta->vector[i] = 'a';
	}
	
	// Creacion de los semaforos
	semid = semget(CLAVE_SEM,3,IPC_CREAT|0600);
	if(semid == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	
	// Inicialización de los semaforos
	semctl(semid, H_DISP, SETVAL, 5); // Huecos disponibles
	semctl(semid, P_DISP, SETVAL, 0); // Productos disponibles
	semctl(semid, MUTEX, SETVAL, 1);  // Mutex para los consumidores
	printf("Valor inicial huecos: %d\n", semctl(semid,H_DISP,GETVAL,0));
	printf("Valor inicial productos: %d\n", semctl(semid,P_DISP,GETVAL,0));
	printf("Valor inicial mutex: %d\n", semctl(semid,MUTEX,GETVAL,0));
	operacion.sem_flg=0; // No activamos ninguna opcion
	
	int i=10; //Numero de iteraciones
	
	while(i>0){
		
		printf("\nIteracion: %d\n", i);
		i--;
		
		// Espera a que haya un hueco
		printf("Productor: WAIT huecos\n");
		operacion.sem_num = H_DISP;
		operacion.sem_op = -1;
		semop(semid, &operacion, 1);
		
		// Si hay algun hueco, deja un producto
		if ( semctl(semid,H_DISP,GETVAL,0) > 0){
			dejarProducto(cinta);
			printf("Quedan %d huecos\n", semctl(semid,H_DISP,GETVAL,0));
			pausa("Productor", "dentro");
		}
		
		// Ahora hay un producto nuevo
		printf("Productor: SIGNAL productos\n");
		operacion.sem_num = P_DISP;
		operacion.sem_op = 1;
		semop(semid,&operacion,1);
		
		pausa("Productor", "fuera");
	}
	
	// Borrado de los semaforos
	semctl(semid,0,IPC_RMID,0);
	semctl(semid,1,IPC_RMID,0);
	semctl(semid,2,IPC_RMID,0);
	
	// Borrado de la memoria compartida
 	shmctl(shmid, IPC_RMID,0);
	
}



















