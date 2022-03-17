 // Fichero que contiene la información común (clave y estructura de datos) de la Practica 4

#define CLAVE_SHM ((key_t) 1001)

typedef struct{
 int intento;
 int respuesta;
 int turno;
} datos;