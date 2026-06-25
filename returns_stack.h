/*
  Modulo: Pila de paquetes devueltos (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Ejercicio 3.4. Pila LIFO para las entregas fallidas. Guarda el codigo del
  paquete; los datos se consultan en el repositorio principal (package_list).
  Permite reprocesar la devolucion mas reciente: al desapilar se reencola el
  paquete en la cola de entregas y su estado vuelve a "Pendiente de entrega".
*/

#ifndef RETURNS_STACK_H
#define RETURNS_STACK_H

#include "package_list.h"   /* para PKG_CODE_LENGTH y los estados */

/* Nodo de la pila: guarda solo el codigo del paquete */
typedef struct StackNode {
    char code[PKG_CODE_LENGTH];
    struct StackNode *next;
} StackNode;

/* Contenedor de la pila (LIFO) */
typedef struct {
    StackNode *top;
    int count;
} ReturnsStack;

/* Inicializacion */
void initStack(ReturnsStack *stack);

/* Operaciones principales */
int pushReturn(ReturnsStack *stack, const char *code);
int isStackEmpty(ReturnsStack *stack);
void displayStack(ReturnsStack *stack);
void peekStack(ReturnsStack *stack);          /* ultimo devuelto sin desapilar */
void reprocessTopReturn(ReturnsStack *stack); /* desapila y reencola */

/* Liberacion de memoria */
void freeStack(ReturnsStack *stack);

/* Persistencia en archivo de texto */
int saveStackToFile(ReturnsStack *stack, const char *filename);
int loadStackFromFile(ReturnsStack *stack, const char *filename);

/* Acceso a la instancia global */
ReturnsStack *getReturnsStack(void);

/* Menu interactivo (lo invoca main.c) */
void displayReturnsStackMenu(void);

/* Contrato con otros modulos */
int pushReturnedPackage(const char *code);  /* usada por delivery_queue */
int getTotalReturnedPackages(void);          /* usada por reports.c */

#endif
