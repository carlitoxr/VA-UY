
#include "agenda.h"


    char aux_fech[20];
    char aux_hora[5];
    char ult_hora[5];
    int aux_visitado[19];
    int alta_ok = 0;
    int agenda_dfs = 0;

void inicializar_aux_visitado(){
    for(int i=0; i<19; i++)
        aux_visitado[i] = 0;
}

void agendar_dfs(char ci[], char pri_nom[], char pri_ape[], char fecha_de_nac[], char localidad[], Grafo *grafo){
    int visitado[19];
    for(int i=0; i<19; i++){ //Inicializo el arreglo visitado
        visitado[i] = 0;
    }
    int vertice, vertice_inicial = obtener_vertice_grafo(localidad);
    //printf("HOLA agendar_dfs %s %s %s %s %s\n", ci, pri_nom, pri_ape, fecha_de_nac, localidad);

    //printf("\nHOLA agendar_dfs vertice_inicial: %d\n", vertice_inicial);
    vertice = DFS2(grafo, vertice_inicial-1, visitado, ci, pri_nom, pri_ape, fecha_de_nac, obtener_ciudad_vertice(vertice+1)); //printf("\nagendar_dfs vertice: %d\n", vertice);

    //agendar2(ci, pri_nom, pri_ape, fecha_de_nac, obtener_ciudad_vertice(vertice+1), grafo);

    //vertice = obtener_vertice_grafo(localidad); //Marco el vertice de la localidad de la persona como visitada
    //visitado[vertice-1] = 1; //Marco el vertice de la localidad de la persona como visitada
}

int agendar2(char ci[], char pri_nom[], char pri_ape[], char fecha_de_nac[], char localidad[]){
    int dia, mes, anio;
    char s_dia[2], s_mes[2], s_anio[4];
    int incremento = 1; //Se inicializa en 1 ya que se agenda apartir de mañana.
    char fecha[40], fecha_ult[40];

    char hora[5];
    int cant_filas;

    /*¿Hay dosis?*/
    if(cant_dosis_p_vac(localidad) > 0){
        dia_habil(&dia, &mes, &anio, incremento);
        formatear_dia(dia, mes, anio);

        cant_filas = obtener_cant_filas_agenda(localidad, aux_fech);

        sprintf(aux_hora, "08:00");

        sprintf(fecha, "%s %s", aux_fech, aux_hora);

        /*si cant_filas es 0 entonces doy de alta*/
        if(cant_filas == 0){

            alta_agenda(ci, obtener_num_lote(localidad), localidad, obtener_zona_punto_vacunacion(localidad), fecha);
            restar_dosis(localidad, obtener_num_lote(localidad));
            return 1;
        }
        /*si cant_filas es 1 entonces aux_hora + 35 y doy de alta hasta 19.05*/
        else{
            obtener_ultima_hora(fecha, localidad);
            sprintf(fecha, "%s %s", aux_fech, ult_hora);

            for(int i=0; i<10; i++)
                fecha_ult[i] = fecha[i];
            fecha_ult[10] = ' ';
            sprintf(fecha_ult, "%s%s", fecha_ult, ult_hora);

            //printf("|agendar2| ult_hora %s\n", ult_hora);

            if(strcmp(ult_hora, "20:15") != 0){
                alta_agenda(ci, obtener_num_lote(localidad), localidad, obtener_zona_punto_vacunacion(localidad), fecha_ult);
                restar_dosis(localidad, obtener_num_lote(localidad));
                //cant_dosis_todos_pto_vac();
                return 1;
            }

        }
    }
    return 0;
}

/*
    Función que retorna la última hora de la fecha dada por parámetro
*/
void obtener_ultima_hora(char fecha[40]){
    int hora, minutos, i=0;

    hora = ((int)fecha[12] - '0') + (((int)fecha[11] - '0') *10);
    printf("obtener_ultima_hora hora %d\n",hora);

    minutos = ((int)fecha[15] - '0') + (((int)fecha[14] - '0') *10);
    printf("obtener_ultima_hora minutos %d\n",minutos);


    minutos += 35; //Le sumo 35 minutos
    if(minutos >= 60){
        hora++;
        minutos = minutos - 60;
    }

    formatear_hora(hora, minutos);

}

void formatear_hora(int hora, int minutos){
    char aux_h[3], aux_m[3];

    if(minutos < 10){
        sprintf(aux_m, "0%d", minutos);
    } else{
        sprintf(aux_m, "%d", minutos);
    }
    if(hora < 10){
        sprintf(aux_h, "0%d", hora);
    } else{
        sprintf(aux_h, "%d", hora);
    }
    sprintf(ult_hora, "%s:%s", aux_h, aux_m);
    //printf("HOLA formatear_hora %s\n", ult_hora);

}

/*
Procedimiento que formatea el día, le agrega 0 adelante del número si es menor a 10.
*/
void formatear_dia(int dia, int mes, int anio){
    if(dia < 10){
        sprintf(aux_fech, "0%d-", dia);
    } else{
        sprintf(aux_fech, "%d-", dia);
    }
    if(mes < 10){
        sprintf(aux_fech, "%s0%d-", aux_fech, mes);
    } else{
        sprintf(aux_fech, "%s0%d-", aux_fech, mes);
    }
    sprintf(aux_fech, "%s%d", aux_fech, anio);

}

/*
    Función que obtiene el día habil siguiente para agendar

    incremento: cantidad de días que se suman dependiendo de las veces que se hizo un ciclo entero de recorrida
*/
void dia_habil(int *dia, int *mes, int *anio, int incremento){
    int dia_semana;
    /*CON CURRENT*/
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);

    ptr->tm_mday = ptr->tm_mday + incremento;
    time_t time = mktime(ptr);

    dia_semana = ptr->tm_wday; //tm_wday: Día desde el domingo (Sun)
    *dia = ptr->tm_mday;
    *mes = ptr->tm_mon+1;
    *anio = 1900 + ptr->tm_year;

    if( dia_semana == 6 ){
        *dia = *dia + 2;
    }
    if(dia_semana == 0){
        *dia = *dia + 1;
    }

    //printf("HOLA dia_habil %d %d %d\n", *dia, *mes, *anio);
}

int obtener_num_lote(char localidad[]){
    int num_lote = 0, cant_dosis;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[200];
    sprintf(sql, "select t.zona, t.ciudad, v.num_lote, v.tipo_vacuna, v.cant_dosis from tiene t, vacuna v where t.ciudad = '%s' and t.num_lote = v.num_lote",
            localidad);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){

        cant_dosis = atoi(row[4]);

        if(cant_dosis > 0){
            num_lote = atoi(row[2]);
            //printf("\nobtener_num_lote%s\t%d\t%d \n", row[2], atoi(row[2]), num_lote);
            mysql_free_result(res);
            mysql_close(conn);
            return num_lote;
        }
    }

    mysql_free_result(res);
    mysql_close(conn);

    return num_lote;
}

void obtener_zona_pto_v(char pto_vac[], char *zona)
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
        printf("ERROR1\n");
        mysql_free_result(res);
        mysql_close(conn);
        return NULL;
    }

    sprintf(zona, row[0]);
    mysql_free_result(res);
    mysql_close(conn);

}

int cant_dosis_p_vac(char localidad[]){
    int cant_dosis = 0;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select v.cant_dosis from tiene t, vacuna v where t.ciudad = '%s' and t.num_lote = v.num_lote",
            localidad);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        //printf("cant_dosis_p_vac %s\n", row[0]);
        cant_dosis += atoi(row[0]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return cant_dosis;
}

/*
    Retorna:
        0 - filas null
        1 - filas 1 o mas pero no lleno
        2 - filas lleno (lleno es cuando hora=19:05)
*/
int obtener_cant_filas_agenda(char localidad[], char fecha[]){
    int cant_filas_agenda = 0;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120], sql2[120];
    sprintf(sql, "select * from agenda where ciudad = '%s' and fecha_hora like '%s%%';",
            localidad, fecha);

    //printf("obtener_cant_filas_agenda %s\n", sql);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    row = mysql_fetch_row(res);
    if(row == NULL){
        cant_filas_agenda = 0;
        //printf("obtener_cant_filas_agenda ES NULL\n");
    }
    else{
        do{
            sprintf(fecha, row[5]);
            obtener_hora(fecha);
            if(strcmp(aux_hora,"19:05") == 0){
                cant_filas_agenda = 2;
                //printf("obtener_cant_filas_agenda ES 2\n");
            }
            else{
                cant_filas_agenda = 1;
                //printf("obtener_cant_filas_agenda ES 1\n");
            }
        }while((row = mysql_fetch_row(res)) != NULL);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return cant_filas_agenda;
}

//El sistema tira la hora hh:mm:ss si son las 8:00 am entonces tira 08:00
void obtener_hora(char fecha_hora[]){
    sprintf(aux_hora, "%.*s", 5, aux_hora + 11);
}

/*
disponibilidad dosis misma localidad
Retorna:
    0 - No hay dosis disponibles
    1 - Hay dosis disponibles
*/
int disponibilidad_dosis_localidad(char localidad[]){
    int disp = 0;

    if( cant_dosis_p_vac(localidad) > 0 )
        disp = 1;

    return disp;
}

/*void disponibilidad_hora_localidad(char localidad[]){
    Horarios horarios;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from paciente where localidad = '%s';", localidad);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        //printf("%s\t%s\t%s \n", row[0], row[1], row[2]);
        //horarios.
    }

    mysql_free_result(res);
    mysql_close(conn);
}*/

/*
Disponibilidad horaria 2.0
1) En x día todo libre (current + 1)
2) 1 o mas pero no todo ocupado -> obtenga la última hora y le sumo 35 y lo ingreso.
3) Todo ocupado -> ???
*/

void obtener_dia(int *dia, int *mes, int *anio){
    /*CON CURRENT*/
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);

    *dia = ptr->tm_wday;
    *mes = ptr->tm_mon;
    *anio = 1900 + ptr->tm_year;

    if( *dia == 6 ){ //tm_wday: Día desde el domingo (Sun)
        *dia = *dia + 2;
    }
    if(*dia == 0){
        *dia = *dia + 1;
    }
}

/* disponibilidad_horaria
retorna:
    1 - current + 1 todo libre
    2 - current + 1 registro anterior + 35 min
    3 - current + 1 ???
*/
void disponibilidad_horaria(char localidad[]){
    int resultado = 0;

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from agenda where localidad = '%s';", localidad);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if((row = mysql_fetch_row(res)) == NULL){ //1) En current + 1 día todo libre

        int dia, mes, anio;
        obtener_dia(&dia, &mes, &anio);

        //disponibilidad_horaria_1(localidad, atoi(ptr->tm_mday), atoi(ptr->tm_mon), atoi(ptr->tm_year));
        resultado = 1;
    }

    mysql_free_result(res);
    mysql_close(conn);

    return resultado;
}

//void alta_agenda(int cod, char ci[], int num_lote, char ciudad[], char *zona, char fecha_hora[]){
void alta_agenda(char ci[], int num_lote, char ciudad[], char *zona, char fecha_hora[]){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "INSERT INTO agenda(ci, num_lote, ciudad, zona, fecha_hora) VALUES('%s',%d,'%s','%s','%s')",
            ci, num_lote, ciudad, zona, fecha_hora);

    //printf("\n%s\n", query);

    int resultado = mysql_query(conn, query);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
    }

    mysql_close(conn);
    alta_ok = 1;
}

void cargar_visitado(char zona[]){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[200];
    sprintf(sql, "select * from grafo_dpto ptoVac, grafo_vert graf, pto_vacunacion zon where ptoVac.id = graf.pri_vertice and ptoVac.dpto = zon.ciudad and zona = '%s' group by ptoVac.id;", zona);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        aux_visitado[atoi(row[0])-1] = 0;
    }

    mysql_free_result(res);
    mysql_close(conn);
}

int visitado_todo_true(){
    for(int i=0; i<19; i++){
        if(aux_visitado[i] == 0)
            return 0;
    }
    return 1;
}

/*
    Procedimiento que resta una dosis de la vacuna asignada al paciente
*/
void restar_dosis(char localidad_pto_vac[], int num_lote){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    char query[250];
    sprintf(query, "update vacuna set cant_dosis = (select v.cant_dosis from vacuna v, tiene t where t.ciudad = '%s' and v.num_lote = %d and t.num_lote = %d) - 1 where num_lote = %d;", localidad_pto_vac, num_lote, num_lote, num_lote);
//printf("\n%s\n", query);
    int resultado = mysql_query(conn, query);

    if (resultado > 0)
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
    }

    mysql_close(conn);
}
