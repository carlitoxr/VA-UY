#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include <winsock.h>
#include <mysql.h>
#include <windows.h>

#define MYSQL_HOST "localhost"
#define MYSQL_PORT 33060
#define MYSQL_USER "admin"
#define MYSQL_PASS "1234"
#define BD_NOMBRE "va_uy_bd"

MYSQL* conectar();

#endif // CONEXION_H_INCLUDED
