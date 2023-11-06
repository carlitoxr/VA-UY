#ifndef AGENDA_H_INCLUDED
#define AGENDA_H_INCLUDED

#include <time.h>
#include <winsock.h>
#include <mysql.h>
#include <windows.h>
#include <stdio.h>

#include "pto_vac.h"
#include "vacuna.h"
#include "paciente.h"
#include "grafo_v4.h"

void inicializar_aux_visitado();
void agendar_dfs(char ci[], char pri_nom[], char pri_ape[], char fecha_de_nac[], char localidad[], Grafo *grafo);


#endif // AGENDA_H_INCLUDED
