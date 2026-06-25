/*
  Modulo: Cola de entregas pendientes (RoutePack)
*/

#ifndef DELIVERY_QUEUE_H
#define DELIVERY_QUEUE_H

#include "package_list.h"

typedef struct QueueNode {
    char code[PKG_CODE_LENGTH];
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int count;
} DeliveryQueue;

void initQueue(DeliveryQueue *queue);

/* Operaciones principales */
int enqueue(DeliveryQueue *queue, const char *code);/* cambia estado a Pendiente*/
int isQueueEmpty(DeliveryQueue *queue);
void displayQueue(DeliveryQueue *queue);
void peekQueue(DeliveryQueue *queue);/* siguiente sin desencolar*/
void processNextDelivery(DeliveryQueue *queue);/* desencola y resuelve entrega*/

/* Libera memoria*/
void freeQueue(DeliveryQueue *queue);

/* Persistencia en archivo de texto*/
int saveQueueToFile(DeliveryQueue *queue, const char *filename);
int loadQueueFromFile(DeliveryQueue *queue, const char *filename);

/* Acceso a la instancia global*/
DeliveryQueue *getDeliveryQueue(void);

/* Menu interactivo*/
void displayDeliveryQueueMenu(void);

int enqueuePackage(const char *code);
int getTotalPendingPackages(void);

#endif
