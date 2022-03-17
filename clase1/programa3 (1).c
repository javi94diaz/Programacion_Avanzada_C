#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/* Variables globales */
/* Función principal */
int main (void)
{
 int i;
 int status;
 pid_t pid; /* pid_t es un entero que representa el identificador ID de un proceso */
 /* Creación de un proceso hijo */
 switch(pid = fork())
{
case (pid_t) -1:
 perror("fork");  /* void perror(const char *message): imprime un mensaje de error asociado a la cadena que tiene como argumento */
 exit(-1);
case (pid_t) 0:
 printf("Hola, yo soy el hijo, y tengo PID %d. Mi padre es el PID %d\n",getpid(),getppid());
 for(i=0;i<20;i++)
printf("Soy el proceso hijo y estoy perdiendo tiempo...\n");
 break;
default:
 printf("Hola, yo soy el padre, y tengo PID %d. Estoy esperando a que mi hijo termine...\n",getpid());
 waitpid(pid,&status,0);
 /* Alternativa: la función wait(int *status) provoca
una espera del proceso padre hasta que cualquier
proceso hijo finalice. La información del estado del
proceso hijo se almacena en status.*/

 printf("Padre: mi hijo ya termino'!\n");
 break;
}
 exit(0);
}
