/*
Practica 3 - Javier Diaz Mena - 12100

Escribir un programa para que se cree una cola de mensajes para comunicación entre un
proceso padre y un hijo. El padre deberá adivinar un numero elegido por el hijo (entre
0 y 100). Para ello, enviará sus intentos al hijo a través de mensajes. El hijo contestará
con un mensaje para decir si el numero es mayor, menor o si ha acertado. En caso de
acierto los proceso terminan. Usar el campo "mtype" de la estructura msgbuf para
identificar los mensajes (ej: el padre envía mensajes de tipo 1 y el hijo de tipo 2)
*/


#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "adivinar.h"


// Función auxiliar que crea la cola, retornando el identificador
int crearcola(int key)
{
	 int msgqid;
	 msgqid = msgget(key, IPC_CREAT|0666);
	 switch(msgqid)
	{
		case -1:
			perror("msgget");
			return(-1);
		default:
			return msgqid;
	}
}


int main(void)
{
	
	int status;
 	int numSecreto=0; 		// Numero que el padre debe adivinar
	int numIntentos=0; 		// Contador para ver cuantos intentos hace el padre
	
 	
	int intento = 128; 		// Intento del padre en int
	int intentoTrad;  		// El msg traducido de nuevo a int por el hijo
	
	int respuesta=0; 		// El codigo de respuesta del hijo al padre; 1=mayor, 2=menor, 3=acierto
	int respuestaTrad=0; 	// La respuesta traducida de nuevo a int
	
	int id_cola, i, ret;
	struct msgbuf mensajePadre, mensajeHijo;
	
	// Creación de la cola
	id_cola=crearcola(Clave_cola);
	
	if(id_cola==-1){
		 printf("No se ha podido crear la cola !\n");
		 exit(EXIT_FAILURE);
	}
	
	srandom(getpid()); 
	
	// Creacion de un proceso Hijo
	switch(fork())
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			
			srandom(getpid());
			// Crea un numero al azar entre 1 y 100 para que el padre lo adivine
			numSecreto = (int) 100.0*random()/RAND_MAX;
			//printf("Hijo: El numero secreto es el %d\n", numSecreto);
								
			while(respuesta != 3) {
				
				// Lectura del mensaje enviado por el padre de tipo 1
				mensajeHijo.mtype = 1;
				ret = msgrcv(id_cola, &mensajeHijo, sizeof(mensajeHijo.mtext), mensajeHijo.mtype, 0);

				if(ret == -1){
					 perror("msgrcv Hijo");
					 exit(EXIT_FAILURE);
				}
				
				//printf("Hijo: he leido el intento: %s\n",mensajeHijo.mtext);
				
				intentoTrad = atoi(mensajeHijo.mtext);
				
				// Evaluar el intento
				if (intentoTrad > numSecreto){
					printf ("Hijo: Incorrecto, tu numero es mayor que el secreto\n");
					respuesta = 1;
				} else if (intentoTrad < numSecreto){
					printf( "Hijo: Incorrecto, tu numero es menor que el secreto\n");
					respuesta = 2;
				} else {
					printf("Hijo: Correcto! Has adivinado el numero secreto, el %d\n", numSecreto);
					respuesta = 3;
				}

				// El hijo escribe su respuesta de tipo 2
				mensajeHijo.mtype = 2;
				sprintf (mensajeHijo.mtext, "%d", respuesta);
				ret = msgsnd(id_cola, &mensajeHijo, sizeof(mensajeHijo.mtext),0);
		 		
				if(ret == -1){
			 		perror("msgsnd Hijo");
			 		exit(EXIT_FAILURE);
				}
				//printf("Hijo: He enviado la respuesta: %d\n",respuesta);
				
				if (respuesta == 3){
					exit(EXIT_SUCCESS);
				}
			}
	}	
	
	// Proceso Padre
	while (respuestaTrad != 3){
		
		switch (respuestaTrad){
			case 0:
				intento = (int) 100.0*random()/RAND_MAX;
				printf("Padre: Intento inicial con %d\n", intento);
				numIntentos = 1;
				break;
			case 1: //si el numero es mayor que el secreto
				numIntentos++;
				intento--;
				printf("Padre: Intento uno menor, el %d\n", intento);
				break;
			case 2: // si el numero es menor que el secreto
				numIntentos++;
				intento++;
				printf("Padre: Intento uno mayor, el %d\n", intento);
				break;
			default:
				break;
		}

		// Envío del intento (mensaje tipo 1)
		mensajePadre.mtype = 1;
		sprintf(mensajePadre.mtext, "%d", intento);
		ret = msgsnd(id_cola, &mensajePadre, sizeof(mensajePadre.mtext), 0);
		
		if(ret == -1){
			 perror("msgsnd Padre");
			 exit(EXIT_FAILURE);
		}
		//printf("Padre: He enviado el intento: %s\n",mensajePadre.mtext);
			
		// Lectura de la respuesta del hijo (mensaje tipo 2)
		mensajePadre.mtype = 2;
		ret = msgrcv(id_cola, &mensajePadre, sizeof(mensajePadre.mtext), mensajePadre.mtype, 0);

		if(ret == -1){
			 perror("msgrcv Padre");
			 exit(EXIT_FAILURE);
		}
		
		//printf("Padre: he leido la respuesta: %s\n",mensajePadre.mtext);
		
		respuestaTrad = atoi(mensajePadre.mtext);
		
		if (respuestaTrad == 3){
			printf("Padre: He adivinado el numero en %d intentos\n", numIntentos);
		}
	}
	
	// Elimina la cola del sistema
	ret=msgctl(id_cola,IPC_RMID,(struct msqid_ds*)0);
	if(ret==-1){
		perror("msgctl");
	}
	else{
		printf("La cola se ha cerrado correctamente\n"); 
	}
	
	
	// El proceso padre espera la finalizacion del proceso hijo
	wait(&status);
	exit(EXIT_SUCCESS);
}

