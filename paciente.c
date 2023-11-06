#include "paciente.h"

void alta_paciente(char ci[], char pri_nom[], char pri_ape[], char fecha_de_nac[], char localidad[]){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "INSERT INTO paciente VALUES('%s','%s','%s','%s','%s')",
            ci, pri_nom, pri_ape, fecha_de_nac, localidad);

    //printf("\n%s\n", query);

    int resultado = mysql_query(conn, query);


    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
    }

    mysql_close(conn);
}
