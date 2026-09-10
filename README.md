# Proyecto1-EDD
Proyecto en conjunto para el 2 octubre, asignatura Estructura de Datos (EDD).

### Integrantes
3vinny, falv-u, feliblade

## Estructura del proyecto
.
├── src/
├   ├──main.c
├── include/
├── Makefile
└── README.md

## Requisitos
1. Un algoritmo de ordenamiento iterativo
2. Un algoritmo de ordenamiento basado en recurisivdad
3. Busqueda binaria (versión recursiva)
4. Generación y carga de datos
5. Funcionalidades del sistema

### Funciones requeridas
1. Gestión catalogo
2. Ordenamiento y búsqueda
3. Ranking y estadísticas
4. Fila de reproducción
5. Reproducción e historial
6. Exportación
7. Interfaz

## Normas
1. Tabs duros (8 espacios), nada de espacio para indentar
2. Macros definidos en mayusculas (ej -> #define MAX 100)
3. Maximo indentacion de 3 niveles, 4 en casos especiales
4. snake_case: Nombres, variables y funciones minusculas (ej variable_baja)
5. Comentarios de linea /* */
6. Comentarios de bloque
7. Sentencias con espacio, ej: if (holi == 1) {}
8. Declarar y luego asignar.

## Ejemplo funcion simple:
```c
int hola()
{
    int a;
    a=10;
    if (a == 10) 
    {
        printf("holi");
        return 0;
    }
    if (a < 10)       
        return a;
    return a;
}
```
# Ejemplito
