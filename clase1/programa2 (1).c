#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/* Variables globales */
pid_t pid; /* pid_t es un entero que representa el identificador
(ID) de un proceso */
/* Función principal */
int main (void)
{
 /* Creación de un proceso hijo */
 switch(pid = fork())
{
case (pid_t) -1:
 perror("fork");
 exit(-1);
case (pid_t) 0:
 printf("Hola, yo soy el hijo, y tengo PID %d. Mi padre es el PID %d\n",getpid(),getppid());
 break;
default:
 printf("Hola, yo soy el padre, y tengo PID %d\n",getpid());
 break;
}
 exit(0);
}
