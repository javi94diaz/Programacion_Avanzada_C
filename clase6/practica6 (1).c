#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

/*
Modificar	el	ejemplo	anterior	de	 forma	que	el	thread	1	se suspenda	hasta	que	el	thread	2	
haya	 hecho	 tres	 iteraciones	 de	 su	 bucle. Para	 ello	 se	 usará	 una	 condition	 variable,	 y	 un	
mutex asociado	a	ella. Las	dos	funciones	tendrán	la	siguiente	forma: (ver function1 y function2)
*/

int una_variable_global = -1;

// Añadir estas variables GLOBALES
pthread_mutex_t mut ;
pthread_cond_t cond ;
// en el main, antes de lanzar los threads, inicializar el mutex
// y la condition variable:
pthread_cond_init(&cond,NULL);
pthread_mutex_init(&mut,NULL);

void* function1(void *arg) {
	int i = 0;
	printf("\n Hello, soy el thread 1 y me voy a dormir...\n");
	// PONER AQUÍ EL CODIGO NECESARIO PARA QUE
	// ESTE THREAD SE SUSPENDA HASTA SER DESPERTADO POR EL OTRO
	for (i=0;i<3;i++)
	{
	printf("\n Hello, soy el thread 1\n");
	sleep(1);
	}
	printf("\n T1 says: bye bye !\n");
	return NULL;
}

void* function2(void *arg) {
	int i;
	for (i=0;i<10;i++)
	{
	printf("\n Hello, soy el thread 2\n" );
	sleep(1);
	if(i==3)
	{
	// PONER AQUÍ EL CODIGO PARA DESBLOQUER EL THREAD 1
	}
	}
	printf("\n T2 says: Hasta luego lucas !\n");
	return NULL;
}

int main(void) {
	pthread_t t1_id,t2_id;
	int i,err;
	float x=0.123;
	err = pthread_create(&t1_id, NULL, &function1, NULL);
	
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
	else
		printf("\n Thread created successfully\n");
		err = pthread_create(&t2_id, NULL, &function2, &x);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
	else
		printf("\n Thread created successfully\n");
	for (i=0;i<15;i++) {
		printf("\n Hello, soy el proceso principal: x vale %4.3f y la var. global vale %d\n",x,una_variable_global);
		sleep(1);
	}
	return 0;
}








