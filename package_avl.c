/*
  Modulo: Gestion de paquetes por codigo mediante Arbol AVL (RoutePack)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "package_avl.h"
#include "package_list.h"
#include "io_utils.h"

/* Raiz global*/
static AVLNode *avlRoot = NULL;
/* Devuelve el mayor*/
static int maxInt(int a, int b) {
    return (a > b) ? a : b;
}

/*
  Entradas: node
  Salidas: altura del nodo (0 si es NULL)
  Restricciones: ninguna
*/
int avlHeight(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

/*
  Entradas: node
  Salidas: factor de balance = altura(izq) - altura(der)
  Restricciones: ninguna
*/
int avlBalanceFactor(AVLNode *node) {
    if (node == NULL) {
        return 0;
    }
    return avlHeight(node->left) - avlHeight(node->right);
}

/* Crea un nodo nuevo*/
static AVLNode *createAVLNode(Package pkg) {
    AVLNode *node = (AVLNode *) malloc(sizeof(AVLNode));
    if (node == NULL) {
        printf("Error: no hay memoria disponible para el nodo AVL.\n");
        return NULL;
    }
    node->data = pkg;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Recalcula la altura de un nodo*/
static void updateHeight(AVLNode *node) {
    node->height = 1 + maxInt(avlHeight(node->left), avlHeight(node->right));
}

/*
  Rotacion simple a la derecha.
*/
AVLNode *rotateRight(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

/*
  Rotacion simple a la izquierda.
*/
AVLNode *rotateLeft(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;

/* Reequilibra un nodo*/
static AVLNode *rebalance(AVLNode *node) {
    int balance = avlBalanceFactor(node);
    /*rotacion simple a la derecha*/
    if (balance > 1 && avlBalanceFactor(node->left) >= 0) {
        return rotateRight(node);
    }
    /*rotacion doble izq-der*/
    if (balance > 1 && avlBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    /*rotacion simple a la izquierda*/
    if (balance < -1 && avlBalanceFactor(node->right) <= 0) {
        return rotateLeft(node);
    }
    /*rotacion doble der-izq*/
    if (balance < -1 && avlBalanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

/*
  Entradas: root, pkg
  Salidas: nueva raiz del subarbol balanceado
  Restricciones: la llave es el codigo; no se insertan codigos duplicados
*/
AVLNode *insertAVL(AVLNode *root, Package pkg) {
    if (root == NULL) {
        return createAVLNode(pkg);
    }
    int cmp = strcmp(pkg.code, root->data.code);
    if (cmp < 0) {
        root->left = insertAVL(root->left, pkg);
    } else if (cmp > 0) {
        root->right = insertAVL(root->right, pkg);
    } else {
        root->data = pkg;
        return root;
    }
    updateHeight(root);
    return rebalance(root);
}

/*
  Entradas: root, code
  Salidas: nodo con ese codigo o NULL
  Restricciones: ninguna
*/
AVLNode *searchAVL(AVLNode *root, const char *code) {
    AVLNode *current = root;
    while (current != NULL) {
        int cmp = strcmp(code, current->data.code);
        if (cmp == 0) {
            return current;
        }
        current = (cmp < 0) ? current->left : current->right;
    }
    return NULL;
}

/* Imprime un paquete del AVL*/
static void printAVLPackage(const Package *p) {
    Package *live = findPackageByCode(p->code);
    const Package *show = (live != NULL) ? live : p;
    printf("  Codigo: %s | Cliente: %s | Destinatario: %s | Destino: %s | Estado: %s\n",
           show->code, show->clientId, show->recipientName, show->destination,
           stateToString(show->state));
}

/* Recorrido inorden*/
static void inorderAVL(AVLNode *node) {
    if (node == NULL) return;
    inorderAVL(node->left);
    printAVLPackage(&node->data);
    inorderAVL(node->right);
}

/*
  Entradas: root
  Salidas: ninguna
*/
void displayAVLInorder(AVLNode *root) {
    if (root == NULL) {
        printf("El arbol AVL de paquetes esta vacio.\n");
        return;
    }
    inorderAVL(root);
}

/* Libera recursivamente*/
void freeAVL(AVLNode *root) {
    if (root == NULL) return;
    freeAVL(root->left);
    freeAVL(root->right);
    free(root);
}

static int countAVL(AVLNode *node) {
    if (node == NULL) return 0;
    return 1 + countAVL(node->left) + countAVL(node->right);
}

static void saveAVLNodes(AVLNode *node, FILE *file) {
    if (node == NULL) return;
    saveAVLNodes(node->left, file);
    Package *p = &node->data;
    fprintf(file, "%s|%s|%s|%s|%.2f|%d|%d\n",
            p->code, p->clientId, p->recipientName, p->destination,
            p->weight, p->priority, (int) p->state);
    saveAVLNodes(node->right, file);
}

int savePackagesAVLToFile(AVLNode *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: no se pudo abrir '%s' para guardar el AVL.\n", filename);
        return 0;
    }
    fprintf(file, "PACKAGES_AVL %d\n", countAVL(root));
    saveAVLNodes(root, file);
    fclose(file);
    return 1;
}

int loadPackagesAVLFromFile(AVLNode **root, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: no se encontro '%s'. Se inicia con el AVL vacio.\n", filename);
        return 0;
    }
    *root = NULL;
    int total = 0;
    if (fscanf(file, "PACKAGES_AVL %d\n", &total) != 1) {
        printf("Error: formato invalido en '%s'.\n", filename);
        fclose(file);
        return 0;
    }
    for (int i = 0; i < total; i++) {
        Package p;
        int stateInt;
        if (fscanf(file, " %[^|]|%[^|]|%[^|]|%[^|]|%f|%d|%d\n",
                   p.code, p.clientId, p.recipientName, p.destination,
                   &p.weight, &p.priority, &stateInt) != 7) {
            break;
        }
        p.state = (PackageState) stateInt;
        *root = insertAVL(*root, p);
    }
    fclose(file);
    return 1;
}

/* Acceso a la raiz global */
AVLNode *getAVLRoot(void) {
    return avlRoot;
}
AVLNode **getAVLRootRef(void) {
    return &avlRoot;
}

void displayPackagesSortedByCode(void) {
    displayAVLInorder(avlRoot);
}

/* Inserta al AVL los paquetes que existan en el repositorio principal */
static void indexAllPackages(void) {
    PackageList *list = getPackageList();
    PackageNode *current = list->head;
    int added = 0;
    while (current != NULL) {
        avlRoot = insertAVL(avlRoot, current->data);
        added++;
        current = current->next;
    }
    printf("Se indexaron %d paquete(s) del repositorio en el AVL.\n", added);
}

/*
  MENU INTERACTIVO
*/
void displayPackageAVLMenu(void) {
    int option;
    do {
        printf("\n========== PAQUETES POR CODIGO (AVL) ==========\n");
        printf("1. Insertar paquete en el AVL (por codigo del repositorio)\n");
        printf("2. Indexar todos los paquetes del repositorio\n");
        printf("3. Buscar paquete por codigo\n");
        printf("4. Mostrar paquetes en inorden (ordenados por codigo)\n");
        printf("5. Mostrar altura y factor de balance de la raiz\n");
        printf("6. Volver\n");
        option = readInt("Opcion: ");

        switch (option) {
            case 1: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo del paquete a insertar: ", code, PKG_CODE_LENGTH);
                Package *p = findPackageByCode(code);
                if (p == NULL) {
                    printf("No existe un paquete con el codigo '%s' en el repositorio.\n", code);
                } else {
                    avlRoot = insertAVL(avlRoot, *p);
                    printf("Paquete '%s' insertado en el AVL. El arbol se mantiene balanceado.\n", code);
                }
                break;
            }
            case 2:
                indexAllPackages();
                break;
            case 3: {
                char code[PKG_CODE_LENGTH];
                readText("Codigo a buscar: ", code, PKG_CODE_LENGTH);
                AVLNode *node = searchAVL(avlRoot, code);
                if (node == NULL) {
                    printf("No se encontro el codigo '%s' en el AVL.\n", code);
                } else {
                    printf("Paquete encontrado en el AVL:\n");
                    printAVLPackage(&node->data);
                }
                break;
            }
            case 4:
                printf("Paquetes ordenados por codigo (inorden):\n");
                displayAVLInorder(avlRoot);
                break;
            case 5:
                printf("Altura del arbol: %d\n", avlHeight(avlRoot));
                printf("Factor de balance de la raiz: %d\n", avlBalanceFactor(avlRoot));
                break;
            case 6:
                printf("Regresando al menu principal...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (option != 6);
}
