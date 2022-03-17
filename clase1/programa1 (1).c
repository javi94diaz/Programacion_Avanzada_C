#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
 /* Definición de las variables utilizadas en el programa */
 uid_t yo; /* uid_t es un entero que representa al usuario IDs */
 struct passwd *mipass; /* passwd es una estructura que presenta distintos campos con información del usuario */
 struct group *migrup; /* group es una estructura que presenta distintos campos con información del grupo de usuarios */
 char **miembros; /* Array de punteros con los nombres de los usuarios del grupo */
 
 pid_t PID,PPID;
 /* Información del proceso */
 PID = getpid();
 PPID = getppid();
 printf("El ID del proceso es %d, el de su padre es %d \n", PID,PPID);
 printf("(Comprobad, a traves del comando ps, que %d es efectivamente el PID de la consola!)\n",PPID);
 yo = getuid();

 mipass = getpwuid(yo);
 if(!mipass)
/* contiene el login del usuario */
/* palabra clave encriptada del usuario */
/* Idinetificador ID de usuario */
{
 printf("No encuentro al usuario %d \n",(int) yo);
 exit(EXIT_FAILURE);
}

 /* Salida por pantalla de la información de usuario */
 printf("Soy : %s \n",mipass->pw_gecos);
 printf("Mi login es: %s\n", mipass->pw_name);
 printf("Mi id es: %d \n", (int) (mipass->pw_uid));
 printf("Mi directorio de trabajo es: %s\n", mipass->pw_dir);
 printf("Mi shell es: %s\n", mipass->pw_shell);

 migrup = getgrgid(mipass->pw_gid);
 if(!migrup)
{
 printf("No encuentro el grupo %d \n",(int) (mipass->pw_gid));
 exit(EXIT_FAILURE);
}

 /* Salida por pantalla de la información de grupo de usuarios */
 printf("Mi grupo es: %s (%d)\n", migrup->gr_name, (int) (mipass->pw_gid));
 printf("Los miembros del grupo son:\n");
 miembros = migrup->gr_mem;

 while(*miembros)
{
 printf("%s \n",*(miembros));
 miembros++;
}

 exit(EXIT_SUCCESS);
}
