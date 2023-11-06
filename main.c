#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>
#include <mysql.h>
#include <time.h>

#include "login.h"
#include "conexion.h"
#include "vacuna.h"
#include "pto_vac.h"
//#include "grafo_cercania.h"
//#include "grafo_v2.h"
#include "grafo_v4.h"
#include "agenda.h"
#include "paciente.h"

    Grafo *grafo;

    Grafo * pto_vac_visitar;

char* charArryToPoin(char arr[])
{
    char *p = &arr[0];
    return p;
}

int main()
{
    //primer_test();

    menu_principal();

    return 0;
}

/*void primer_test()
{
    char usu[20], pass[20];

    printf("***Prueba de conexion***\n");

    printf("Ingrese usu: ");
    scanf("%s", usu);

    printf("Ingrese pass: ");
    scanf("%s", pass);

    switch(login(usu, pass))
    {
    case -1:
        printf("error al aplicar al tratar de ejecutar la consulta\n");
        break;
    case 0:
        printf("existe el usuario\n");
        break;
    case 1:
        printf("no existe el usuario\n");
        break;
    }
}*/
/*
void prueba_time(){
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("prueba_time %s\n\n", asctime(ptr));
}
void prueba_time2(int *dia, int *mes, int *anio){
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    //printf("prueba_time2 %s\n", asctime(ptr));

    printf("prueba_time2 %d %d %d\n", ptr->tm_mday, ptr->tm_mon, 1900 + ptr->tm_year);
    *dia = ptr->tm_wday;
    *mes = ptr->tm_mon;
    *anio = 1900 + ptr->tm_year;
    //printf("prueba_time2 %d %d %d\n", dia, mes, anio);

    if( *dia == 6 ){ //tm_wday: Día desde el domingo (Sun)
        *dia = *dia + 2;
    }
    if(*dia == 0){
        *dia = *dia + 1;
    }
    printf("prueba_time2 %d %d %d\n", *dia, *mes, *anio);
}
void prueba_time3(int *dia, int *mes, int *anio){
    int dia_semana;
    struct tm* ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    //printf("prueba_time2 %s\n", asctime(ptr));

    printf("prueba_time3 current %d %d %d\n", ptr->tm_mday, ptr->tm_mon, 1900 + ptr->tm_year);

    ptr->tm_mday = ptr->tm_mday + 1;printf("HOLA\n");
    time_t time = mktime(ptr);printf("HOLA\n");
    printf("prueba_time3 current+1 %d %d %d\n", ptr->tm_mday, ptr->tm_mon, 1900 + ptr->tm_year);

    dia_semana = ptr->tm_wday;
    *dia = ptr->tm_mday;
    *mes = ptr->tm_mon+1;
    *anio = 1900 + ptr->tm_year;
    //printf("prueba_time2 %d %d %d\n", dia, mes, anio);

    if( dia_semana == 6 ){ //tm_wday: Día desde el domingo (Sun)
        *dia = *dia + 2;
    }
    if(dia_semana == 0){
        *dia = *dia + 1;
    }
    printf("prueba_time3 %d %d %d\n", *dia, *mes, *anio);
}*/

/*
PARA COPIAR STRING: strdup
    char *bar = strdup(foo);
*/

void menu_principal()
{
    /*char * zona_ptv = NULL;
    zona_ptv = obtener_zona_punto_vacunacion("Soriano");
    printf("%s\n", zona_ptv);

    prueba_time();*/

    int op;

    grafo = crearGrafo(19);
    crear_grafo_BD(grafo);
    //imprimir_grafo(grafo);

    do{
        printf("***Bienvenido al plan de vacunacion contra COVID***\n");
        printf("1) Agenda paciente\n");
        printf("2) Gestion\n");
        printf("0) Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &op);
        fflush(stdin);
        switch(op)
        {
            case 1:
                system("cls");
                menu_agenda();
                break;
            case 2:
                system("cls");
                menu_gestion();
                break;
            case 0:
                printf("Saliendo del programa...\n");
                system("PAUSE");
                system("cls");
                break;
            default:
                printf("Opcion incorrecta\n");
                system("PAUSE");
                system("cls");
                break;
        }
        //printf("\nOp %d\n", op);
    }while(op!=0);

    printf("");

}

void menu_gestion()
{
    int op;

    do{
        printf("***Modulo de gestion***\n");
        printf("Seleccione el rol con el cual desea ingresar al sistema\n");
        printf("1) Administrador\n");
        printf("2) MSP\n");
        printf("3) SINAE\n");
        printf("0) Volver\n");
        printf("\n");
        printf("Ingrese opcion: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op)
        {
            case 1:
                //printf("Op 0\n");
                //ingresar();
                if(ingresar(0)==0){
                    system("cls");
                    menu_admin();
                }
                else{
                    printf("Usuario o contraseña incorrectos\n");
                    system("PAUSE");
                }
                break;
            case 2:
                //menu_msp();
                if(ingresar(1)==0){
                    system("cls");
                    menu_msp();
                }
                else{
                    printf("Usuario o contraseña incorrectos\n");
                    system("PAUSE");
                }
                break;
            case 3:
                //menu_siane();
                if(ingresar(2)==0){
                    system("cls");
                    menu_siane();
                }
                else{
                    printf("Usuario o contraseña incorrectos\n");
                    system("PAUSE");
                }
                break;
            /*case 4:
                printf("Se mostraran los puntos de vacunacion a visitar\n");
                printf("Ingrese SECOED: ");
                scanf("%s", &pto_vac_localidad);
                fflush(stdin);
                pto_vac_visitar = crearGrafo(19); imprimir_grafo(pto_vac_visitar); system("PAUSE");
                crear_grafo_visita_MSP(pto_vac_visitar, pto_vac_localidad);
                imprimir_grafo_MSP(pto_vac_visitar);
                break;*/
            case 0:
                system("cls");
                break;
            default:
                printf("Opcion incorrecta\n");
                system("PAUSE");
                system("cls");
                break;
        }
        system("cls");
    }while(op!=0);
    //menu_principal();
}

void menu_agenda()
{
    //imprimir_grafo(grafo);
    char ci[8], pri_nom[20], pri_ape[20], fech_nac[20], localidad[20];
    printf("CI (sin puntos ni guiones ni digito verificador): ");
    scanf("%s", &ci);
    printf("Nombre: ");
    scanf("%s", &pri_nom);
    printf("Apellido: ");
    scanf("%s", &pri_ape);
    printf("Fecha de nacimiento (YYYY-MM-DD): ");
    scanf("%s", &fech_nac);
    //fflush(stdin);
    printf("Localidad\n"); fflush(stdin);
    scanf("%[^\n]%*c", &localidad);
    //printf("ci %s pri_nom %s pri_ape %s fech_nac %s localidad %s\n", ci, pri_nom, pri_ape, fech_nac, localidad); system("PAUSE");

    inicializar_aux_visitado();

    alta_paciente(ci, pri_nom, pri_ape, fech_nac, localidad);
    agendar_dfs(ci, pri_nom, pri_ape, fech_nac, localidad, grafo);
    resetear_pudo_agendar();
}

int ingresar(int rol)
{
    char usu[50];
    char pass[50];

    printf("Ingreso nombre de usuario: ");
    scanf("%s", usu);
    fflush(stdin);
    printf("Ingreso pass: ");
    scanf("%s", pass);
    fflush(stdin);

    if(login(usu,pass,rol)==0)
        //administrador();
        return 1;
    else
        //printf(":( \n");
        return 0;

}

void menu_msp()
{
    int op;
    char pto_vac_localidad[50];

    do{
        printf("***Gestion - MSP***\n");
        printf("\n");
        printf("1) Ingreso de vacunas\n");
        printf("2) Listado de vacunas\n");
        printf("3) Informe\n");
        printf("0) Volver\n");
        printf("\n");
        printf("Ingrese opcion: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op)
        {
            case 1:
                menu_ingreso_vacunas();
                break;
            case 2:
                listar_todas_vac();
                break;
            case 3:
                printf("Se mostraran los puntos de vacunacion a visitar\n");
                printf("Ingrese SECOED: ");
                scanf("%s", &pto_vac_localidad);
                fflush(stdin);
                pto_vac_visitar = crearGrafo(19); //imprimir_grafo(pto_vac_visitar); system("PAUSE");
                crear_grafo_visita_MSP(pto_vac_visitar, pto_vac_localidad);
                imprimir_grafo_MSP(pto_vac_visitar);
                break;
            case 0:
                break;
            default:
                printf("Opcion incorrecta\n");
                system("PAUSE");
                system("cls");
                break;
        }
        system("PAUSE");
        system("cls");
    }while(op!=0);
}

void menu_ingreso_vacunas()
{
    char pto_vac[30];
    Lote_vacunas *v = malloc(sizeof(Lote_vacunas));
    //v = crear_lote_vacunas(lote,tipo_vac,lab,vencimiento,cant_dosis);


    if(alta_lote_vacunas(&v) == 0)
    {
        system("pause");
        imprimir_str_lote_vacunas(v);
        if(alta_vacuna_2(v) == 0)
            printf("\nSe cargaron correctamente los datos\n");
        else
            printf("\nError\n");

        listar_pto_vac_BD();
        printf("Ingrese punto de vacunacion destino para el lote %d: ", v->num_lote); fflush(stdin);
        scanf("%[^\n]%*c", &pto_vac);
        //scanf("%s",&pto_vac);
        fflush(stdin);
        printf("main %d %s", v->num_lote, pto_vac);
        alta_vacuna_tiene(v->num_lote, pto_vac);
    }
}

void menu_siane(){
    int op;
    char localidad[20], op_arch;

    do{
        printf("***SINAE***\n");
        printf("1) Dado pto vac muestra SECOED y vecinos\n");
        printf("2) Puntos de vacunacion y sus dosis\n");
        printf("3) Puntos de vacunacion con menos de 10 dosis\n");
        printf("0) Volver\n");
        printf("Ingrese opcion: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op)
        {
            case 1:
                printf("Ingrese punto de vacunacion: "); fflush(stdin);
                scanf("%[^\n]%*c", &localidad);
                //scanf("%s",&localidad);
                fflush(stdin);
                printf("Punto de vacunacion: %s\n", localidad);
                printf("Zona: %s\n", obtener_zona_punto_vacunacion(localidad));
                info_pto_vac(grafo, localidad);
                break;
            case 2:
                printf("Hay %d SECOED actualmente.\n", cant_secoed());
                cant_dosis_todos_pto_vac();
                break;
            case 3: //Esta opción también debe generar un txt
                printf("Los puntos de vacunacion con menos de 10 dosis son:\n");
                pto_vac_pocas_dosis();

                printf("Desea generar un archivo de texto (s/n): "); fflush(stdin);
                scanf("%c", &op_arch); fflush(stdin);
                if(op_arch == 's'){
                    int archivo;
                    archivo = guardar_archivo();
                }
                //printf("ARCHIVO %d\n", archivo);
                break;
            case 0:
                break;
            default:
                printf("Opcion incorrecta\n");
                break;
        }
        system("PAUSE");
        system("cls");
    }while(op!=0);
}

void administrador()
{
    printf("hola admin :)\n");
    printf("\n");
}

void menu_admin(){
    char nom_usu[20], pass[20], pri_nom[20], pri_ape[20];
    int op,rol;

    do{
        printf("***Administrador***\n");
        printf("1) Alta de usuario\n");
        printf("2) Baja de usuario\n");
        printf("3) Listado de usuarios\n");
        printf("0) Volver\n");
        printf("Ingrese opcion: ");
        scanf("%d", &op);
        fflush(stdin);

        switch(op)
        {
            case 1:
                printf("Nombre de usuario: ");
                scanf("%s",&nom_usu);
                fflush(stdin);
                printf("Password: ");
                scanf("%s",&pass);
                fflush(stdin);
                printf("Primer nombre: ");
                scanf("%s",&pri_nom);
                fflush(stdin);
                printf("Primer apellido: ");
                scanf("%s",&pri_ape);
                fflush(stdin);
                printf("Rol (1-MSP, 2-SINAE): ");
                scanf("%d",&rol);
                fflush(stdin);
                if(existe_usuario(nom_usu)==0)
                    if(alta_usuario(nom_usu, pass, pri_nom, pri_ape, rol))
                        printf("El usuario se ha creado correctamente\n");
                    else
                        printf("Error al crear el usuario\n");
                else
                    printf("El usuario ya existe\n");
                break;
            case 2:
                listar_usuario();
                printf("\nNombre de usuario: ");
                scanf("%s",&nom_usu);
                baja_usuario(nom_usu);
                break;
            case 3:
                listar_usuario();
                break;
            case 0:
                break;
            default:
                printf("Opcion incorrecta\n");
                break;
        }
        system("PAUSE");
        system("cls");
    }while(op!=0);
}
