#ifndef VACUNA_H_INCLUDED
#define VACUNA_H_INCLUDED

#include <time.h>
#include <winsock.h>
#include <mysql.h>
#include <windows.h>

/*
dicc_vacuna = Secuencia???(vacuna)
vacuna = lote x tipo_vacuna x laboratorio x vencimiento

Se agrego lote porque en si las vacunas de un lote se vencen el mismo día
y luego en si no importa si el paciente se dió la vacuna 1 o 2 de ese lote.
*/
#define TAM_CHAR_VAC 30
#define TAM_DATE_VENC 20 //dd-mm-yyyy

typedef struct
{
    int num_lote;
    char tipo_vacuna[TAM_CHAR_VAC];
    char lab[TAM_CHAR_VAC];
    //time_t vencimiento;
    char vencimiento[TAM_DATE_VENC];
    int cant_dosis;
} Lote_vacunas;

typedef struct nodo_vac
{
    Lote_vacunas lote;
    struct nodo_vac *sig;
}*Dicc_vacuna;

#endif // VACUNA_H_INCLUDED
