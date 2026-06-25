/*
    Modulo: Persistencia en archivos de texto
 */

#include <stdio.h>
#include <string.h>
#include "files.h"

/*
    Entradas: graph, filename (nombre del archivo de texto donde se guarda)
    Salidas: 1 si se guardo correctamente, 0 si hubo un error al abrir el archivo
    Restricciones: ninguna
 */
int saveRoutesToFile(Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir el archivo '%s' para guardar.\n", filename);
        return 0;
    }
    fprintf(file, "POINTS %d\n", graph->pointCount);
    for (int i = 0; i < graph->pointCount; i++) {
        fprintf(file, "%s\n", graph->pointNames[i]);
    }
    /* Contar las rutas*/
    int routeCount = 0;
    for (int i = 0; i < graph->pointCount; i++) {
        for (int j = i + 1; j < graph->pointCount; j++) {
            if (graph->adjacencyMatrix[i][j] != NO_CONNECTION) {
                routeCount++;
            }
        }
    }
    fprintf(file, "ROUTES %d\n", routeCount);
    for (int i = 0; i < graph->pointCount; i++) {
        for (int j = i + 1; j < graph->pointCount; j++) {
            if (graph->adjacencyMatrix[i][j] != NO_CONNECTION) {
                fprintf(file, "%s,%s,%d\n",
                        graph->pointNames[i],
                        graph->pointNames[j],
                        graph->adjacencyMatrix[i][j]);
            }
        }
    }
    fclose(file);
    return 1;
}

/*
    Entradas: graph, filename (nombre del archivo de texto a cargar)
    Salidas: 1 si se cargo bien, 0 si el archivo no existe o es invalido
    Restricciones: el grafo debe haber sido inicializado previamente con initGraph
 */
int loadRoutesFromFile(Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro el archivo '%s'. Se inicia con un grafo vacio.\n", filename);
        return 0;
    }
    initGraph(graph);
    int pointTotal = 0;
    if (fscanf(file, "POINTS %d\n", &pointTotal) != 1) {
        printf("Error: formato invalido en el archivo '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    char nameBuffer[MAX_NAME_LENGTH];
    for (int i = 0; i < pointTotal; i++) {
        if (fgets(nameBuffer, MAX_NAME_LENGTH, file) == NULL) break;
        nameBuffer[strcspn(nameBuffer, "\n")] = '\0';
        addPoint(graph, nameBuffer);
    }
    int routeTotal = 0;
    if (fscanf(file, "ROUTES %d\n", &routeTotal) != 1) {
        printf("Error: formato invalido en la seccion de rutas del archivo '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    char origin[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
    int weight;
    for (int i = 0; i < routeTotal; i++) {
        if (fscanf(file, "%[^,],%[^,],%d\n", origin, destination, &weight) != 3) break;
        addRoute(graph, origin, destination, weight);
    }
    fclose(file);
    return 1;
}
