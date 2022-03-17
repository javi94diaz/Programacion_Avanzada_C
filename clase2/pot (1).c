
#include <stdio.h>

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
int num = 0;
int pot = 1;
printf("numero: ");
scanf("%d", &num);
pot = potencia (2, num);
printf("2 elevado a %d es: %d\n", num, pot);

}