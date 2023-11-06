#ifndef SECOED_H_INCLUDED
#define SECOED_H_INCLUDED

#include "pto_vac.h"

/*
dicc_SECOED = Diccionario(SECOED)
SECOED = zona x ubicaci�n

dicc_punto_vacunacion = Diccionario (punto_vacunacion)
punto_vacunacion = ciudad x horario x zona

*/

enum zonas {Este, Sur, Oeste, Norte};

typedef struct nodo_secoed
{
    enum zonas zona;
    char* ubicacion; //�Tambi�n enum?
    Dicc_punto_vacunacion p_vac;
    nodo_secoed *sig;
}*Dicc_secoed;



#endif // SECOED_H_INCLUDED
