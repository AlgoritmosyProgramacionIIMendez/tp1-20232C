<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de Tomas Garcia Alimena - 110478 - tgarciaa@fi.uba.ar

- Para compilar:

```bash
gcc -c pokemon.c -o pokemon.o
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluír **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).

### Por ejemplo:

El programa funciona abriendo el archivo pasado como parámetro y leyendolo línea por línea. Por cada línea crea un registro e intenta agregarlo al vector. La función de lectura intenta leer todo el archivo o hasta encontrar el primer error. Devuelve un vector con todos los registros creados.

<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

En el archivo `sarasa.c` la función `funcion1` utiliza `realloc` para agrandar la zona de memoria utilizada para conquistar el mundo. El resultado de `realloc` lo guardo en una variable auxiliar para no perder el puntero original en caso de error:

```c
int *vector = realloc(vector_original, (n+1)*sizeof(int));

if(vector == NULL)
    return -1;
vector_original = vector;
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>

---

## Respuestas a las preguntas teóricas
Para ordenar a los pokemones alfabeticamente, utilice el metodo bubble sort(burbujeo), que ordena la lista, comparando elemento a elemento y realizando un cambio de posicion en caso de ser necesario. En el peor de los casos va a tener que cambiar de posicion a todos los elementos, por lo que si tomamos a n como la cantidad de elementos, se van a tener q realizar n movimientos n veces, es decir n.n o n². Costo computacional O(n²). Por lo contrario, en el mejor de los casos, la lista recibida ya esta ordenada por lo que solamente la recorre 1 vez, comparando los elementos n veces, es decir, que tiene un costo computacional en el mejor de los casos de O(n).
