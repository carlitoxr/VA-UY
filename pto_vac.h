#ifndef PTO_VAC_H_INCLUDED
#define PTO_VAC_H_INCLUDED

#include "vacuna.h"

/*
dicc_punto_vacunacion = Diccionario (punto_vacunacion)
punto_vacunacion = ciudad x horario x zona
*/

/*
typedef struct { //porque yo voy a necesitar solo esta info en las otras partes
    char* ciudad;
    //horario. Se puede sacar por el horario de la pc
    Dicc_vacuna vacunas;
}Pto_vacunacion;

typedef struct nodo_pto_vacunacion{
    Pto_vacunacion p_vacunacion;
    nodo_pto_vacunacion *sig;
}*Dicc_punto_vacunacion;*/


void listar_pto_vac_BD();

#endif // PTO_VAC_H_INCLUDED
