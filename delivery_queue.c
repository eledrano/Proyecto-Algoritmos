/*
  Modulo: Cola de entregas pendientes (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez
  Ver contrato de interfaz documentado en delivery_queue.h
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delivery_queue.h"
#include "package_list.h"
#include "io_utils.h"

/* La pila de devoluciones expone esta funcion (declaracion para evitar ciclo de headers) */
extern int pushReturnedPackage(const char *code);

/* Instancia global unica de la cola */
static DeliveryQueue deliveryQueue;

/*
  Entradas: queue
  Salidas: ninguna
  Restricciones: debe llamarse antes de usar la cola
*/
void initQueue(DeliveryQueue *queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
}

/*
  Entradas: queue
  Salidas: 1 si esta vacia, 0 si no
*/
int isQueueEmpty(DeliveryQueue *queue) {
    return queue->front == NULL;
}

/*
  Entradas: queue, code
  Salidas: 1 si se encolo, 0 si el paquete no existe en el repositorio
  Restricciones: el paquete debe existir; al encolar pasa a Pendiente de entrega
*/
int enqueue(DeliveryQueue *queue, const char *code) {
    Package *pkg = findPackageByCode(code);
    if (pkg == NULL) {
        printf("Error: no existe un paquete con el codigo '%s'.\n", code);
        return 0;
    }
    QueueNode *node = (QueueNode *) malloc(sizeof(QueueNode));
    if (node == NULL) {
        printf("Error: no hay memoria disponible para encolar.\n");
        return 0;
    }
    strcpy(node->code, code);
    node->next = NULL;
    if (queue->rear == NULL) {
        queue->front = node;
        queue->rear = node;
    } else {
        queue->rear->next = node;
        queue->rear = node;
    }
    queue->count++;
    pkg->state = STATE_PENDING;   /* cambia el estado en el repositorio */
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
  Entradas: queue
  Salidas: ninguna
*/
void displayQueue(DeliveryQueue *queue) {
    if (isQueueEmpty(queue)) {
        printf("La cola de entregas esta vacia.\n");
        return;
    }
    printf("Paquetes en cola (del primero al ultimo):\n");
    QueueNode *current = queue->front;
    while (current != NULL) {
        printPackageByCode(current->code);
        current = current->next;
    }
}

/*
  Entradas: queue
  Salidas: ninguna (muestra el siguiente sin desencolar)
*/
void peekQueue(DeliveryQueue *queue) {
    if (isQueueEmpty(queue)) {
        printf("La cola de entregas esta vacia.\n");
        return;
    }
    printf("Siguiente paquete a entregar:\n");
    printPackageByCode(queue->front->code);
}

/* Desencola el frente y devuelve el codigo en 'codeOut'. 1 si habia algo, 0 si vacia */
static int dequeue(DeliveryQueue *queue, char *codeOut) {
    if (isQueueEmpty(queue)) {
        return 0;
    }
    QueueNode *node = queue->front;
    strcpy(codeOut, node->code);
    queue->front = node->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(node);
    queue->count--;
    return 1;
}

/*
  Entradas: queue
  Salidas: ninguna
  Restricciones: desencola el siguiente paquete y pregunta si la entrega fue
  exitosa. Si fue exitosa -> Entregado. Si fallo -> Devuelto y se apila.
*/
void processNextDelivery(DeliveryQueue *queue) {
    char code[PKG_CODE_LENGTH];
    if (!dequeue(queue, code)) {
        printf("La cola de entregas esta vacia. No hay nada que procesar.\n");
        return;
    }
    printf("Procesando entrega del paquete:\n");
    printPackageByCode(code);

    int success = readInt("La entrega fue exitosa? (1 = si, 0 = no): ");
    if (success == 1) {
        updatePackageState(code, STATE_DELIVERED);
        printf("Paquete '%s' marcado como Entregado.\n", code);
    } else {
        updatePackageState(code, STATE_RETURNED);
        pushReturnedPackage(code);   /* se envia a la pila de devoluciones */
        printf("Paquete '%s' marcado como Devuelto y enviado a la pila de devoluciones.\n", code);
    }
}

/* Libera todos los nodos de la cola */
void freeQueue(DeliveryQueue *queue) {
    QueueNode *current = queue->front;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
}

/*
  PERSISTENCIA
  Formato:
  QUEUE <cantidad>
  <code>   (un codigo por linea, en orden del frente al final)
*/
int saveQueueToFile(DeliveryQueue *queue, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir '%s' para guardar la cola.\n", filename);
        return 0;
    }
    fprintf(file, "QUEUE %d\n", queue->count);
    QueueNode *current = queue->front;
    while (current != NULL) {
        fprintf(file, "%s\n", current->code);
        current = current->next;
    }
    fclose(file);
    return 1;
}

int loadQueueFromFile(DeliveryQueue *queue, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro '%s'. Se inicia con la cola vacia.\n", filename);
        return 0;
    }
    initQueue(queue);
    int total = 0;
    if (fscanf(file, "QUEUE %d\n", &total) != 1) {
        printf("Error: formato invalido en '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    char code[PKG_CODE_LENGTH];
    for (int i = 0; i < total; i++) {
        if (fscanf(file, " %[^\n]\n", code) != 1) break;
        /* Encolado directo respetando el orden, sin re-cambiar el estado */
        QueueNode *node = (QueueNode *) malloc(sizeof(QueueNode));
        if (node == NULL) break;
        strcpy(node->code, code);
        node->next = NULL;
        if (queue->rear == NULL) {
            queue->front = node;
            queue->rear = node;
        } else {
            queue->rear->next = node;
            queue->rear = node;
        }
        queue->count++;
    }
    fclose(file);
    return 1;
}

/* Acceso a la instancia global */
DeliveryQueue *getDeliveryQueue(void) {
    return &deliveryQueue;
}

/* Contrato con otros modulos */
int enqueuePackage(const char *code) {
    return enqueue(&deliveryQueue, code);
}

int getTotalPendingPackages(void) {
    return deliveryQueue.count;
}

/*
  MENU INTERACTIVO
*/
void displayDeliveryQueueMenu(void) {
    int option;
    do {
        printf("\n========== COLA DE ENTREGAS PENDIENTES ==========\n");
        printf("1. Encolar un paquete registrado\n");
        printf("2. Mostrar todos los paquetes en cola\n");
        printf("3. Consultar el siguiente paquete (sin desencolar)\n");
        printf("4. Procesar la siguiente entrega\n");
        printf("5. Volver\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo del paquete a encolar: ", code, PKG_CODE_LENGTH);
                if (enqueue(&deliveryQueue, code)) {
                    printf("Paquete '%s' encolado y marcado como Pendiente de entrega.\n", code);
                }
                break;
            }
            case 2:
                displayQueue(&deliveryQueue);
                break;
            case 3:
                peekQueue(&deliveryQueue);
                break;
            case 4:
                processNextDelivery(&deliveryQueue);
                break;
            case 5:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 5);
}
