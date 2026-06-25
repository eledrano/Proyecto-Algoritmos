/*
  Modulo: Gestion de clientes mediante Arbol Binario de Busqueda (RoutePack)
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
*/
typedef struct {
    char id[CLIENT_ID_LENGTH];
    char fullName[CLIENT_NAME_LENGTH];
    char phone[CLIENT_PHONE_LENGTH];
    char email[CLIENT_EMAIL_LENGTH];
    char address[CLIENT_ADDRESS_LENGTH];
} Client;

typedef struct ClientNode {
    Client data;
    struct ClientNode *left;
    struct ClientNode *right;
} ClientNode;

/*
  Contenedor del arbol
*/
typedef struct {
    ClientNode *root;
    int count;
} ClientBST;

void initClientBST(ClientBST *tree);

/* Operaciones principales */
int insertClient(ClientBST *tree, Client client);          /* 1 bien, 0 si id está duplicado */
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

/* Menu interactivo del modulo */
void displayClientMenu(void);

int clientExists(const char *id);   /*package_list para validar */
void displayClientsInOrder(void);

#endif
