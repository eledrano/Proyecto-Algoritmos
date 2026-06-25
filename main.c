/*
    Sistema de gestion de entregas urbanas - RoutePack
    Integrantes:
    Luis Orlando Medrano Gonzalez
    Tyrone Carranza Hernandez

Menu principal del sistema. Integra todos los modulos del proyecto: clientes
(BST), paquetes (lista enlazada), cola de entregas, pila de devoluciones,
paquetes por codigo (AVL), grafo de rutas y Dijkstra, y reportes del sistema.

La persistencia se maneja con un archivo de texto por estructura. Al iniciar se
cargan todos los datos y al salir se guardan y se libera la memoria dinamica.
*/

#include <stdio.h>
#include "io_utils.h"
#include "client_bst.h"
#include "package_list.h"
#include "delivery_queue.h"
#include "returns_stack.h"
#include "package_avl.h"
#include "routes_graph.h"
#include "files.h"
#include "reports.h"

/* Nombres de los archivos de persistencia */
#define CLIENTS_FILE  "clients_data.txt"
#define PACKAGES_FILE "packages_data.txt"
#define QUEUE_FILE    "queue_data.txt"
#define STACK_FILE    "stack_data.txt"
#define AVL_FILE      "avl_data.txt"
#define ROUTES_FILE   "routes_data.txt"

/* Carga todos los datos del sistema desde sus archivos de texto */
static void loadAllData(Graph *graph) {
    /* Se inicializa cada estructura antes de cargar, por si el archivo no existe */
    initClientBST(getClientTree());
    initPackageList(getPackageList());
    initQueue(getDeliveryQueue());
    initStack(getReturnsStack());
    *getAVLRootRef() = NULL;
    initGraph(graph);

    /* El orden importa: la cola y la pila referencian paquetes del repositorio */
    loadClientsFromFile(getClientTree(), CLIENTS_FILE);
    loadPackagesFromFile(getPackageList(), PACKAGES_FILE);
    loadQueueFromFile(getDeliveryQueue(), QUEUE_FILE);
    loadStackFromFile(getReturnsStack(), STACK_FILE);
    loadPackagesAVLFromFile(getAVLRootRef(), AVL_FILE);
    loadRoutesFromFile(graph, ROUTES_FILE);
}

/* Guarda todos los datos del sistema en sus archivos de texto */
static void saveAllData(Graph *graph) {
    saveClientsToFile(getClientTree(), CLIENTS_FILE);
    savePackagesToFile(getPackageList(), PACKAGES_FILE);
    saveQueueToFile(getDeliveryQueue(), QUEUE_FILE);
    saveStackToFile(getReturnsStack(), STACK_FILE);
    savePackagesAVLToFile(getAVLRoot(), AVL_FILE);
    saveRoutesToFile(graph, ROUTES_FILE);
}

/* Libera toda la memoria dinamica reservada por el sistema */
static void freeAllData(void) {
    freeClientBST(getClientTree());
    freePackageList(getPackageList());
    freeQueue(getDeliveryQueue());
    freeStack(getReturnsStack());
    freeAVL(getAVLRoot());
}

int main(void) {
    Graph cityGraph;
    int option;

    loadAllData(&cityGraph);

    do {
        printf("\n================ ROUTEPACK ================\n");
        printf("1. Gestionar clientes (BST)\n");
        printf("2. Gestionar paquetes registrados (Lista)\n");
        printf("3. Cola de entregas pendientes\n");
        printf("4. Pila de paquetes devueltos\n");
        printf("5. Gestionar paquetes por codigo (AVL)\n");
        printf("6. Gestion de puntos y rutas (Grafo)\n");
        printf("7. Reportes del sistema\n");
        printf("8. Salir\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1:
                displayClientMenu();
                break;
            case 2:
                displayPackageListMenu();
                break;
            case 3:
                displayDeliveryQueueMenu();
                break;
            case 4:
                displayReturnsStackMenu();
                break;
            case 5:
                displayPackageAVLMenu();
                break;
            case 6:
                displayRoutesMenu(&cityGraph);
                break;
            case 7:
                displayReportsMenu(&cityGraph);
                break;
            case 8:
                saveAllData(&cityGraph);
                printf("Datos guardados. Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 8);

    freeAllData();
    return 0;
}
