/*
   Modulo: Grafo de puntos y rutas (RoutePack)
   Luis Medrano Gonzalez / Tyrone Carranza Hernandez
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "routes_graph.h"
#include "files.h"

/*
INICIALIZACION
   Entradas: graph
   Salidas: ninguna
   Restricciones: debe llamarse antes de usar cualquier otra funcion del grafo
 */
void initGraph(Graph *graph) {
    graph->pointCount = 0;
    for (int i = 0; i < MAX_POINTS; i++) {
        for (int j = 0; j < MAX_POINTS; j++) {
            graph->adjacencyMatrix[i][j] = NO_CONNECTION;
        }
    }
}

/*
GESTION DE PUNTOS
   Entradas: graph, name
   Salidas: indice del punto en el arreglo, o -1 si no existe
   Restricciones: ninguna
 */
int findPointIndex(Graph *graph, const char *name) {
    for (int i = 0; i < graph->pointCount; i++) {
        if (strcmp(graph->pointNames[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

/*
   Entradas: graph, name
   Salidas: 1 si el punto existe, 0 si no
   Restricciones: ninguna
 */
int pointExists(Graph *graph, const char *name) {
    return findPointIndex(graph, name) != -1;
}

/*
   Entradas: graph, name
   Salidas: 1 si se agrego bien, 0 si ya existia o no hay espacio
   Restricciones: el grafo no puede exceder MAX_POINTS puntos y sin nombres duplicados
 */
int addPoint(Graph *graph, const char *name) {
    if (pointExists(graph, name)) {
        printf("Error: el punto '%s' ya existe.\n", name);
        return 0;
    }
    if (graph->pointCount >= MAX_POINTS) {
        printf("Error: se alcanzo el limite de puntos permitidos.\n");
        return 0;
    }
    strcpy(graph->pointNames[graph->pointCount], name);
    graph->pointCount++;
    return 1;
}

/*
   Entradas: graph
   Salidas: ninguna
   Restricciones: ninguna
 */
void displayPoints(Graph *graph) {
    if (graph->pointCount == 0) {
        printf("No hay puntos registrados.\n");
        return;
    }
    printf("Puntos disponibles:\n");
    for (int i = 0; i < graph->pointCount; i++) {
        printf("  %d. %s\n", i + 1, graph->pointNames[i]);
    }
}

/*
GESTION DE RUTAS
Entradas: graph, origin, destination, weight
Salidas: 1 si se agrego bien, 0 si algun punto no existe
Restricciones: origin y destination deben existir previamente y weight debe ser mayor que 0
 */
int addRoute(Graph *graph, const char *origin, const char *destination, int weight) {
    if (!pointExists(graph, origin) || !pointExists(graph, destination)) {
        printf("Error: no se puede crear la ruta, uno o ambos puntos no existen.\n");
        return 0;
    }
    if (weight <= 0) {
        printf("Error: el peso de la ruta debe ser mayor que 0.\n");
        return 0;
    }
    int originIndex = findPointIndex(graph, origin);
    int destinationIndex = findPointIndex(graph, destination);
    /* Grafo no dirigido: la ruta se registra en ambos sentidos */
    graph->adjacencyMatrix[originIndex][destinationIndex] = weight;
    graph->adjacencyMatrix[destinationIndex][originIndex] = weight;
    return 1;
}

/*
   Entradas: graph
   Salidas: ninguna
   Restricciones: ninguna
 */
void displayAdjacencyMatrix(Graph *graph) {
    if (graph->pointCount == 0) {
        printf("No hay puntos registrados.\n");
        return;
    }

    printf("\nMatriz de adyacencia:\n\n");
    printf("%15s", " ");
    for (int i = 0; i < graph->pointCount; i++) {
        printf("%10s", graph->pointNames[i]);
    }
    printf("\n");

    for (int i = 0; i < graph->pointCount; i++) {
        printf("%15s", graph->pointNames[i]);
        for (int j = 0; j < graph->pointCount; j++) {
            printf("%10d", graph->adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}

/*
DIJKSTRA - RUTA MAS CORTA
   Entradas: graph, originName, destinationName
   Salidas: ninguna
   Restricciones: originName y destinationName deben existir en el grafo
 */
void dijkstraShortestPath(Graph *graph, const char *originName, const char *destinationName) {
    int originIndex = findPointIndex(graph, originName);
    int destinationIndex = findPointIndex(graph, destinationName);
    if (originIndex == -1 || destinationIndex == -1) {
        printf("Error: el punto de origen o destino no existe en el grafo.\n");
        return;
    }
    int distance[MAX_POINTS];
    int visited[MAX_POINTS];
    int previous[MAX_POINTS];
    int n = graph->pointCount;

    for (int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
        visited[i] = 0;
        previous[i] = -1;
    }
    distance[originIndex] = 0;
    for (int count = 0; count < n - 1; count++) {
        /* Buscar el vertice no visitado en la menor distancia */
        int minDistance = INT_MAX;
        int currentVertex = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && distance[i] <= minDistance) {
                minDistance = distance[i];
                currentVertex = i;
            }
        }
        if (currentVertex == -1) break;
        visited[currentVertex] = 1;

        for (int neighbor = 0; neighbor < n; neighbor++) {
            int weight = graph->adjacencyMatrix[currentVertex][neighbor];
            if (weight != NO_CONNECTION && !visited[neighbor] &&
                distance[currentVertex] != INT_MAX &&
                distance[currentVertex] + weight < distance[neighbor]) {
                distance[neighbor] = distance[currentVertex] + weight;
                previous[neighbor] = currentVertex;
            }
        }
    }
    if (distance[destinationIndex] == INT_MAX) {
        printf("No existe una ruta disponible entre '%s' y '%s'.\n", originName, destinationName);
        return;
    }
    /* Reconstruir el camino desde destino hasta origen*/
    int path[MAX_POINTS];
    int pathLength = 0;
    int current = destinationIndex;
    while (current != -1) {
        path[pathLength++] = current;
        current = previous[current];
    }
    printf("\nRuta mas corta: ");
    for (int i = pathLength - 1; i >= 0; i--) {
        printf("%s", graph->pointNames[path[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\nDistancia total: %d km\n", distance[destinationIndex]);
}

/*
MENU INTERACTIVO DE GESTION DE PUNTOS Y RUTAS
   Entradas: graph
   Salidas: ninguna
   Restricciones: ninguna
 */
void displayRoutesMenu(Graph *graph) {
    int option;
    char name[MAX_NAME_LENGTH];
    char origin[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
    int weight;

    do {
        printf("\n========== GESTION DE PUNTOS Y RUTAS ==========\n");
        printf("1. Registrar punto\n");
        printf("2. Registrar ruta entre dos puntos\n");
        printf("3. Mostrar puntos disponibles\n");
        printf("4. Mostrar matriz de adyacencia\n");
        printf("5. Calcular ruta mas corta (Dijkstra)\n");
        printf("6. Guardar datos en archivo\n");
        printf("7. Volver\n");
        printf("Opcion: ");
        if (scanf("%d", &option) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            if (feof(stdin)) { printf("\nEntrada finalizada.\n"); return; }
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }

        switch (option) {
            case 1:
                printf("Nombre del nuevo punto: ");
                scanf(" %[^\n]", name);
                if (addPoint(graph, name)) {
                    printf("Punto '%s' registrado correctamente.\n", name);
                }
                break;
            case 2:
                displayPoints(graph);
                printf("Punto de origen: ");
                scanf(" %[^\n]", origin);
                printf("Punto de destino: ");
                scanf(" %[^\n]", destination);
                printf("Peso de la ruta (distancia o tiempo): ");
                scanf("%d", &weight);
                if (addRoute(graph, origin, destination, weight)) {
                    printf("Ruta registrada correctamente.\n");
                }
                break;
            case 3:
                displayPoints(graph);
                break;
            case 4:
                displayAdjacencyMatrix(graph);
                break;
            case 5:
                displayPoints(graph);
                printf("Punto de origen: ");
                scanf(" %[^\n]", origin);
                printf("Punto de destino: ");
                scanf(" %[^\n]", destination);
                dijkstraShortestPath(graph, origin, destination);
                break;
            case 6:
                if (saveRoutesToFile(graph, "routes_data.txt")) {
                    printf("Datos guardados correctamente en 'routes_data.txt'.\n");
                }
                break;
            case 7:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 7);
}
