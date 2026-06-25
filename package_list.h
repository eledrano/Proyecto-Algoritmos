/*
  Modulo: Gestion de paquetes registrados mediante Lista Enlazada (RoutePack)
  Luis Medrano Gonzalez / Tyrone Carranza Hernandez

  Ejercicio 3.2. La lista enlazada simple es el repositorio principal de
  paquetes del sistema (fuente unica de la verdad). La cola, la pila y el AVL
  trabajan sobre estos mismos paquetes referenciandolos por su codigo.

  Este header define el tipo Package y el enum PackageState que son
  compartidos por el resto de modulos de paquetes.
*/

#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

#define PKG_CODE_LENGTH 30
#define PKG_CLIENT_ID_LENGTH 30
#define PKG_NAME_LENGTH 60
#define PKG_DEST_LENGTH 50

/*
  Estados minimos permitidos para un paquete. El valor entero se usa tambien
  para la persistencia en archivo.
*/
typedef enum {
    STATE_REGISTERED = 0,   /* Registrado */
    STATE_PENDING    = 1,   /* Pendiente de entrega */
    STATE_IN_ROUTE   = 2,   /* En ruta */
    STATE_DELIVERED  = 3,   /* Entregado */
    STATE_RETURNED   = 4    /* Devuelto */
} PackageState;

/* Datos solicitados para cada paquete. La llave es 'code'. */
typedef struct {
    char code[PKG_CODE_LENGTH];
    char clientId[PKG_CLIENT_ID_LENGTH];
    char recipientName[PKG_NAME_LENGTH];
    char destination[PKG_DEST_LENGTH];
    float weight;
    int priority;
    PackageState state;
} Package;

/* Nodo de la lista enlazada simple */
typedef struct PackageNode {
    Package data;
    struct PackageNode *next;
} PackageNode;

/* Contenedor de la lista */
typedef struct {
    PackageNode *head;
    int count;
} PackageList;

/* Inicializacion */
void initPackageList(PackageList *list);

/* Operaciones principales */
int addPackage(PackageList *list, Package pkg);                       /* valida cliente y codigo unico */
Package *findPackageByCodeInList(PackageList *list, const char *code);
int updatePackageStateInList(PackageList *list, const char *code, PackageState newState);
int removePackage(PackageList *list, const char *code);
void displayPackages(PackageList *list);
int countByState(PackageList *list, PackageState state);

/* Liberacion de memoria */
void freePackageList(PackageList *list);

/* Utilidades de estado */
const char *stateToString(PackageState state);

/* Persistencia en archivo de texto */
int savePackagesToFile(PackageList *list, const char *filename);
int loadPackagesFromFile(PackageList *list, const char *filename);

/* Acceso a la instancia global del modulo */
PackageList *getPackageList(void);

/* Menu interactivo (lo invoca main.c) */
void displayPackageListMenu(void);

/*
  Contrato con otros modulos: estas funciones operan sobre el repositorio
  global y las usan la cola, la pila, el AVL y los reportes.
*/
Package *findPackageByCode(const char *code);                 /* repositorio global */
int updatePackageState(const char *code, PackageState state); /* repositorio global */
int getTotalRegisteredPackages(void);                         /* reports.c */
int getTotalDeliveredPackages(void);                          /* reports.c */

#endif
