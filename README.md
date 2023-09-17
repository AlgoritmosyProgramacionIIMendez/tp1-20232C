<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP1

## Repositorio de Tomas Garcia Alimena - 110478 - tgarciaa@fi.uba.ar

- Para compilar:

```bash
make pruebas_chanutron
```

- Para ejecutar:

```bash
make valgrind-chanutron
```

- Para ejecutar con valgrind:
```bash
make valgrind-chanutron
```
---
##  Funcionamiento

pokemon_cargar_archivo: Se comprueba que la ruta del archivo recibido como parametro no sea NULL, en caso de serlo devuelve NULL, luego, reserva memoria para la estructura informacion_pokemon_t llamada lista_pokemones, si esta falla, tambien devuelve NULL. Se inicializa la estructura lista_pokemones y se intenta abrir el archivo en modo lectura, en caso de error, devuelve NULL y libera memoria reservada. Luego inicia un ciclo while, que se encarga de obtener pokemon a pokemon utilizando la funcion obtener_un_pokemon hasta que finalice el archivo, para cada uno de los pokemones se verifica si este es o no NULL, si lo es, y la cantidad de pokemones cargados hasta el momento es 0, se libera toda la memoria previamente asignada, se cierra el archivo y se devuelve NULL, pero si ya hay algun pokemon valido en la lista, se libera de igual modo toda la memoria reservada, se cierra el archivo y se devuelve la lista que se tiene hasta el momento. En caso de que el pokemon no sea NULL, se aumenta el campo lista_pokemones->cantidad en 1 unidad (lista_pokemones->cantidad++), luego, utiliza realloc para ajustar dinámicamente la memoria asignada a lista_pokemones->pokemones para asignar al nuevo pokemon leido, si realloc falla, se cierra el archivo, se libera memoria asignada previamente y se devuelve NULL. Para asignar al nuevo pokemon se copia la estructura de *pokemon_leido a la ultima posicion de lista_pokemones->pokemones, y se libera la memoria de pokemon_leido. Para finalizar, se cierra el archivo, y se realiza una ultima verificacion, que se encarga de que en caso de no haberse obtenido ningun pokemon valido, se libere la memoria reservada para la lista y se devuelva NULL.

<div align="center">
<img width="70%" src="src/img/pokemon_cargar_archivo.odg">
</div>

obtener_un_pokemon: Se comprueba que el archivo recibido no sea NULL, en caso de serlo, es un error, devuelve NULL. Se reserva memoria para un struct pokemon utilizando malloc. Si la asignacion de memoria falla se libera la memoria y la función devuelve NULL. Se declara un string de caracteres linea, de tamaño MAX_LINEA(100) para almacenar lineas leidas del archivo, luego se llama a la función leer_linea para obtener una linea del archivo. Si la lectura falla se libera la memoria asignada para pokemon y la funcion devuelve NULL. Para parsear los datos, se llama a la funcion parsear_pokemon para analizar la línea leida y completar los campos nombre y tipo del pokemon. Despues, se inicia un for que se ejecuta MAX_ATAQUES(3) veces. En cada iteracion, se lee una linea, se parsea el ataque y se almacena en la estructura pokemon en la posicion i del arreglo ataques. Por ultimo, se realiza una verificación en un nuevo ciclo for, que comprueba si el tipo del pokemon como el tipo de los ataques, son validos (no sean ERROR). Si se encuentra un tipo de pokemon o de ataque invalido, se libera la memoria asignada para pokemon y se devuelve NULL, caso contrario se devuelve un puntero a la estructura pokemon creada.

<div align="center">
<img width="70%" src="src/img/obtener_un_pokemon.odg">
</div>


## Respuestas a las preguntas teóricas
Para ordenar a los pokemones alfabeticamente, utilice el metodo bubble sort(burbujeo), que ordena la lista, comparando elemento a elemento y realizando un cambio de posicion en caso de ser necesario. En el peor de los casos va a tener que cambiar de posicion a todos los elementos, por lo que si tomamos a n como la cantidad de elementos, se van a tener q realizar n movimientos n veces, es decir n.n o n². Costo computacional O(n²). Por lo contrario, en el mejor de los casos, la lista recibida ya esta ordenada por lo que solamente la recorre 1 vez, comparando los elementos n veces, es decir, que tiene un costo computacional en el mejor de los casos de O(n).
