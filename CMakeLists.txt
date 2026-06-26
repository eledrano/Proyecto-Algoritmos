cmake_minimum_required(VERSION 3.15)
project(Proyecto_AED C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto_AED
        main.c
        client_bst.c
        package_list.c
        delivery_queue.c
        returns_stack.c
        package_avl.c
        routes_graph.c
        reports.c
        files.c
)
