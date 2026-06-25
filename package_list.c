/*
  Modulo: Gestion de paquetes registrados mediante Lista Enlazada (RoutePack)

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package_list.h"
#include "client_bst.h"
#include "io_utils.h"

/* Instancia global*/
static PackageList masterList;

/*
  Entradas: list
  Salidas: ninguna
  Restricciones: debe llamarse antes de usar cualquier otra funcion
*/
void initPackageList(PackageList *list) {
    list->head = NULL;
    list->count = 0;
}

/*
  Entradas: state
  Salidas: etiqueta legible del estado
  Restricciones: ninguna
*/
const char *stateToString(PackageState state) {
    switch (state) {
        case STATE_REGISTERED: return "Registrado";
        case STATE_PENDING:    return "Pendiente de entrega";
        case STATE_IN_ROUTE:   return "En ruta";
        case STATE_DELIVERED:  return "Entregado";
        case STATE_RETURNED:   return "Devuelto";
        default:               return "Desconocido";
    }
}

/*
  Entradas: list, pkg
  Salidas: 1 si se agrego, 0 si el codigo ya existe o el cliente no existe
  Restricciones: no se permiten codigos duplicados y el cliente debe existir
*/
int addPackage(PackageList *list, Package pkg) {
    if (findPackageByCodeInList(list, pkg.code) != NULL) {
        printf("Error: ya existe un paquete con el codigo '%s'.\n", pkg.code);
        return 0;
    }
    if (!clientExists(pkg.clientId)) {
        printf("Error: no se puede registrar el paquete, el cliente '%s' no existe.\n", pkg.clientId);
        return 0;
    }
    PackageNode *node = (PackageNode *) malloc(sizeof(PackageNode));
    if (node == NULL) {
        printf("Error: no hay memoria disponible para registrar el paquete.\n");
        return 0;
    }
    node->data = pkg;
    node->next = list->head;
    list->head = node;
    list->count++;
    return 1;
}

/*
  Entradas: list, code
  Salidas: puntero al paquete dentro de la lista, o NULL
  Restricciones: ninguna
*/
Package *findPackageByCodeInList(PackageList *list, const char *code) {
    PackageNode *current = list->head;
    while (current != NULL) {
        if (strcmp(current->data.code, code) == 0) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;
}

/*
  Entradas: list, code, newState
  Salidas: 1 si se actualizo, 0 si el paquete no existe
  Restricciones: ninguna
*/
int updatePackageStateInList(PackageList *list, const char *code, PackageState newState) {
    Package *pkg = findPackageByCodeInList(list, code);
    if (pkg == NULL) {
        return 0;
    }
    pkg->state = newState;
    return 1;
}

/*
  Entradas: list, code
  Salidas: 1 si se elimino, 0 si no se encontro
  Restricciones: libera la memoria del nodo eliminado
*/
int removePackage(PackageList *list, const char *code) {
    PackageNode *current = list->head;
    PackageNode *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->data.code, code) == 0) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            list->count--;
            return 1;
        }
        previous = current;
        current = current->next;
    }
    return 0;
}

/* Imprime un paquete con todos sus datos */
static void printPackage(const Package *p) {
    printf("  Codigo: %s | Cliente: %s | Destinatario: %s | Destino: %s | "
           "Peso: %.2f | Prioridad: %d | Estado: %s\n",
           p->code, p->clientId, p->recipientName, p->destination,
           p->weight, p->priority, stateToString(p->state));
}

/*
  Entradas: list
  Salidas: ninguna
  Restricciones: ninguna
*/
void displayPackages(PackageList *list) {
    if (list->head == NULL) {
        printf("No hay paquetes registrados.\n");
        return;
    }
    PackageNode *current = list->head;
    while (current != NULL) {
        printPackage(&current->data);
        current = current->next;
    }
}

/*
  Entradas: list, state
  Salidas: cantidad de paquetes con ese estado
  Restricciones: ninguna
*/
int countByState(PackageList *list, PackageState state) {
    int total = 0;
    PackageNode *current = list->head;
    while (current != NULL) {
        if (current->data.state == state) {
            total++;
        }
        current = current->next;
    }
    return total;
}

/* Libera los nodos de la lista*/
void freePackageList(PackageList *list) {
    PackageNode *current = list->head;
    while (current != NULL) {
        PackageNode *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
    list->count = 0;
}
int savePackagesToFile(PackageList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir '%s' para guardar paquetes.\n", filename);
        return 0;
    }
    fprintf(file, "PACKAGES %d\n", list->count);
    PackageNode *current = list->head;
    while (current != NULL) {
        Package *p = &current->data;
        fprintf(file, "%s|%s|%s|%s|%.2f|%d|%d\n",
                p->code, p->clientId, p->recipientName, p->destination,
                p->weight, p->priority, (int) p->state);
        current = current->next;
    }
    fclose(file);
    return 1;
}

int loadPackagesFromFile(PackageList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro '%s'. Se inicia sin paquetes.\n", filename);
        return 0;
    }
    initPackageList(list);
    int total = 0;
    if (fscanf(file, "PACKAGES %d\n", &total) != 1) {
        printf("Error: formato invalido en '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    for (int i = 0; i < total; i++) {
        Package p;
        int stateInt;
        if (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%f|%d|%d\n",
                   p.code, p.clientId, p.recipientName, p.destination,
                   &p.weight, &p.priority, &stateInt) != 7) {
            break;
        }
        p.state = (PackageState) stateInt;
        /* Insercion directa*/
        PackageNode *node = (PackageNode *) malloc(sizeof(PackageNode));
        if (node == NULL) break;
        node->data = p;
        node->next = list->head;
        list->head = node;
        list->count++;
    }
    fclose(file);
    return 1;
}

/* Acceso a la instancia global */
PackageList *getPackageList(void) {
    return &masterList;
}

Package *findPackageByCode(const char *code) {
    return findPackageByCodeInList(&masterList, code);
}
int updatePackageState(const char *code, PackageState state) {
    return updatePackageStateInList(&masterList, code, state);
}
int getTotalRegisteredPackages(void) {
    return masterList.count;
}
int getTotalDeliveredPackages(void) {
    return countByState(&masterList, STATE_DELIVERED);
}

/*
  MENU INTERACTIVO
*/
void displayPackageListMenu(void) {
    int option;
    do {
        printf("\n========== GESTION DE PAQUETES (LISTA) ==========\n");
        printf("1. Registrar paquete\n");
        printf("2. Mostrar todos los paquetes\n");
        printf("3. Buscar paquete por codigo\n");
        printf("4. Modificar estado de un paquete\n");
        printf("5. Eliminar paquete\n");
        printf("6. Volver\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1: {
                Package p;
                readText("Codigo unico: ", p.code, PKG_CODE_LENGTH);
                if (findPackageByCodeInList(&masterList, p.code) != NULL) {
                    printf("Error: ya existe un paquete con el codigo '%s'.\n", p.code);
                    break;
                }
                readText("Identificacion del cliente asociado: ", p.clientId, PKG_CLIENT_ID_LENGTH);
                readText("Nombre del destinatario: ", p.recipientName, PKG_NAME_LENGTH);
                readText("Destino o punto de entrega: ", p.destination, PKG_DEST_LENGTH);
                p.weight = readFloat("Peso: ");
                p.priority = readInt("Prioridad (numero): ");
                p.state = STATE_REGISTERED;
                if (addPackage(&masterList, p)) {
                    printf("Paquete '%s' registrado correctamente.\n", p.code);
                }
                break;
            }
            case 2:
                displayPackages(&masterList);
                break;
            case 3: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo a buscar: ", code, PKG_CODE_LENGTH);
                Package *p = findPackageByCodeInList(&masterList, code);
                if (p == NULL) {
                    printf("No se encontro un paquete con el codigo '%s'.\n", code);
                } else {
                    printf("Paquete encontrado:\n");
                    printPackage(p);
                }
                break;
            }
            case 4: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo del paquete: ", code, PKG_CODE_LENGTH);
                Package *p = findPackageByCodeInList(&masterList, code);
                if (p == NULL) {
                    printf("No se encontro un paquete con el codigo '%s'.\n", code);
                    break;
                }
                printf("Estados: 0=Registrado 1=Pendiente 2=En ruta 3=Entregado 4=Devuelto\n");
                int s = readInt("Nuevo estado: ");
                if (s < 0 || s > 4) {
                    printf("Estado invalido.\n");
                    break;
                }
                p->state = (PackageState) s;
                printf("Estado actualizado a '%s'.\n", stateToString(p->state));
                break;
            }
            case 5: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo del paquete a eliminar: ", code, PKG_CODE_LENGTH);
                if (removePackage(&masterList, code)) {
                    printf("Paquete '%s' eliminado.\n", code);
                } else {
                    printf("No se encontro un paquete con el codigo '%s'.\n", code);
                }
                break;
            }
            case 6:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 6);
}
