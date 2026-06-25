/*
  Modulo: Gestion de paquetes registrados mediante Lista Enlazada (RoutePack)
*/

#ifndef PACKAGE_LIST_H
#define PACKAGE_LIST_H

#define PKG_CODE_LENGTH 30
#define PKG_CLIENT_ID_LENGTH 30
#define PKG_NAME_LENGTH 60
#define PKG_DEST_LENGTH 50

/*
  Estados permitidos para un paquete
*/
typedef enum {
    STATE_REGISTERED = 0,/* Registrado*/
    STATE_PENDING    = 1,/* Pendiente de entrega*/
    STATE_IN_ROUTE   = 2,/* En ruta*/
    STATE_DELIVERED  = 3,/* Entregado*/
    STATE_RETURNED   = 4 /* Devuelto*/
} PackageState;

/* Datos para cada paquete*/
typedef struct {
    char code[PKG_CODE_LENGTH];
    char clientId[PKG_CLIENT_ID_LENGTH];
    char recipientName[PKG_NAME_LENGTH];
    char destination[PKG_DEST_LENGTH];
    float weight;
    int priority;
    PackageState state;
} Package;

/* Nodo de lista simple */
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

int addPackage(PackageList *list, Package pkg);/* valida cliente y codigo unico*/
Package *findPackageByCodeInList(PackageList *list, const char *code);
int updatePackageStateInList(PackageList *list, const char *code, PackageState newState);
int removePackage(PackageList *list, const char *code);
void displayPackages(PackageList *list);
int countByState(PackageList *list, PackageState state);

/* Libera de memoria */
void freePackageList(PackageList *list);
const char *stateToString(PackageState state);

int savePackagesToFile(PackageList *list, const char *filename);
int loadPackagesFromFile(PackageList *list, const char *filename);

/* Acceso a la instancia global*/
PackageList *getPackageList(void);

/* Menu interactivo*/
void displayPackageListMenu(void);
Package *findPackageByCode(const char *code);
int updatePackageState(const char *code, PackageState state);
int getTotalRegisteredPackages(void);
int getTotalDeliveredPackages(void);

#endif
