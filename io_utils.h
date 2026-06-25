/*
  Modulo: Utilidades de entrada/salida por consola (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Funciones de lectura robusta desde teclado. Se definen como 'static' en el
  header para que cada modulo que lo incluya tenga su propia copia interna
  (sin conflictos de enlazado) y para centralizar la validacion de entradas.

  Cumple el requisito no funcional 3.9: la aplicacion no debe cerrarse de forma
  inesperada por errores del usuario al digitar datos.
*/

#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Descarta los caracteres que quedan en el buffer hasta el fin de linea */
static void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descartar */
    }
}

/*
  Entradas: prompt (texto a mostrar)
  Salidas: un entero validado digitado por el usuario
  Restricciones: insiste hasta recibir un entero valido
*/
static int readInt(const char *prompt) {
    char buffer[64];
    for (;;) {
        printf("%s", prompt);
        int result = scanf(" %63s", buffer);
        if (result == EOF) {
            /* Fin de la entrada (por ejemplo Ctrl+D): se cierra ordenadamente */
            printf("\nEntrada finalizada. Saliendo del sistema...\n");
            exit(0);
        }
        if (result == 1) {
            char *end;
            long value = strtol(buffer, &end, 10);
            if (end != buffer && *end == '\0') {
                return (int) value;
            }
        }
        printf("Entrada invalida. Debe digitar un numero entero.\n");
    }
}

/*
  Entradas: prompt (texto a mostrar)
  Salidas: un numero real validado digitado por el usuario
  Restricciones: insiste hasta recibir un numero valido
*/
static float readFloat(const char *prompt) {
    char buffer[64];
    for (;;) {
        printf("%s", prompt);
        int result = scanf(" %63s", buffer);
        if (result == EOF) {
            /* Fin de la entrada (por ejemplo Ctrl+D): se cierra ordenadamente */
            printf("\nEntrada finalizada. Saliendo del sistema...\n");
            exit(0);
        }
        if (result == 1) {
            char *end;
            float value = strtof(buffer, &end);
            if (end != buffer && *end == '\0') {
                return value;
            }
        }
        printf("Entrada invalida. Debe digitar un numero.\n");
    }
}

/*
  Entradas: prompt, destino (buffer), size (capacidad del buffer)
  Salidas: ninguna (escribe el texto leido en 'destination')
  Restricciones: lee una linea completa (admite espacios) y la recorta a size-1
*/
static void readText(const char *prompt, char *destination, int size) {
    char temp[256];
    printf("%s", prompt);
    if (scanf(" %255[^\n]", temp) != 1) {
        temp[0] = '\0';
    }
    strncpy(destination, temp, size - 1);
    destination[size - 1] = '\0';
}

#endif
