#ifndef LOGIN_H_INC LUDED
#define LOGIN_H_INCLUDED

#include <winsock.h>
#include <mysql.h>
#include <windows.h>

typedef struct
{
    int id;
    char* nom_usu;
    char* pass;
    char* pri_nom;
    char* pri_ape;
    int rol;
} usuario;

int login(char usu[], char pass[], int rol);

#endif // LOGIN_H_INCLUDED
