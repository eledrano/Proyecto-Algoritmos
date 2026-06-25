/*
  Modulo: Reportes del sistema (RoutePack)
Menu de reportes del sistema. Algunos reportes dependen de datos gestionados por otros modulos del equipo (paquetes y clientes).
 */

#ifndef REPORTS_H
#define REPORTS_H

#include "routes_graph.h"

/* Muestra el menu de reportes y ejecuta el reporte elegido */
void displayReportsMenu(Graph *graph);

#endif
