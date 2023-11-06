#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vacuna.h"
#include "conexion.h"

char zona[20];

//alta_vacuna(pto_vac,tipo_vac,lab,vencimiento,cant_dosis);
int alta_vacuna(int lote, char pto_vac[], char tipo_vac[], char lab[], char vencimiento[], int cant_dosis)
{
    int resultado;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "INSERT INTO vacuna(num_lote, tipo_vacuna, lab, vencimiento, cant_dosis) VALUES(%d,'%s','%s','%s',%d)",lote, tipo_vac, lab, vencimiento, cant_dosis);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 0;
    }

    mysql_close(conn);

    return 1;
}

Lote_vacunas *crear_lote_vacunas(int lote, char tipo_vac[], char lab[], char vencimiento[], int cant_dosis)
{
    Lote_vacunas *v = malloc(sizeof(Lote_vacunas));
    v->num_lote = lote;
    //printf("\n%s\n", tipo_vac);
    strncpy(v->tipo_vacuna, tipo_vac, TAM_CHAR_VAC);
    //printf("\n%s\n", v->tipo_vacuna);

    //printf("\n%s\n", lab);
    strncpy(v->lab, lab, TAM_CHAR_VAC);
    //printf("\n%s\n", v->lab);

    //printf("\n%s\n", vencimiento);
    strncpy(v->vencimiento, vencimiento, TAM_DATE_VENC);
    //printf("\n%s\n", v->vencimiento);
    //v.vencimiento = vencimiento;
    v->cant_dosis = cant_dosis;

    return v;
}

int alta_vacuna_2(Lote_vacunas *v)
{
    int resultado;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "INSERT INTO vacuna(num_lote, tipo_vacuna, lab, vencimiento, cant_dosis) VALUES(%d,'%s','%s','%s',%d)",
            v->num_lote, v->tipo_vacuna, v->lab, v->vencimiento, v->cant_dosis);

    resultado = mysql_query(conn, query);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    mysql_close(conn);

    return 0;
}

int alta_vacuna_tiene(int lote, char pto_vac[])
{
    int resultado;

    obtener_zona_pto_vac(pto_vac);
    //sprintf(zona, "%s", obtener_zona_pto_vac(pto_vac));printf("\n\nalta_vacuna_tiene zona %s\n", zona);

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char sql[250];
    sprintf(sql, "INSERT INTO tiene(num_lote,ciudad,zona) VALUES (%d,'%s','%s')",lote, pto_vac, zona);
    //printf("alta_vacuna_tiene sql %s\n", sql);
    resultado = mysql_query(conn, sql);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    mysql_close(conn);
    return 0;
}

/*
    Obtengo la zona del punto de vacunación
*/
void obtener_zona_pto_vac(char pto_vac[])
{
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[250];
    sprintf(sql, "SELECT zona FROM pto_vacunacion WHERE ciudad = '%s'", pto_vac);
    //printf("%s", sql);

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return NULL;
    }

    res = mysql_use_result(conn);

    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        //printf("ERROR1\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    sprintf(zona, row[0]);

    mysql_free_result(res);
    mysql_close(conn);
}

Dicc_vacuna *crear()
{
    struct nodo_vac *el_v = (struct nodo_vac*) malloc(sizeof(struct nodo_vac));
}

//Persistencia - va trayendo las vacunas de la bd para darselas a add_vacunas para que las agregue al diccionario
Dicc_vacuna *listar_todas_vac()
{
    int resultado;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "SELECT * FROM vacuna");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s\t", row[0]);
        if(strlen(row[1])<=7)
            printf("%s\t\t", row[1]);
        else
            printf("%s\t", row[1]);
        if(strlen(row[2])<=7)
            printf("%s\t\t", row[2]);
        else
            printf("%s\t", row[2]);
        printf("%s\t%s \n", row[3], row[4]);
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

void imprimir_str_lote_vacunas(Lote_vacunas * lote)
{
    printf("Numero de lote: %d\n", lote->num_lote);
    printf("Tipo de vacuna: %s\n", lote->tipo_vacuna);
    printf("Laboratorio: %s\n", lote->lab);
    printf("Vencimiento: %s\n", lote->vencimiento);
    printf("Cantidad de dosis: %d\n", lote->cant_dosis);
}

//Función que se utiliza para la confirmación del alta de un lote de vacunas
int alta_lote_vacunas(Lote_vacunas **lote_vac)
{
    char pto_vac[30];
    char tipo_vac[30];
    char vencimiento[20];
    char lab[30];
    int cant_dosis, lote;
    Lote_vacunas *v = malloc(sizeof(Lote_vacunas));

    char confirmacion;
    int ingreso_correcto = 0;

    fflush(stdin);

    while(ingreso_correcto == 0)
    {
        printf("***Gestion - MSP***\n");
        printf("***Ingreso de vacunas***\n");
        printf("\n");
        printf("Ingresar lote de las vacunas: ");
        scanf("%d",&lote);
        fflush(stdin);

        printf("Ingresar tipo de vacuna: ");
        scanf("%s",&tipo_vac);
        fflush(stdin);
        printf("Ingresar laboratorio: ");
        scanf("%s",&lab);

        fflush(stdin);
        printf("Ingresar vencimiento del lote(yyyy-MM-dd): ");
        scanf("%s",&vencimiento);
        fflush(stdin);
        printf("Ingresar cantidad de dosis: ");
        scanf("%d",&cant_dosis);
        fflush(stdin);

        v = crear_lote_vacunas(lote,tipo_vac,lab,vencimiento,cant_dosis);

        printf("Usted ingreso el siguiente lote:\n");
        imprimir_str_lote_vacunas(v);
        printf("Es correcto?(Y/N)\n");
        fflush(stdin);
        scanf("%c", &confirmacion);

        if(confirmacion == 'Y')
        {
            ingreso_correcto = 1;
        }
    }
    //printf("NumLote %d\n", lote);
    *lote_vac = v;
    return 0;
}
