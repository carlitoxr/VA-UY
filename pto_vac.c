#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vacuna.h"
#include "pto_vac.h"
#include "agenda.h"

void listar_pto_vac_BD()
{
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "SELECT * FROM pto_vacunacion");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s\t%s \n", row[0], row[1]);
    }
    if((row = mysql_fetch_row(res)) != NULL)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0;
    }

    mysql_free_result(res);
    mysql_close(conn);
}

char * obtener_zona_punto_vacunacion(char ciudad_pto_vac[]){
    char *retorno_zona = NULL;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "SELECT zona FROM pto_vacunacion WHERE ciudad = '%s'", ciudad_pto_vac);

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        retorno_zona = strdup(row[0]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return retorno_zona;
}

/*
    Al SINAE le interesa saber cuántos SECOED existen y
    las localidades donde están sus puntos de vacunación
    junto con la disponibilidad de dosis de cada punto.
*/

//Al SINAE le interesa saber cuántos SECOED existen
int cant_secoed(){
    int cant = 0;
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select count(*) from secoed");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL){
        cant = atoi(row[0]);
    }

    mysql_free_result(res);
    mysql_close(conn);
    return cant;
}

//localidades donde están sus puntos de vacunación junto con la disponibilidad de dosis de cada punto
void cant_dosis_todos_pto_vac(){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[180];
    sprintf(sql, "select p.ciudad, p.zona, v.num_lote, v.tipo_vacuna, v.cant_dosis from pto_vacunacion p, tiene t, vacuna v where p.ciudad = t.ciudad  and t.num_lote = v.num_lote order by p.ciudad");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);
    printf("Ciudad\t\tZona\t\tLote\t\tVacuna\t\tCantidad de dosis\n");
    while ((row = mysql_fetch_row(res)) != NULL){
        if(strlen(row[0]) <= 7)
            printf("%s\t\t%s\t\t%s\t\t%s", row[0], row[1], row[2], row[3]);
        else
            printf("%s\t%s\t\t%s\t\t%s", row[0], row[1], row[2], row[3]);

        if(strlen(row[3]) <= 7)
            printf("\t\t\t%s", row[4]);
        else
            printf("\t\t%s", row[4]);
        printf("\n");
    }

    mysql_free_result(res);
    mysql_close(conn);
}

//Puntos de vacunación con existencia menor a 10 dosis
void pto_vac_pocas_dosis(){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[180];
    sprintf(sql, "select p.ciudad, p.zona, v.num_lote, v.tipo_vacuna, v.cant_dosis from pto_vacunacion p, tiene t, vacuna v where p.ciudad = t.ciudad  and t.num_lote = v.num_lote and v.cant_dosis < 10");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);
    printf("Ciudad\t\tZona\t\tNum Lote\tVacuna\tCantidad de dosis\n");
    while ((row = mysql_fetch_row(res)) != NULL){
        //printf("%d\n", strlen(row[0]));
        if(strlen(row[0]) <= 7)
            printf("%s\t\t%s\t\t   %s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4]);
        else
            printf("%s\t%s\t\t   %s\t\t%s\t\t%s\n", row[0], row[1], row[2], row[3], row[4]);
    }

    mysql_free_result(res);
    mysql_close(conn);
}
