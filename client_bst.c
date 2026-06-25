/*
  Modulo: Gestion de clientes mediante Arbol Binario de Busqueda (RoutePack)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client_bst.h"
#include "io_utils.h"

static ClientBST clientTree;

/*
  Entradas: tree
  Salidas: ninguna
  Restricciones: debe llamarse antes de usar cualquier otra funcion del arbol
*/
void initClientBST(ClientBST *tree) {
    tree->root = NULL;
    tree->count = 0;
}

/* Crea un nodo nuevo con los datos del cliente */
static ClientNode *createClientNode(Client client) {
    ClientNode *node = (ClientNode *) malloc(sizeof(ClientNode));
    if (node == NULL) {
        printf("Error: no hay memoria disponible para registrar el cliente.\n");
        return NULL;
    }
    node->data = client;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Insercion recursiva. Devuelve la raiz del subarbol y avisa si hubo duplicado */
static ClientNode *insertClientNode(ClientNode *node, Client client, int *inserted) {
    if (node == NULL) {
        ClientNode *newNode = createClientNode(client);
        if (newNode != NULL) {
            *inserted = 1;
        }
        return newNode;
    }
    int cmp = strcmp(client.id, node->data.id);
    if (cmp < 0) {
        node->left = insertClientNode(node->left, client, inserted);
    } else if (cmp > 0) {
        node->right = insertClientNode(node->right, client, inserted);
    } else {
        /* Llave duplicada no se inserta */
        *inserted = 0;
    }
    return node;
}

/*
  Entradas: tree, client
  Salidas: 1 si se inserto, 0 si la identificacion ya existia
  Restricciones: no se permiten identificaciones duplicadas
*/
int insertClient(ClientBST *tree, Client client) {
    int inserted = 0;
    tree->root = insertClientNode(tree->root, client, &inserted);
    if (inserted) {
        tree->count++;
    }
    return inserted;
}

/*
  Entradas: tree, id
  Salidas: puntero al nodo encontrado o NULL
  Restricciones: ninguna
*/
ClientNode *findClient(ClientBST *tree, const char *id) {
    ClientNode *current = tree->root;
    while (current != NULL) {
        int cmp = strcmp(id, current->data.id);
        if (cmp == 0) {
            return current;
        }
        current = (cmp < 0) ? current->left : current->right;
    }
    return NULL;
}

/*
  Entradas: tree, id
  Salidas: 1 si el cliente existe, 0 si no
  Restricciones: ninguna
*/
int clientExistsInTree(ClientBST *tree, const char *id) {
    return findClient(tree, id) != NULL;
}

/* Imprime un cliente con sus datos */
static void printClient(const Client *c) {
    printf("  ID: %s | Nombre: %s | Tel: %s | Correo: %s | Zona: %s\n",
           c->id, c->fullName, c->phone, c->email, c->address);
}

static void inorder(ClientNode *node) {
    if (node == NULL) return;
    inorder(node->left);
    printClient(&node->data);
    inorder(node->right);
}

static void preorder(ClientNode *node) {
    if (node == NULL) return;
    printClient(&node->data);
    preorder(node->left);
    preorder(node->right);
}

static void postorder(ClientNode *node) {
    if (node == NULL) return;
    postorder(node->left);
    postorder(node->right);
    printClient(&node->data);
}

void displayClientsInorder(ClientBST *tree) {
    if (tree->root == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }
    inorder(tree->root);
}

void displayClientsPreorder(ClientBST *tree) {
    if (tree->root == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }
    preorder(tree->root);
}

void displayClientsPostorder(ClientBST *tree) {
    if (tree->root == NULL) {
        printf("No hay clientes registrados.\n");
        return;
    }
    postorder(tree->root);
}

/* Liberacion de memoria*/
static void freeNodes(ClientNode *node) {
    if (node == NULL) return;
    freeNodes(node->left);
    freeNodes(node->right);
    free(node);
}

void freeClientBST(ClientBST *tree) {
    freeNodes(tree->root);
    tree->root = NULL;
    tree->count = 0;
}

/*
  CLIENTS <cantidad>
  <id>|<nombre>|<telefono>|<correo>|<direccion>
*/

/* Recorre el arbol guardando cada cliente*/
static void saveNodes(ClientNode *node, FILE *file) {
    if (node == NULL) return;
    saveNodes(node->left, file);
    fprintf(file, "%s|%s|%s|%s|%s\n",
            node->data.id, node->data.fullName, node->data.phone,
            node->data.email, node->data.address);
    saveNodes(node->right, file);
}

int saveClientsToFile(ClientBST *tree, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir '%s' para guardar clientes.\n", filename);
        return 0;
    }
    fprintf(file, "CLIENTS %d\n", tree->count);
    saveNodes(tree->root, file);
    fclose(file);
    return 1;
}

int loadClientsFromFile(ClientBST *tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro '%s'. Se inicia sin clientes.\n", filename);
        return 0;
    }
    initClientBST(tree);
    int total = 0;
    if (fscanf(file, "CLIENTS %d\n", &total) != 1) {
        printf("Error: formato invalido en '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    for (int i = 0; i < total; i++) {
        Client c;
        if (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                   c.id, c.fullName, c.phone, c.email, c.address) != 5) {
            break;
        }
        insertClient(tree, c);
    }
    fclose(file);
    return 1;
}

/* Acceso a la instancia global */
ClientBST *getClientTree(void) {
    return &clientTree;
}

int clientExists(const char *id) {
    return clientExistsInTree(&clientTree, id);
}

void displayClientsInOrder(void) {
    displayClientsInorder(&clientTree);
}

/*
  MENU INTERACTIVO
*/
void displayClientMenu(void) {
    int option;
    do {
        printf("\n========== GESTION DE CLIENTES (BST) ==========\n");
        printf("1. Registrar cliente\n");
        printf("2. Buscar cliente por identificacion\n");
        printf("3. Mostrar clientes (inorden)\n");
        printf("4. Mostrar clientes (preorden)\n");
        printf("5. Mostrar clientes (postorden)\n");
        printf("6. Volver\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1: {
                Client c;
                readText("Identificacion: ", c.id, CLIENT_ID_LENGTH);
                if (clientExistsInTree(&clientTree, c.id)) {
                    printf("Error: ya existe un cliente con la identificacion '%s'.\n", c.id);
                    break;
                }
                readText("Nombre completo: ", c.fullName, CLIENT_NAME_LENGTH);
                readText("Telefono: ", c.phone, CLIENT_PHONE_LENGTH);
                readText("Correo electronico: ", c.email, CLIENT_EMAIL_LENGTH);
                readText("Direccion o zona de entrega: ", c.address, CLIENT_ADDRESS_LENGTH);
                if (insertClient(&clientTree, c)) {
                    printf("Cliente '%s' registrado correctamente.\n", c.id);
                }
                break;
            }
            case 2: {
                char id[CLIENT_ID_LENGTH];
                readText("Identificacion a buscar: ", id, CLIENT_ID_LENGTH);
                ClientNode *node = findClient(&clientTree, id);
                if (node == NULL) {
                    printf("No se encontro un cliente con la identificacion '%s'.\n", id);
                } else {
                    printf("Cliente encontrado:\n");
                    printClient(&node->data);
                }
                break;
            }
            case 3:
                printf("Clientes en recorrido inorden:\n");
                displayClientsInorder(&clientTree);
                break;
            case 4:
                printf("Clientes en recorrido preorden:\n");
                displayClientsPreorder(&clientTree);
                break;
            case 5:
                printf("Clientes en recorrido postorden:\n");
                displayClientsPostorder(&clientTree);
                break;
            case 6:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 6);
}
