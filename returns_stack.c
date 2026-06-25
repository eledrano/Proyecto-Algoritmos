/*
  Modulo: Pila de paquetes devueltos (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez
  Ver contrato de interfaz documentado en returns_stack.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "returns_stack.h"
#include "package_list.h"
#include "io_utils.h"

/* La cola expone esta funcion (declaracion para evitar ciclo de headers) */
extern int enqueuePackage(const char *code);

/* Instancia global unica de la pila */
static ReturnsStack returnsStack;

/*
  Entradas: stack
  Salidas: ninguna
  Restricciones: debe llamarse antes de usar la pila
*/
void initStack(ReturnsStack *stack) {
    stack->top = NULL;
    stack->count = 0;
}

/*
  Entradas: stack
  Salidas: 1 si esta vacia, 0 si no
*/
int isStackEmpty(ReturnsStack *stack) {
    return stack->top == NULL;
}

/*
  Entradas: stack, code
  Salidas: 1 si se apilo, 0 si hubo error de memoria
  Restricciones: ninguna
*/
int pushReturn(ReturnsStack *stack, const char *code) {
    StackNode *node = (StackNode *) malloc(sizeof(StackNode));
    if (node == NULL) {
        printf("Error: no hay memoria disponible para apilar la devolucion.\n");
        return 0;
    }
    strcpy(node->code, code);
    node->next = stack->top;
    stack->top = node;
    stack->count++;
    return 1;
}

/* Imprime los datos del paquete asociado a un codigo, consultando el repositorio */
static void printPackageByCode(const char *code) {
    Package *pkg = findPackageByCode(code);
    if (pkg == NULL) {
        printf("  Codigo: %s (paquete no encontrado en el repositorio)\n", code);
    } else {
        printf("  Codigo: %s | Destinatario: %s | Destino: %s | Estado: %s\n",
               pkg->code, pkg->recipientName, pkg->destination, stateToString(pkg->state));
    }
}

/*
  Entradas: stack
  Salidas: ninguna
*/
void displayStack(ReturnsStack *stack) {
    if (isStackEmpty(stack)) {
        printf("La pila de devoluciones esta vacia.\n");
        return;
    }
    printf("Paquetes devueltos (del mas reciente al mas antiguo):\n");
    StackNode *current = stack->top;
    while (current != NULL) {
        printPackageByCode(current->code);
        current = current->next;
    }
}

/*
  Entradas: stack
  Salidas: ninguna (muestra el ultimo devuelto sin desapilar)
*/
void peekStack(ReturnsStack *stack) {
    if (isStackEmpty(stack)) {
        printf("La pila de devoluciones esta vacia.\n");
        return;
    }
    printf("Ultimo paquete devuelto:\n");
    printPackageByCode(stack->top->code);
}

/* Desapila el tope y devuelve el codigo en 'codeOut'. 1 si habia algo, 0 si vacia */
static int pop(ReturnsStack *stack, char *codeOut) {
    if (isStackEmpty(stack)) {
        return 0;
    }
    StackNode *node = stack->top;
    strcpy(codeOut, node->code);
    stack->top = node->next;
    free(node);
    stack->count--;
    return 1;
}

/*
  Entradas: stack
  Salidas: ninguna
  Restricciones: desapila el paquete mas reciente, lo reencola en la cola de
  entregas y su estado vuelve a Pendiente de entrega (lo hace enqueue).
*/
void reprocessTopReturn(ReturnsStack *stack) {
    char code[PKG_CODE_LENGTH];
    if (!pop(stack, code)) {
        printf("La pila de devoluciones esta vacia. No hay nada que reprocesar.\n");
        return;
    }
    printf("Reprocesando el paquete devuelto:\n");
    printPackageByCode(code);
    if (enqueuePackage(code)) {
        printf("Paquete '%s' reencolado y marcado como Pendiente de entrega.\n", code);
    } else {
        /* Si no se pudo reencolar, se vuelve a apilar para no perderlo */
        pushReturn(stack, code);
        printf("No se pudo reencolar el paquete; se mantiene en la pila.\n");
    }
}

/* Libera todos los nodos de la pila */
void freeStack(ReturnsStack *stack) {
    StackNode *current = stack->top;
    while (current != NULL) {
        StackNode *next = current->next;
        free(current);
        current = next;
    }
    stack->top = NULL;
    stack->count = 0;
}

/*
  PERSISTENCIA
  Formato:
  STACK <cantidad>
  <code>   (un codigo por linea; se guarda desde el tope hacia abajo)
*/
int saveStackToFile(ReturnsStack *stack, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir '%s' para guardar la pila.\n", filename);
        return 0;
    }
    fprintf(file, "STACK %d\n", stack->count);
    StackNode *current = stack->top;
    while (current != NULL) {
        fprintf(file, "%s\n", current->code);
        current = current->next;
    }
    fclose(file);
    return 1;
}

int loadStackFromFile(ReturnsStack *stack, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro '%s'. Se inicia con la pila vacia.\n", filename);
        return 0;
    }
    initStack(stack);
    int total = 0;
    if (fscanf(file, "STACK %d\n", &total) != 1) {
        printf("Error: formato invalido en '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    /* Se leen los codigos en orden de tope-a-fondo y se guardan en un arreglo
       temporal para reapilarlos en orden inverso y conservar el tope original */
    char codes[1024][PKG_CODE_LENGTH];
    int read = 0;
    for (int i = 0; i < total && read < 1024; i++) {
        if (fscanf(file, " %[^\n]\n", codes[read]) != 1) break;
        read++;
    }
    for (int i = read - 1; i >= 0; i--) {
        pushReturn(stack, codes[i]);
    }
    fclose(file);
    return 1;
}

/* Acceso a la instancia global */
ReturnsStack *getReturnsStack(void) {
    return &returnsStack;
}

/* Contrato con otros modulos */
int pushReturnedPackage(const char *code) {
    return pushReturn(&returnsStack, code);
}

int getTotalReturnedPackages(void) {
    return returnsStack.count;
}

/*
  MENU INTERACTIVO
*/
void displayReturnsStackMenu(void) {
    int option;
    do {
        printf("\n========== PILA DE PAQUETES DEVUELTOS ==========\n");
        printf("1. Mostrar paquetes devueltos\n");
        printf("2. Consultar el ultimo paquete devuelto\n");
        printf("3. Reprocesar el ultimo paquete devuelto (reencolar)\n");
        printf("4. Volver\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1:
                displayStack(&returnsStack);
                break;
            case 2:
                peekStack(&returnsStack);
                break;
            case 3:
                reprocessTopReturn(&returnsStack);
                break;
            case 4:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 4);
}
