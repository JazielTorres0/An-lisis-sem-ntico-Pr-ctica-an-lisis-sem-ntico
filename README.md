#  Analizador Semántico en C++

##  Descripción

Este proyecto implementa un **analizador semántico** para un lenguaje sencillo, utilizando programación orientada a objetos en C++.

El sistema forma parte de las etapas de un compilador y se encarga de:

- Validar tipos de datos
- Detectar variables no declaradas
- Detectar redefiniciones (variables y funciones)
- Manejar ámbitos (global y local)
- Construir y mostrar una tabla de símbolos

---


##  Estructura del programa

###  Clase Nodo

Clase base para todos los elementos del árbol sintáctico.

Atributos:

* `tipoDato`
* `sig`
* `tablaSimbolos` (estático)
* `ambito` (estático)

Método principal:

* `validaTipos()`

---

###  Tabla de Símbolos

Clase encargada de almacenar variables y funciones.

Funciones principales:

* `agrega()` → Inserta elementos
* `varGlobalDefinida()` → Verifica variables globales
* `varLocalDefinida()` → Verifica variables locales
* `funcionDefinida()` → Verifica funciones
* `buscaIdentificador()` → Busca variables
* `muestra()` → Imprime la tabla

---

###  Clases principales

* `DefVar` → Definición de variables
* `DefFunc` → Definición de funciones
* `Parametro` → Parámetros de funciones
* `Identificador` → Uso de variables
* `Tipo` → Manejo de tipos de datos

---

###  Clase Semantico

Encargada de ejecutar el análisis semántico.

Métodos:

* `analiza(arbol)` → Ejecuta validaciones y muestra resultados

---

##  Compilación y ejecución

###  Compilar

```bash
g++ semantico.cpp -o semantico
```

###  Ejecutar

```bash
./semantico
```

---

##  Ejemplo de uso

### Entrada simulada:

```cpp
int x;
int x;
```

### Salida esperada:

```
--- TABLA ---
x | i |
x | i |

--- ERRORES ---
Error: variable global redefinida
```

---

##  Tipos soportados

| Tipo   | Código |
| ------ | ------ |
| int    | i      |
| float  | f      |
| string | s      |
| void   | v      |

---

## Funcionalidades implementadas

- Validación de tipos
- Detección de variables no definidas
- Manejo de ámbitos
- Detección de redefiniciones
- Tabla de símbolos

---

