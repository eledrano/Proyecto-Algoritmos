/*
  Modulo: Gestion de paquetes por codigo mediante Arbol AVL (RoutePack)
*/

#ifndef PACKAGE_AVL_H
#define PACKAGE_AVL_H

#include "package_list.h"

/*guarda una copia del paquete y la altura*/
typedef struct AVLNode {
    Package data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *insertAVL(AVLNode *root, Package pkg);
AVLNode *searchAVL(AVLNode *root, const char *code);
int avlHeight(AVLNode *node);
int avlBalanceFactor(AVLNode *node);
void displayAVLInorder(AVLNode *root);

/* Libera de memoria */
void freeAVL(AVLNode *root);

/* Persistencia en archivo de texto*/
int savePackagesAVLToFile(AVLNode *root, const char *filename);
int loadPackagesAVLFromFile(AVLNode **root, const char *filename);

/* Acceso a la raiz global del modulo*/
AVLNode *getAVLRoot(void);
AVLNode **getAVLRootRef(void);

/* Menu interactivo*/
void displayPackageAVLMenu(void);
void displayPackagesSortedByCode(void);

#endif
