/*
  Modulo: Pila de paquetes devueltos (RoutePack)
*/

#ifndef RETURNS_STACK_H
#define RETURNS_STACK_H

#include "package_list.h"

typedef struct StackNode {
    char code[PKG_CODE_LENGTH];
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int count;
} ReturnsStack;

/* Inicializacion*/
void initStack(ReturnsStack *stack);

/* Operaciones principales*/
int pushReturn(ReturnsStack *stack, const char *code);
int isStackEmpty(ReturnsStack *stack);
void displayStack(ReturnsStack *stack);
void peekStack(ReturnsStack *stack); 
void reprocessTopReturn(ReturnsStack *stack);

/* Libera memoria*/
void freeStack(ReturnsStack *stack);

/* Persistencia en archivo de texto*/
int saveStackToFile(ReturnsStack *stack, const char *filename);
int loadStackFromFile(ReturnsStack *stack, const char *filename);

/* Acceso a la instancia global*/
ReturnsStack *getReturnsStack(void);

/* Menu interactivo*/
void displayReturnsStackMenu(void);
int pushReturnedPackage(const char *code);
int getTotalReturnedPackages(void);

#endif
