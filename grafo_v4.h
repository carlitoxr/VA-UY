#ifndef GRAFO_V4_H_INCLUDED
#define GRAFO_V4_H_INCLUDED

#include <winsock.h>
#include <mysql.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V 19

//typedef struct nodo_graf_v3{
        //int info;
        //nodo_graf_v3 *sig;
    //}*Lista_Graf_v3;
//typedef Lista_Graf_v3 Grafo_v3[V];

/*
 * Nodo Vértice.
 */
typedef struct nodoListaAdy
{
    int destino; //Vértice destino
    //double costo; // Costo o peso.
    struct nodoListaAdy *sig; // Puntero a otro nodo.
} NodoListaAdy;


/*
 * Lista de adyacencia.
 */
typedef struct ListaAdy
{
    NodoListaAdy *cabecera;
} ListaAdy;

/*
 * Grafo representado como arreglo de listas de adyacencia.
 */
typedef struct
{
    //int numeroVertices; // Cantidad de vértices
    ListaAdy *vertices; // Arreglo de listas de adyacencias
} Grafo;

//int pudo_agendar = 0;

NodoListaAdy *crearNodoLstAdy(int destino);
Grafo *crearGrafo(int vertices);
void agregarArista(Grafo *grafo, int origen, int destino);
void crear_grafo_BD(Grafo *grafo);
void imprimir_lista(NodoListaAdy *lista);
void imprimir_grafo(Grafo *grafo);

#endif // GRAFO_V4_H_INCLUDED
