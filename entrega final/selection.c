/* 
Practica 9 - Recursividad

Implementar el algoritmo de Selection Sort de forma iterativa y recursiva,
y medir los tiempos de ambos para vectores de tamaños cada vez mayores siguiendo
una escala logaritmica (10, 100, 1.000, 10.000 y 100.000 elementos)

*/


#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// Crea un vector de la dimension indicada con elementos aleatorios entre 0 y 100
void rand_vect(int*v, int dim)
{
    for(int i = 0; i < dim; i++)
      v[i]= (int)(rand()%100 + 1);

}


// Imprime un vector por pantalla
void imprime(int vect[], int dim) {
	
	printf("\n");
    for (int i = 0; i < dim; i++) {
        printf("%d ", vect[i]);
    }
	printf("\n");
}


// Algoritmo iterativo Selection Sort
void iterSelectionSort(int vect[], int dim){

	for(int i=0; i< dim-1; i++) {
		
		// La nueva posicion minima es la i, los elementos anteriores ya estan ordenados
		int posMin = i;
		
		// Hallamos la posicion del elemento minimo, comenzando por el elemento i
		for (int j = i; j< dim; j++) {
			
			if ( vect[j] < vect[posMin]) {
				posMin = j;
			}
		}
		
		// Intercambiamos el minimo con el de la posicion i
		int aux = vect[i];
		vect[i] = vect[posMin];
		vect[posMin] = aux;
		
	}
}


// Algoritmo recursivo Selection Sort
int recurSelectionSort(int vect[], int dim) {

	if(dim == 1){
		return 0;
	}
	
	int posMin = 0;
	
	// Hallamos la posicion del elemento minimo
	for (int j = 0; j< dim; j++) {
		
		if ( vect[j] < vect[posMin]) {
			posMin = j;
		}
	}
	
	//Intercambiamos el minimo con el de la posicion 0
	int aux = vect[0];
	vect[0] = vect[posMin];
	vect[posMin] = aux;
	
	//Llamamos a esta misma funcion pasandole la parte aun desordenada y su dimension
	recurSelectionSort(vect+1, dim-1);
	
}


int main(void) {

	float etime1, etime2;
	clock_t start1, end1, start2, end2;
	int dim = 10;
	
	srand(getpid());
	
	while (dim <= 100000){

		// Creamos dos vectores de elementos aleatorios
		int* vect = (int*) malloc(dim*sizeof(int));
		rand_vect(vect,dim);

		int* vect2 = (int*) malloc(dim*sizeof(int));

		for (int k=0; k<dim; k++){
			vect2[k] = vect[k];
		}

		// Vemos vector original (solo cuando hay pocos elementos)
		if (dim <= 100) {
			printf("Vector original de %d elementos: ", dim);
			imprime(vect, dim);
		}

		// Ordenamos con selectionSort iterativo
		start1 = clock(); 
		iterSelectionSort(vect, dim);
		end1 = clock();

		etime1 = (end1-start1)/(float)CLOCKS_PER_SEC;
		printf("\nTiempo sort iterativo con %d elementos: %f seg\n",dim, etime1); 

		// Ordenamos con selectionSort recursivo
		start2 = clock(); 
		recurSelectionSort(vect2, dim);
		end2 = clock();

		etime2 = (end2-start2)/(float)CLOCKS_PER_SEC;
		printf("Tiempo sort recursivo con %d elementos: %f seg\n",dim, etime2);

		// Vemos los vectores finales ordenados con ambos metodos (solo cuando hay pocos elementos)
		if (dim <= 100) {
			printf("\nVector ordenado iterativamente: ");
			imprime(vect, dim);
			printf("Vector ordenado recursivamente: ");
			imprime(vect2, dim);
			printf("\n");
		}
		
		dim *= 10;
		
	}
		
	
	return 0;
}

	
	
	
	
	
	


