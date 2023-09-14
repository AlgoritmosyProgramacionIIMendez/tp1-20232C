#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>

#define MAX_LINEA 50
#define MAX_ATAQUES 3
#define MAX_NOMBRE 20
#define ERROR -1

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t* pokemones;
	int cantidad;
};

char* leer_linea(FILE* archivo, char* linea, int tamaño_maximo)
{
	return fgets(linea, tamaño_maximo, archivo);
}

void parsear_pokemon(char* linea, struct pokemon *pokemon)
{
	char str_tipo[MAX_NOMBRE];
	int leidos = sscanf(linea, "%[^;];%[^\n]", pokemon->nombre, str_tipo);
	if(leidos == 2){
		if (strcmp(str_tipo, "normal") == 0) {
            pokemon->tipo = NORMAL;
        }else if(strcmp(str_tipo, "fuego") == 0){
            pokemon->tipo = FUEGO;
        }else if(strcmp(str_tipo, "agua") == 0){
            pokemon->tipo = AGUA;
        }else if(strcmp(str_tipo, "planta") == 0){
            pokemon->tipo = PLANTA;
        }else if(strcmp(str_tipo, "electrico") == 0){
            pokemon->tipo = ELECTRICO;
        }else if(strcmp(str_tipo, "roca") == 0){
            pokemon->tipo = ROCA;
        }else{
			pokemon->tipo = ERROR;
		}
	}
}

void parsear_ataque(char* linea, struct pokemon *pokemon, int i)
{
	char str_tipo[MAX_NOMBRE];
	int leidos = sscanf(linea,"%[^;];%[^;];%u", pokemon->ataques[i].nombre, str_tipo, &pokemon->ataques[i].poder);
	if(leidos  == 3){
		if(strcmp(str_tipo, "normal") == 0){
			pokemon->ataques[i].tipo = NORMAL;
		}else if(strcmp(str_tipo, "fuego") == 0){
			pokemon->ataques[i].tipo = FUEGO;
		}else if(strcmp(str_tipo, "agua") == 0){
			pokemon->ataques[i].tipo = AGUA;
		}else if(strcmp(str_tipo, "planta") == 0){
			pokemon->ataques[i].tipo = PLANTA;
		}else if(strcmp(str_tipo, "electrico") == 0){
			pokemon->ataques[i].tipo = ELECTRICO;
		}else if(strcmp(str_tipo, "roca") == 0){
			pokemon->ataques[i].tipo = ROCA;
		}
	}else{
		pokemon->ataques[i].tipo = ERROR;
	}
}

pokemon_t* obtener_un_pokemon(FILE* archivo)
{
	struct pokemon *pokemon = malloc(sizeof(struct pokemon));
	if(pokemon == NULL){
		return NULL;
	}

	char linea[MAX_LINEA];
    
	if(leer_linea(archivo, linea, sizeof(linea)) == NULL){
        free(pokemon);
        return NULL;
    }
	parsear_pokemon(linea, pokemon);
	
	for(int i = 0; i < MAX_ATAQUES; i++){
        if(leer_linea(archivo, linea, sizeof(linea)) == NULL){
            free(pokemon);
            return NULL;
        }
		parsear_ataque(linea, pokemon, i);
		
        if(pokemon->tipo == ERROR || pokemon->ataques[i].tipo == ERROR){
            free(pokemon);
            return NULL;
        }
    }
	return pokemon;
}

void burbujeo(pokemon_t *pokemones, int cantidad){
	for(int i = 0; i < cantidad - 1; i++){
		for(int j = 0; j < cantidad - i - 1; j++){
			if(strcmp(pokemones[j].nombre, pokemones[j + 1].nombre) > 0){
				pokemon_t aux = pokemones[j];
				pokemones[j] = pokemones [j + 1];
				pokemones[j + 1] = aux;
			}
		}
	}
}

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	informacion_pokemon_t *lista_pokemones = malloc(sizeof(informacion_pokemon_t));
	if(lista_pokemones == NULL){
    	return NULL;
    }
	
	lista_pokemones->pokemones = NULL;
	lista_pokemones->cantidad = 0;
	
	FILE *archivo = fopen(path, "r");
	if(archivo == NULL){
		free(lista_pokemones);
		return NULL;
	}
	
	pokemon_t *pokemon_leido = NULL;
	while(!feof(archivo)){
		pokemon_leido = obtener_un_pokemon(archivo);
		if(pokemon_leido != NULL){
			lista_pokemones->cantidad ++;
			pokemon_t *aux = realloc(lista_pokemones->pokemones, sizeof(pokemon_t) * (size_t)lista_pokemones->cantidad);

			if(aux == NULL){
        		fclose(archivo);
        		free(pokemon_leido);
				free(lista_pokemones->pokemones);
				free(lista_pokemones);
        		return NULL;
    		}
			lista_pokemones->pokemones = aux;
			lista_pokemones->pokemones[lista_pokemones->cantidad - 1] = *pokemon_leido;
        	free(pokemon_leido);
		}	
	}
	fclose(archivo);

	if(lista_pokemones->cantidad == 0){
		free(lista_pokemones->pokemones);
		free(lista_pokemones);
		return NULL;
	}

	return lista_pokemones;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if(ip == NULL || nombre == NULL){
		return NULL;
	}
	
	for(int i = 0; i < ip->cantidad; i++){
		if(strcmp(ip->pokemones[i].nombre, nombre) == 0){
			return &ip->pokemones[i];
		}
	}
	
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if(ip == NULL){
		return 0;
	}
	
	return ip->cantidad;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return NULL;
	}

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if(pokemon == NULL){
		return NORMAL;
	}

	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if(pokemon == NULL || nombre == NULL){
		return NULL;
	}
	
	for(int i = 0; i < MAX_ATAQUES; i++){
		if(pokemon->ataques[i].nombre != NULL && strcmp(pokemon->ataques[i].nombre, nombre) == 0){
			return &pokemon->ataques[i];
		}
	}

	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if(ip == NULL || f == NULL || aux == NULL){
		return 0;
	}
	
	burbujeo(ip->pokemones, ip->cantidad);

	int contador = 0;
	for(int i = 0; i < ip->cantidad; i++){
		if(ip->pokemones[i].nombre[0] != '\0'){
			f(&(ip->pokemones[i]), aux);
			contador ++;
		}
	}

	return contador;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if(pokemon == NULL || f == NULL || aux == NULL){
		return 0;
	}
	
	int contador = 0;
	for(int i = 0; i < MAX_ATAQUES; i++){
		if(pokemon->ataques[i].nombre[0] != '\0'){
			f(&(pokemon->ataques[i]), aux);
			contador ++;
		}
	}

	return contador;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if(ip == NULL){
		return;
	}
	free(ip->pokemones);
	free(ip);
}
