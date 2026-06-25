/*
    Modulo: Grafo de puntos y rutas (RoutePack)
 */

#ifndef ROUTES_GRAPH_H
#define ROUTES_GRAPH_H

#define MAX_POINTS 50
#define MAX_NAME_LENGTH 50
#define NO_CONNECTION 0

typedef struct {
    char pointNames[MAX_POINTS][MAX_NAME_LENGTH];
    int adjacencyMatrix[MAX_POINTS][MAX_POINTS];
    int pointCount;
} Graph;

/* Inicializacion*/
void initGraph(Graph *graph);
/* Gestion de puntos*/
int findPointIndex(Graph *graph, const char *name);
int pointExists(Graph *graph, const char *name);
int addPoint(Graph *graph, const char *name);
void displayPoints(Graph *graph);

/* Gestion de rutas*/
int addRoute(Graph *graph, const char *origin, const char *destination, int weight);
void displayAdjacencyMatrix(Graph *graph);

/* Dijkstra*/
void dijkstraShortestPath(Graph *graph, const char *originName, const char *destinationName);

/* Menu interactivo de gestion de puntos y rutas*/
void displayRoutesMenu(Graph *graph);

#endif
