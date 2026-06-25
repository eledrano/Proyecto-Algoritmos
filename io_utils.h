/*
  Modulo: Utilidades de entrada/salida por consola (RoutePack)
*/

#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Descarta caracteres que quedan en el buffer hasta fin de linea*/
static void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
  Entradas: prompt
  Salidas: un entero validado digitado por el usuario
  Restricciones: insiste hasta recibir un entero valido
*/
static int readInt(const char *prompt) {
    char buffer[64];
    for (;;) {
        printf("%s", prompt);
        int result = scanf(" %63s", buffer);
        if (result == EOF) {
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
  Entradas: prompt
  Salidas: un numero real validado digitado por el usuario
  Restricciones: insiste hasta recibir un numero valido
*/
static float readFloat(const char *prompt) {
    char buffer[64];
    for (;;) {
        printf("%s", prompt);
        int result = scanf(" %63s", buffer);
        if (result == EOF) {
            printf("\nEntrada finalizada. Saliendo del sistema...\n")
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
