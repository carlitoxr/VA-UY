#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agenda.h"

int archivo_existe(){
    FILE * archivo = fopen("./archivo.txt", "a+"); //+a: abre el archivo para lectura/escritura (se sitúa al final del archivo)

    // Si no podemos abrir el archivo, terminamos el programa
    if(archivo == NULL){
        printf("No se pudo abrir el archivo... \n");
        return -1;
    }
}

/*
    Retorno:
        -1: No se pudo abrir el archivo.
*/
int guardar_archivo(){
    FILE * archivo, * arch; //Puntero de archivo

    arch = fopen("./menos_dosis.txt", "w");
    if(arch == NULL){
        printf("No se pudo abrir el archivo... \n");
        return -2;
    }
    fclose(arch);

    archivo = fopen("./menos_dosis.txt", "a+"); //+a: abre el archivo para lectura/escritura (se sitúa al final del archivo)
    // Si no podemos abrir el archivo, terminamos el programa
    if(archivo == NULL){
        printf("No se pudo abrir el archivo... \n");
        return -1;
    }

    /**/
    char * buffer, * cabecera;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[180];
    sprintf(sql, "select p.ciudad, p.zona, v.num_lote, v.tipo_vacuna, v.cant_dosis from pto_vacunacion p, tiene t, vacuna v where p.ciudad = t.ciudad  and t.num_lote = v.num_lote and v.cant_dosis < 10");

    if (mysql_query(conn, sql)){
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    buffer = strdup("Ciudad\t\tZona\t\tNum Lote\tVacuna\tCantidad de dosis\n");

    fputs(buffer, archivo);
    while ((row = mysql_fetch_row(res)) != NULL){
        if(strlen(row[0]) <= 7){
            sprintf(buffer, "%s\t\t%s\t\t   %s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4]);
            fputs(buffer, archivo);
        }
        else{
            sprintf(buffer, "%s\t%s\t\t   %s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4]);
            fputs(buffer, archivo);
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    /**/

    fclose(archivo);
}
