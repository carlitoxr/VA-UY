#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "conexion.h"

int login(char usu[], char pass[], int rol)
{
    int resultado;

    MYSQL *conn;
    conn = mysql_init(NULL); //sacarlo cuando se explique y luego agregarlo
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from login where nom_usu='%s' and pass = '%s' and rol = %d", usu, pass, rol);

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);


    if((row = mysql_fetch_row(res)) == NULL)
    {
        mysql_free_result(res);
        mysql_close(conn);
        return 0; //Error
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 1; //BIEN

}

int alta_usuario(char nom_usu[], char pass[], char pri_nom[], char pri_ape[], int rol){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "INSERT INTO login VALUES('%s','%s','%s','%s',%d)",
            nom_usu, pass, pri_nom, pri_ape, rol);

    int resultado = mysql_query(conn, query);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 0; //error
    }

    mysql_close(conn);
    return 1;
}

/*
    0 - no existe
    1 - si existe
*/
int existe_usuario(char nom_usu[]){
    int retorno = 0;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "SELECT * FROM login WHERE nom_usu = '%s'", nom_usu);

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL){
        retorno = 1;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return retorno;
}

void baja_usuario(char nom_usu[]){

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "DELETE FROM login WHERE nom_usu = '%s'", nom_usu);

    int resultado = mysql_query(conn, query);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 0; //error
    }

    mysql_close(conn);
    return 1;
}

void listar_usuario(){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "SELECT * FROM login");

    if (mysql_query(conn, sql))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        printf("%s\t%s\t%s\t%s\t%s\n", row[0], row[1], row[2], row[3], row[4]);
    }

    mysql_free_result(res);
    mysql_close(conn);
}
