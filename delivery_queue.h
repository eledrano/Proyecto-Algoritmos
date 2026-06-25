/*
  Modulo: Cola de entregas pendientes (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Ejercicio 3.3. Cola FIFO de paquetes listos para entregar. Almacena el codigo
  del paquete; los datos completos se consultan en el repositorio principal
  (package_list). Al encolar, el estado del paquete pasa a "Pendiente de
  entrega". Al procesar una entrega, si falla, el paquete pasa a "Devuelto" y se
  apila en la pila de devoluciones (returns_stack).
*/

#ifndef DELIVERY_QUEUE_H
#define DELIVERY_QUEUE_H

#include "package_list.h"   /* para PKG_CODE_LENGTH y los estados */

/* Nodo de la cola: guarda solo el codigo del paquete */
typedef struct QueueNode {
    char code[PKG_CODE_LENGTH];
    struct QueueNode *next;
} QueueNode;

/* Contenedor de la cola (FIFO) */
typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;
} DeliveryQueue;

/* Inicializacion */
void initQueue(DeliveryQueue *queue);

/* Operaciones principales */
int enqueue(DeliveryQueue *queue, const char *code);  /* cambia estado a Pendiente */
int isQueueEmpty(DeliveryQueue *queue);
void displayQueue(DeliveryQueue *queue);
void peekQueue(DeliveryQueue *queue);                 /* siguiente sin desencolar */
void processNextDelivery(DeliveryQueue *queue);       /* desencola y resuelve entrega */

/* Liberacion de memoria */
void freeQueue(DeliveryQueue *queue);

/* Persistencia en archivo de texto */
int saveQueueToFile(DeliveryQueue *queue, const char *filename);
int loadQueueFromFile(DeliveryQueue *queue, const char *filename);

/* Acceso a la instancia global */
DeliveryQueue *getDeliveryQueue(void);

/* Menu interactivo (lo invoca main.c) */
void displayDeliveryQueueMenu(void);

/* Contrato con otros modulos */
int enqueuePackage(const char *code);   /* usada por returns_stack para reencolar */
int getTotalPendingPackages(void);       /* usada por reports.c */

#endif
