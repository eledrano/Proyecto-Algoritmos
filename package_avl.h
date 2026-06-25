/*
  Modulo: Gestion de paquetes por codigo mediante Arbol AVL (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Ejercicio 3.5. Arbol AVL que usa el codigo del paquete como llave, para
  busquedas eficientes y balanceadas. Implementa altura, factor de balance,
  rotaciones simples (izquierda/derecha) y dobles (izq-der y der-izq). El arbol
  permanece balanceado luego de cada insercion.

  El AVL funciona como un indice ordenado de paquetes: al insertar se toma una
  copia del paquete que ya existe en el repositorio principal (package_list).
*/

#ifndef PACKAGE_AVL_H
#define PACKAGE_AVL_H

#include "package_list.h"   /* para el tipo Package */

/* Nodo del AVL: guarda una copia del paquete y la altura del subarbol */
typedef struct AVLNode {
    Package data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

/* Operaciones del arbol (reciben/retornan la raiz) */
AVLNode *insertAVL(AVLNode *root, Package pkg);
AVLNode *searchAVL(AVLNode *root, const char *code);
int avlHeight(AVLNode *node);
int avlBalanceFactor(AVLNode *node);
void displayAVLInorder(AVLNode *root);

/* Liberacion de memoria */
void freeAVL(AVLNode *root);

/* Persistencia en archivo de texto */
int savePackagesAVLToFile(AVLNode *root, const char *filename);
int loadPackagesAVLFromFile(AVLNode **root, const char *filename);

/* Acceso a la raiz global del modulo */
AVLNode *getAVLRoot(void);
AVLNode **getAVLRootRef(void);

/* Menu interactivo (lo invoca main.c) */
void displayPackageAVLMenu(void);

/* Contrato con reports.c */
void displayPackagesSortedByCode(void);

#endif
