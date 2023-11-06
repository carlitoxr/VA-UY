#include <stdio.h>
#include <stdlib.h>

#include "conexion.h"

/*
    ---- CONSULTAS CON ARREGLOS, NO PUNTEROS
    ENTONCES SE CONCATENA CON ARREGLOS NO CON PUNTEROS!!!!!!!!!!!
*/

MYSQL* conectar()
{
    MYSQL *conn;

    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn,MYSQL_HOST, MYSQL_USER, MYSQL_PASS, BD_NOMBRE, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return conn;
    }

    return conn;
}
