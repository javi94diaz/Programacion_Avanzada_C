#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/* Variables globales */
/* Función principal */
int main (void)
{
 int i;
 int status;
 pid_t pid; /* pid_t es un entero que representa el
identificador ID de un
 proceso */

 char *nombre = "programa_hijo";
 // probar con otros !
 // char *nombre = "/bin/ls";
 // char *nombre = "/bin/ps";

 char *av[2]; /* lista de argumentos */

 /* Creación de un proceso hijo */
 switch(pid = fork())
{
case (pid_t) -1:
 /* void perror(const char *message): imprime un
mensaje de error asociado a la cadena que tiene como
argumento */
 perror("fork");
 exit(-1);
case (pid_t) 0:
 printf("Hola, yo soy el hijo, y tengo PID %d. Mi padre es el PID %d\n",getpid(),getppid());
 printf("ahora ejecuto otro programa !\n");
 av[0]=nombre;
 av[1]=NULL;
 if(execve(nombre,av,NULL) == -1)
{
 perror("execve");
 exit(EXIT_FAILURE);
}
 break;
default:
 printf("Hola, yo soy el padre, y tengo PID %d.\n",getpid());
 break;
}
 exit(0);
}
