/*
    Modulo: Grafo de puntos y rutas (RoutePack)
    Luis Medrano Gonzalez / Tyrone Carranza Hernandez

Representa el mapa de distribucion como un grafo ponderado no dirigido, usando matriz de adyacencia. Incluye el algoritmo de Dijkstra para
calcular la ruta mas corta entre dos puntos.
 */

#ifndef ROUTES_GRAPH_H
#define ROUTES_GRAPH_H

#define MAX_POINTS 50
#define MAX_NAME_LENGTH 50
#define NO_CONNECTION 0

/*
Estructura Graph
    pointNames: nombres de los puntos registrados (vertices del grafo)
    adjacencyMatrix: matriz de adyacencia con los pesos de cada ruta
                     (0 significa que no existe conexion directa)
    pointCount: cantidad de puntos actualmente registrados
 */
typedef struct {
    char pointNames[MAX_POINTS][MAX_NAME_LENGTH];
    int adjacencyMatrix[MAX_POINTS][MAX_POINTS];
    int pointCount;
} Graph;

/* Inicializacion */
void initGraph(Graph *graph);

/* Gestion de puntos */
int findPointIndex(Graph *graph, const char *name);
int pointExists(Graph *graph, const char *name);
int addPoint(Graph *graph, const char *name);
void displayPoints(Graph *graph);

/* Gestion de rutas */
int addRoute(Graph *graph, const char *origin, const char *destination, int weight);
void displayAdjacencyMatrix(Graph *graph);

/* Dijkstra */
void dijkstraShortestPath(Graph *graph, const char *originName, const char *destinationName);

/* Menu interactivo de gestion de puntos y rutas*/
void displayRoutesMenu(Graph *graph);

#endif
