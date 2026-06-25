/*
    Modulo: Reportes del sistema
    Luis Medrano Gonzalez / Tyrone Carranza Hernandez
    
===========================================================
    CONTRATO DE INTERFAZ CON OTROS MODULOS DEL EQUIPO
===========================================================

int  getTotalRegisteredPackages(void);   -> package_list.c
int  getTotalDeliveredPackages(void);    -> package_list.c
int  getTotalPendingPackages(void);      -> delivery_queue.c
int  getTotalReturnedPackages(void);     -> returns_stack.c
void displayClientsInOrder(void);        -> client_bst.c
void displayPackagesSortedByCode(void);  -> package_avl.c
===========================================================
 */

#include <stdio.h>
#include <string.h>
#include "reports.h"

/* Funciones esperadas de los modulos del equipo*/
extern int getTotalRegisteredPackages(void);
extern int getTotalDeliveredPackages(void);
extern int getTotalPendingPackages(void);
extern int getTotalReturnedPackages(void);
extern void displayClientsInOrder(void);
extern void displayPackagesSortedByCode(void);

/*
    Entradas: graph
    Salidas: ninguna
    Restricciones: el usuario debe ingresar nombres de puntos existentes en el grafo
 */
static void displayShortestRouteReport(Graph *graph) {
    char origin[MAX_NAME_LENGTH];
    char destination[MAX_NAME_LENGTH];
    displayPoints(graph);
    printf("\nIngrese el punto de origen: ");
    scanf(" %[^\n]", origin);
    printf("Ingrese el punto de destino: ");
    scanf(" %[^\n]", destination);

    dijkstraShortestPath(graph, origin, destination);
}

/*
    Entradas: graph
    Salidas: ninguna
    Restricciones: ninguna
 */
void displayReportsMenu(Graph *graph) {
    int option;
    do {
        printf("\n========== MENU DE REPORTES ==========\n");
        printf("1. Total de paquetes registrados\n");
        printf("2. Total de paquetes entregados\n");
        printf("3. Total de paquetes pendientes\n");
        printf("4. Total de paquetes devueltos\n");
        printf("5. Clientes registrados en el BST\n");
        printf("6. Paquetes ordenados por codigo (AVL)\n");
        printf("7. Ruta mas corta entre dos puntos\n");
        printf("8. Volver\n");
        printf("Opcion: ");
        if (scanf("%d", &option) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            if (feof(stdin)) { printf("\nEntrada finalizada.\n"); return; }
            printf("Entrada invalida. Intente de nuevo.\n");
            continue;
        }

        switch (option) {
            case 1:
                printf("Total de paquetes registrados: %d\n", getTotalRegisteredPackages());
                break;
            case 2:
                printf("Total de paquetes entregados: %d\n", getTotalDeliveredPackages());
                break;
            case 3:
                printf("Total de paquetes pendientes: %d\n", getTotalPendingPackages());
                break;
            case 4:
                printf("Total de paquetes devueltos: %d\n", getTotalReturnedPackages());
                break;
            case 5:
                printf("Clientes registrados (BST en orden):\n");
                displayClientsInOrder();
                break;
            case 6:
                printf("Paquetes ordenados por codigo (AVL):\n");
                displayPackagesSortedByCode();
                break;
            case 7:
                displayShortestRouteReport(graph);
                break;
            case 8:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 8);
}
