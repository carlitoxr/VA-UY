
#include "grafo_v4.h"
#include "agenda.h"

char retorno_zona_2[30];
int pudo_agendar = 0;


NodoListaAdy *crearNodoLstAdy(int destino){

    NodoListaAdy* aux = (NodoListaAdy*) malloc(sizeof(NodoListaAdy));

    aux->destino = destino;

    aux->sig = NULL;
    return aux;
}


Grafo *crearGrafo(int vertices)
{
    //Reservar memoria para la estrutura del grafo.
    Grafo *grafo = (Grafo*) malloc(sizeof(Grafo));

    //Reservar para el vector de ciudades.
    grafo->vertices = (ListaAdy*) calloc(vertices, sizeof(ListaAdy));

     // Crear lista de adyacencia vacía en cada nodo.
    for (int i = 0; i < vertices; ++i){
        grafo->vertices[i].cabecera = NULL;
    }

    return grafo;
}

void agregarArista(Grafo *grafo, int origen, int destino){
    // Para representar una arista, añadir un nodo a la lista de adyacencia del origen.
    NodoListaAdy *aux = crearNodoLstAdy(destino);
    //Se inserta en la primera posición de la lista.
    aux->sig = grafo->vertices[origen].cabecera;
    grafo->vertices[origen].cabecera = aux;
}

void crear_grafo_BD(Grafo *grafo){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from grafo_vert");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        agregarArista(grafo, (atoi(row[1])-1), (atoi(row[2])-1));
    }

    mysql_free_result(res);
    mysql_close(conn);
}

void crear_grafo_zona_BD(Grafo *grafo_zona){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from grafo_dpto ptoVac, grafo_vert graf, pto_vacunacion zon where ptoVac.id = graf.pri_vertice and ptoVac.dpto = zon.ciudad;");

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL){
        agregarArista(grafo_zona, (atoi(row[3])-1), (atoi(row[4])-1));
    }

    mysql_free_result(res);
    mysql_close(conn);
}

void imprimir_lista(NodoListaAdy *lista){
    while(lista!=NULL){
        printf("%d\t", (lista->destino + 1));
        lista = lista->sig;
    }
}

void imprimir_grafo(Grafo *grafo){
    for (int i = 0; i < 19; ++i){
        printf("%d: ",i+1);
        //if(grafo->vertices[i].cabecera != NULL)
        imprimir_lista(grafo->vertices[i].cabecera);
        printf("\n");
    }
}

void DFS(Grafo *g, int verticeActual, int visitado[19]){

    visitado[verticeActual] = 1;

    NodoListaAdy *aux = g->vertices[verticeActual].cabecera;

    printf("|DFS| verticeActual %d\n", verticeActual+1);

    while(aux != NULL){
        printf("|DFSasdasd| aux destino %d\n", aux->destino+1);

        if(visitado[aux->destino]==0){
            DFS(g, aux->destino, visitado);
        }

        aux = aux->sig;
    }
}

void DFS2(Grafo *g, int verticeActual, int visitado[19], char ci[], char pri_nom[], char pri_ape[], char fecha_de_nac[], char localidad[]){
    obtener_ciudad_vertice2(verticeActual+1);
    //printf("|DFS2!!| VA A TRATAR DE AGENDAR EN %s\n", retorno_zona_2);
    //system("PAUSE");
    if(pudo_agendar==0){ //Si aún no pudo agendar entonces sigo en las subramas
        pudo_agendar = agendar2(ci, pri_nom, pri_ape, fecha_de_nac, retorno_zona_2);
        //printf("|DFS2!!| PUDO AGENDAR %d\n", pudo_agendar);

        visitado[verticeActual] = 1;

        NodoListaAdy *aux = g->vertices[verticeActual].cabecera;

        //printf("|DFS| verticeActual %d\n", verticeActual+1);

        while(aux != NULL){
            //printf("|DFSasdasd| aux destino %d\n", aux->destino+1);

            if(visitado[aux->destino]==0){
                obtener_ciudad_vertice2(aux->destino+1);
                DFS2(g, aux->destino, visitado, ci, pri_nom, pri_ape, fecha_de_nac, retorno_zona_2);
            }

            aux = aux->sig;
        }
    }
}

void resetear_pudo_agendar(){
    pudo_agendar = 0;
}

int obtener_vertice_grafo(char ciudad_pto_vac[]){
    int vertice;
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select id from grafo_dpto where dpto = '%s'", ciudad_pto_vac);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL){
        vertice = atoi(row[0]);
        mysql_free_result(res);
        mysql_close(conn);
        return vertice;
    }

    mysql_free_result(res);
    mysql_close(conn);
    return vertice;
}

char* obtener_ciudad_vertice(int vertice){
    char *retorno_zona = NULL;
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from grafo_dpto where id = %d", vertice);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL){
        retorno_zona = strdup(row[1]);
    }

    mysql_free_result(res);
    mysql_close(conn);

    return retorno_zona;
}

void obtener_ciudad_vertice2(int vertice){
    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[120];
    sprintf(sql, "select * from grafo_dpto where id = %d", vertice);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL){
        sprintf(retorno_zona_2, "%s", row[1]);
    }

    mysql_free_result(res);
    mysql_close(conn);
}

//Dado un punto de vacunación, se muestra el SECOED al que pertenece
//y los punto de vacunación vecinos.
//Esta función retorna los puntos de vacunación vecinos
void info_pto_vac(Grafo *g, char localidad_pto_vac[]){
    NodoListaAdy *aux = g->vertices[obtener_vertice_grafo(localidad_pto_vac)-1].cabecera;

    printf("Punto de vacunacion vecinos:\n");

    while(aux != NULL){
        obtener_ciudad_vertice2(aux->destino+1);
        printf("\t%s\n", retorno_zona_2);

        aux = aux->sig;
    }
}

void imprimir_lista_MSP(NodoListaAdy *lista){
    while(lista!=NULL){
        printf("%s ",obtener_ciudad_vertice(lista->destino+1));
        //obtener_ciudad_vertice(lista->destino+1);
        //printf("%d\t", (lista->destino + 1));
        lista = lista->sig;
    }
}
void imprimir_grafo_MSP(Grafo *grafo){
    for (int i = 0; i < 19; ++i){
        if(grafo->vertices[i].cabecera != NULL){
            printf("%s: ",obtener_ciudad_vertice(i+1));
            imprimir_lista_MSP(grafo->vertices[i].cabecera);
            printf("\n");
        }
    }
}

void crear_grafo_visita_MSP(Grafo * pto_vac_visitar, char localidad[]){

    MYSQL *conn;
    conn = mysql_init(NULL);
    conn = conectar();

    MYSQL_RES *res;
    MYSQL_ROW row;

    char sql[180];
    sprintf(sql, "select * from grafo_dpto ptoVac, grafo_vert graf, pto_vacunacion p where ptoVac.id = graf.pri_vertice and ptoVac.dpto = p.ciudad and p.zona = '%s'", localidad); //printf("%s\n", sql);

    if (mysql_query(conn, sql)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
    }

    res = mysql_use_result(conn);

    printf("***Puntos de vacunacion a visitar***\n");
    while ((row = mysql_fetch_row(res)) != NULL){
        agregarArista(pto_vac_visitar, (atoi(row[3])-1), (atoi(row[4])-1));
    }

    mysql_free_result(res);
    mysql_close(conn);
}
