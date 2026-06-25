/*
  Modulo: Persistencia en archivos de texto (RoutePack)
  
  Centraliza la carga y guardado de los datos del sistema en archivos de texto

===========================================================
  CONTRATO DE INTERFAZ CON OTROS MODULOS DEL EQUIPO
===========================================================
Cada estructura del proyecto agrega sus propias funciones de persistencia, siguiendo el mismo patron usado
para las rutas (un archivo de texto por estructura, con funciones save/load independientes)

int saveClientsToFile(ClientBST *tree, const char *filename);    -> client_bst
int loadClientsFromFile(ClientBST *tree, const char *filename);  -> client_bst
int savePackagesToFile(PackageList *list, const char *filename); -> package_list
int loadPackagesFromFile(PackageList *list, const char *filename); -> package_list
int saveQueueToFile(DeliveryQueue *queue, const char *filename); -> delivery_queue
int loadQueueFromFile(DeliveryQueue *queue, const char *filename); -> delivery_queue
int saveStackToFile(ReturnsStack *stack, const char *filename);  -> returns_stack
int loadStackFromFile(ReturnsStack *stack, const char *filename); -> returns_stack
int savePackagesAVLToFile(AVLNode *root, const char *filename);  -> package_avl
int loadPackagesAVLFromFile(AVLNode **root, const char *filename); -> package_avl

Implementadas en este archivo (responsabilidad propia del grafo de rutas):
===========================================================
*/

#ifndef FILES_H
#define FILES_H
#include "routes_graph.h"

/*
  Entradas: graph, filename
  Salidas: 1 si se guardo bien, 0 si hubo un error
  Restricciones: ninguna
 */
int saveRoutesToFile(Graph *graph, const char *filename);

/*
  Entradas: graph, filename
  Salidas: 1 si se cargo bien, 0 si el archivo no existe o es invalido
  Restricciones: el grafo debe haber sido inicializado previamente con initGraph
 */
int loadRoutesFromFile(Graph *graph, const char *filename);

#endif
