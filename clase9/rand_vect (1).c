#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void imprime(int vect[], int dim) {
	
    for (int i = 0; i < dim; i++) {
        printf("%d ", vect[i]);
    }
	printf("\n");
}

void rand_vect(int*v, int n)
{
    int i;

    for(i=0;i<n;i++)
      v[i]=(int)(rand());

}

int main(int argc, char**argv)
{
	//int vect[10000];

	float etime;
	clock_t start, end;
	int dim=0;
	
	while(dim <= 0){
		
		printf("\nIntroduce dimension del vector: ");
		scanf("%d", &dim);
	}
	
	int* vect = (int*) malloc(dim*sizeof(int));
	
	
	srand(getpid());
	rand_vect(vect,dim);
	
	
	start=clock();  

	end=clock();  
	
	imprime(vect, dim);
	
	etime=(end-start)/(float)CLOCKS_PER_SEC;
	printf("Tiempo: %f\n",etime); 

}