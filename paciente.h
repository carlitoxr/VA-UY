#ifndef PACIENTE_H_INCLUDED
#define PACIENTE_H_INCLUDED

#include <windows.h>
#include <mysql.h>
#include <stdio.h>


/*
dicc_paciente = Diccionario(paciente)
paciente = ci x nombre_completo x fecha_de_nac x localidad
*/

/*typedef struct
{
    char* nombre;
    char* apellido;
} Nombre_completo;

typedef struct
{
    char* ci;
    Nombre_completo nom;
    time_t fecha_de_nac;
    char* localidad;
} Paciente;

typedef struct nodo_paciente
{
    Paciente paciente;
    nodo_paciente *sig;
}*Dicc_paciente;*/

#endif // PACIENTE_H_INCLUDED
