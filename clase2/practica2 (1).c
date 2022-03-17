#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
Practica2
Basandose en el programa prog1.c proporcionado, escribir otro para que se creen dos pipes,
una para comunicación de padre a hijo, y otra de hijo a padre. El Padre deberá
adivinar un numero elegido por el hijo (entre 0 y 256). Para ello, enviará sus intentos
al hijo a través de la pipe. El hijo contestará con un mensaje para decir si el numero es
mayor, menor o si ha acertado. En caso de acierto los proceso terminan.
*/

int potencia(int base, int exponente){
	int potencia = base;
	
	for (int i=1; i< exponente; i++){
		potencia = potencia*base;
	}
	printf("Tu potencia es: %d\n", potencia);
	return potencia;
}

int main(void)
{
	int fdPreg[2];
	int fdResp[2];
	int status;
 	int nread;
	int numSecreto=0;
	int numIntentos=0;
	
 	char buffer[100];
	int intento = 128; 		//el intento del padre en int
	char intentoChar[100];  //el numero del padre traducido a cadena para enviarlo
	int intentoTrad; 		//el msg traducido de nuevo a int por el hijo
	
	char bufferResp[100];
	int respuesta=0; 			//el codigo de respuesta del hijo al padre; 1=mayor, 2=menor, 3=acierto
	char respuestaChar[100]; 	// la respuesta pasada a cadena
	int respuestaTrad=0; 		// la respuesta traducida de nuevo a int

	
	// Creacion de la pipe de preguntas
	if(pipe(fdPreg) == -1){
		perror("pipePreg");
		exit(EXIT_FAILURE);
	}
 	printf("Pipe pregunta OK!\n");
	
	// Creacion de la pipe de respuestas
	if(pipe(fdResp) == -1){
		perror("pipeResp");
		exit(EXIT_FAILURE);
	}
 	printf("Pipe respuesta OK!\n");
	
	srandom(getpid());
	
	 // Creacion de un proceso hijo
	switch(fork())
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			// Crea un numero al azar entre 0 y 256 para que el padre lo adivine
			numSecreto = (int)(256.0*random()/RAND_MAX);
			printf("Hijo: El numero secreto es el %d\n", numSecreto);
		
			// Cierre del descriptor de escritura de la pipe en el proceso hijo
			if(close(fdPreg[1]) == -1)
			perror("closeHijo");
						
			while(respuesta != 3) {

				// Lectura de la información contenida en la pipe de pregunta
				switch(nread = read(fdPreg[0],buffer,sizeof(buffer)))
				{
					case -1:
						perror("readHijo");
						break;
					case 0:
						perror("EOF");
						break;
					default:
						intentoTrad = atoi(buffer);
						//printf("Hijo: he leido el numero %d\n",intentoTrad);
						if (intentoTrad > numSecreto){
							printf ("Hijo: Incorrecto, tu numero es mayor que el secreto\n");
							respuesta = 1;
						} else if (intentoTrad < numSecreto){
							printf( "Hijo: Incorrecto, tu numero es menor que el secreto\n");
							respuesta = 2;
						} else {
							printf("Correcto! Has adivinado el numero secreto\n");
							respuesta = 3;
						}

						// El hijo escribe su respuesta en el pipe de respuesta
						sprintf (respuestaChar, "%d", respuesta);
						if(write(fdResp[1], respuestaChar , sizeof(respuestaChar)) == -1)
							perror("writeRespuesta");
						if (respuesta == 3){
							exit(EXIT_SUCCESS);
						}
						
				}
			}	
	
	}
	
//Este es el PADRE ¡
	
	/* Cierre del descriptor de lectura en el proceso padre */
	if(close(fdPreg[0]) ==- 1)
		perror("closeLecturaPadre");
	
	while (respuestaTrad != 3){
	
		switch (respuestaTrad){
			case 0:
				intento = 128;
				printf("Padre: Intento inicial con %d\n", intento);
				numIntentos = 1;
				break;
			case 1: //si el numero es mayor q el secreto
				numIntentos++;
				//int i = potencia(2, numIntentos);
				//intento = intento - intento/i;
				intento--;
				printf("Padre: Intento uno menor, el %d\n", intento);
				break;
			case 2: // si el numero es menor q el secreto
				numIntentos++;
				//int j = potencia(2, numIntentos);
				//intento = intento + intento/j;
				intento++;
				printf("Padre: Intento uno mayor, el %d\n", intento);
				break;
			default:
				break;
		}

		sprintf(intentoChar, "%d", intento); //Transformamos el intento en una cadena para enviarla

		 /* El proceso padre escribe en la pipe de Pregunta el numero que intente*/
		if(write(fdPreg[1], intentoChar , sizeof(intentoChar)) == -1)
			perror("writeIntento");

		/*El proceso padre lee la respuesta del hijo*/
		switch(nread = read(fdResp[0],bufferResp,sizeof(bufferResp)))
		{
			case -1:
			 perror("readRespuesta");
			 break;
			case 0:
			 perror("EOF");
			 break;
			default:
				respuestaTrad = atoi(bufferResp);
				//printf("Padre: La respuesta que me da mi hijo es un %d\n", respuestaTrad);
				if (respuestaTrad == 3){
					printf("Padre: He adivinado el numero en %d intentos\n", numIntentos);
					//exit(EXIT_SUCCESS);
				}
		}
	}
	
	
 /* El proceso padre espera la finalizacion del proceso hijo */
 wait(&status);
 exit(EXIT_SUCCESS);
}