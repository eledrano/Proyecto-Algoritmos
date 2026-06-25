/*
  Modulo: Gestion de clientes mediante Arbol Binario de Busqueda (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Ejercicio 3.1. Los clientes se almacenan en un BST usando como llave la
  identificacion del cliente. Permite insertar sin duplicados, buscar por id,
  recorridos inorden/preorden/postorden y validar la existencia de un cliente
  (esta ultima la usa el modulo de paquetes para no asociar paquetes a
  clientes inexistentes).
*/

#ifndef CLIENT_BST_H
#define CLIENT_BST_H

#define CLIENT_ID_LENGTH 30
#define CLIENT_NAME_LENGTH 60
#define CLIENT_PHONE_LENGTH 20
#define CLIENT_EMAIL_LENGTH 60
#define CLIENT_ADDRESS_LENGTH 80

/*
  Estructura Client: datos solicitados para cada cliente.
  La llave del arbol es 'id'.
*/
typedef struct {
    char id[CLIENT_ID_LENGTH];
    char fullName[CLIENT_NAME_LENGTH];
    char phone[CLIENT_PHONE_LENGTH];
    char email[CLIENT_EMAIL_LENGTH];
    char address[CLIENT_ADDRESS_LENGTH];
} Client;

/* Nodo del BST */
typedef struct ClientNode {
    Client data;
    struct ClientNode *left;
    struct ClientNode *right;
} ClientNode;

/*
  Contenedor del arbol. Se usa un struct (en vez de solo el puntero raiz) para
  respetar la firma documentada en files.h: saveClientsToFile(ClientBST *tree,...)
*/
typedef struct {
    ClientNode *root;
    int count;
} ClientBST;

/* Inicializacion */
void initClientBST(ClientBST *tree);

/* Operaciones principales */
int insertClient(ClientBST *tree, Client client);          /* 1 ok, 0 si id duplicado */
ClientNode *findClient(ClientBST *tree, const char *id);   /* nodo o NULL */
int clientExistsInTree(ClientBST *tree, const char *id);   /* 1 si existe, 0 si no */

/* Recorridos */
void displayClientsInorder(ClientBST *tree);
void displayClientsPreorder(ClientBST *tree);
void displayClientsPostorder(ClientBST *tree);

/* Liberacion de memoria */
void freeClientBST(ClientBST *tree);

/* Persistencia en archivo de texto */
int saveClientsToFile(ClientBST *tree, const char *filename);
int loadClientsFromFile(ClientBST *tree, const char *filename);

/* Acceso a la instancia global del modulo (usada por menus y otros modulos) */
ClientBST *getClientTree(void);

/* Menu interactivo del modulo (lo invoca main.c) */
void displayClientMenu(void);

/* Funciones esperadas por otros modulos (contrato de interfaz) */
int clientExists(const char *id);   /* lo usa package_list para validar */
void displayClientsInOrder(void);   /* lo usa reports.c */

#endif
