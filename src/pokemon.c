#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>

#define MAX_LINEA 50
#define MAX_ATAQUES 3
#define MAX_NOMBRE 20

struct pokemon {
	char nombre[MAX_NOMBRE];
	enum TIPO tipo;
	struct ataque ataques[MAX_ATAQUES];
};

struct info_pokemon {
	pokemon_t* pokemones;
	int cantidad;
};

char* leer_linea(FILE* archivo, char* linea)
{
	return fgets(linea, sizeof(linea), archivo);
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
        } 
	}
}

struct ataque* parsear_ataque(char* linea)
{
	struct ataque *ataque = malloc(sizeof(struct ataque));
	char str_tipo[MAX_NOMBRE];
	int leidos = sscanf(linea,"%[^;];%[^;];%u", ataque->nombre, str_tipo, &ataque->poder);
	if(leidos  == 3){
		if(strcmp(str_tipo, "normal") == 0){
			ataque->tipo = NORMAL;
		}else if(strcmp(str_tipo, "fuego") == 0){
			ataque->tipo = FUEGO;
		}else if(strcmp(str_tipo, "agua") == 0){
			ataque->tipo = AGUA;
		}else if(strcmp(str_tipo, "planta") == 0){
			ataque->tipo = PLANTA;
		}else if(strcmp(str_tipo, "electrico") == 0){
			ataque->tipo = ELECTRICO;
		}else if(strcmp(str_tipo, "roca") == 0){
			ataque->tipo = ROCA;
		}
		return ataque;
	}else{
		return NULL;
	}
}

pokemon_t* obtener_un_pokemon(FILE* archivo)
{
	struct pokemon *pokemon = malloc(sizeof(struct pokemon));
	char linea[MAX_LINEA];
    if (leer_linea(archivo, linea) == NULL){
        free(pokemon);
        return NULL;
    }
	leer_linea(archivo, linea);
	parsear_pokemon(linea, pokemon);
	if(pokemon == NULL){
		return NULL;
	}
	leer_linea(archivo, linea);
	struct ataque *ataque1 = parsear_ataque(linea);
	leer_linea(archivo, linea);
	struct ataque *ataque2 = parsear_ataque(linea);
	leer_linea(archivo, linea);
	struct ataque *ataque3 = parsear_ataque(linea);
	
	pokemon->ataques[0] = *ataque1;
	pokemon->ataques[1] = *ataque2;
	pokemon->ataques[2] = *ataque3;
	if(ataque1 == NULL ||ataque2 == NULL ||ataque3 == NULL){
		free(pokemon);
		return NULL;
	}
	free(ataque1);
	free(ataque2);
	free(ataque3);

	return pokemon;
}

void burbujeo(pokemon_t *pokemones, int cantidad){
	for(int i = 0; i < cantidad - 1; i++){
		for(int j = 0; j < cantidad - i - 1; j++){
			if(strcpm(pokemones[j].nombre, pokemones[j + 1].nombre) > 0){
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
	
	pokemon_t *pokemon_leido;
	while((pokemon_leido = obtener_un_pokemon(archivo)) != NULL){
		lista_pokemones->cantidad ++;
		pokemon_t *aux = realloc(lista_pokemones->pokemones, sizeof(pokemon_t) * lista_pokemones->cantidad);

		if(aux == NULL){
            fclose(archivo);
            free(lista_pokemones);
            return NULL;
    	}

		lista_pokemones->pokemones = aux;
		lista_pokemones->pokemones[lista_pokemones->cantidad - 1] = *pokemon_leido;
        free(pokemon_leido);
	}

	fclose(archivo);

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
		if(strcmp(pokemon->ataques[i].nombre, nombre) == 0){
			return &pokemon->ataques[i];
		}
	}

	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if(ip == NULL || f == NULL){
		return NULL;
	}
	
	burbujeo(ip->pokemones, ip->cantidad);

	int contador = 0;
	for(int i = 0; i < ip->cantidad; i++){
		if(f(ip->pokemones[i], aux) == NULL){
			contador ++;
		}
	}

	return contador;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	return 0;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if(ip == NULL){
		return;
	}
	if(ip->pokemones != NULL){
		for(int i = 0; i < ip->cantidad; i++){
			free(&(ip->pokemones[i]));
		}
		free(ip->pokemones);
	}
	free(ip);
}
