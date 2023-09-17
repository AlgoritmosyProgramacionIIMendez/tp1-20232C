#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>



#define LONGITUD_INICIAL 100
#define LONGITUD_ATAQUE_POKEMON_MAXIMA 30
#define LONGITUD_NOMBRE_POKEMON_MAXIMA 30

struct pokemon {
/*ver hardcodes*/
	char nombre[30];
	enum TIPO tipo;
	
	struct ataque ataque[3];
};

struct info_pokemon {

	pokemon_t * pokemones;
	int cantidad_pokemones;
	size_t memoria_pedida;
};


enum TIPO obtener_tipo_pokemon(char letra){

enum TIPO tipo;

	switch(letra){
		case 'f':
		case 'F':
			 tipo = FUEGO; 
return tipo;
		case 'a':
		case 'A':
			 tipo = AGUA;
return tipo;
		case 'P':
		case 'p':
			 tipo= PLANTA;
	return tipo;
		case 'e':
		case 'E':
			tipo= ELECTRICO;
return tipo;
		case 'r':
		case 'R':
			 tipo= ROCA;
return tipo;
		case 'n':
		case 'N':
		default:
		 tipo= NORMAL;
}
return tipo;
}



int procesar_pokemon (pokemon_t * pokemon, FILE * datos,char * nombre)
{
	char ataque[LONGITUD_ATAQUE_POKEMON_MAXIMA+2];
	char * temp=NULL;

	if (datos == NULL || pokemon == NULL|| nombre == NULL)
		return 0;
	
	 int campos_nombre  = sscanf(nombre, "%100[a-zA-Z ];%50[a-zA-Z ]\n", pokemon->nombre, temp);
		if (campos_nombre != 2)
        return 1;
		pokemon->tipo = obtener_tipo_pokemon(*temp);
 

	for(int i =0;i<3;i++)
	{

    	if (fgets(ataque, LONGITUD_ATAQUE_POKEMON_MAXIMA+2, datos) ==NULL)
    	    return 1;
    	
		 int campos_ataque  = sscanf(ataque, "%100[a-zA-Z ];%50[a-zA-Z ];%u\n", pokemon->ataque[i].nombre,temp,&pokemon->ataque[i].poder);
    	if (campos_ataque != 3) 
        	return 1;
   
	pokemon->ataque[i].tipo =  obtener_tipo_pokemon(*temp);


/*
char poder[9] 
   	pokemon->ataque[i].poder = strtoul(poder,&temp,10);
		if(*temp)
		return 1;
*/	
	}

    return 0;
}


informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	
	informacion_pokemon_t* info;
	char nombre[LONGITUD_NOMBRE_POKEMON_MAXIMA+2];
	FILE * datos;
 	pokemon_t pokemon_aux;

	if(path == NULL)
	{
		return NULL;
	}

	if( (datos = fopen(path,"rt") ) == NULL){
		return NULL;
	}



	if(  (info = (informacion_pokemon_t*) malloc( sizeof(informacion_pokemon_t)) )  == NULL);
    {
        fclose(datos);
        return NULL;
    }
	/*malloc pokemones*/
	if(  (info->pokemones = (pokemon_t*) malloc( sizeof(pokemon_t) * LONGITUD_INICIAL) )  == NULL);
    {
        fclose(datos);
		  free(info);
        return NULL;
    }

	info->cantidad_pokemones = 0;
   info->pokemones = NULL;
   info->memoria_pedida = LONGITUD_INICIAL;

	/*procesar pokemon*/	
	while(fgets(nombre, LONGITUD_NOMBRE_POKEMON_MAXIMA , datos) != NULL)	
	{
		pokemon_t* aux;
		if(info->cantidad_pokemones>=info->memoria_pedida)
			{
			if( (aux= (pokemon_t*)realloc(info->pokemones , info->memoria_pedida *sizeof(pokemon_t) *2    ) ) ==NULL)
				{
        		fclose(datos);
				
		  		return info;
        		
    			}
			info->pokemones = aux;
			info->memoria_pedida = info->memoria_pedida *2;
			}

		if( procesar_pokemon(&pokemon_aux,datos,nombre) != 0 )
			{
			fclose(datos);
			return info;
			
			}
		info->pokemones[info->cantidad_pokemones] = pokemon_aux;
		info->cantidad_pokemones++;

	}
	
			
	fclose(datos);
	return info;

}

pokemon_t * pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{	
	if (ip == NULL|| nombre == NULL)
		return NULL;

	for(int i=0;i<ip->cantidad_pokemones;i++)
	{
	if(ip->pokemones[i].nombre == nombre)
		return &(ip->pokemones[i]);

	}
 return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if(ip == NULL)
		return 0;

	return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return NULL;

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if(pokemon == NULL)
		return 0;

	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL|| nombre == NULL)
		return NULL;

	for(int i=0;i<3;i++)
	{
	if(pokemon->ataque[i].nombre == nombre)
		return &(pokemon->ataque[i]);

	}

	return NULL;
}


void ordenar_alfabeticamente_menor_mayor(informacion_pokemon_t *info)
{
	if (info == NULL||info->pokemones == NULL)
		return;
	
	for (int i = 0; i < info->cantidad_pokemones-1; i++) {
		for (int j = 0; j < info->cantidad_pokemones - i - 1; j++) {

			if (  strcmp(info->pokemones[j].nombre, info->pokemones[j+1].nombre) >=  0) {
				pokemon_t *aux;// = &info->pokemones[j];
				//&(info->pokemones[j]) = &(info->pokemones[j+1]);
				//&info->pokemones[j+1] = aux;
				memcpy(aux, &info->pokemones[j], sizeof(pokemon_t));
				memcpy(&info->pokemones[j],&(info->pokemones[j+1]), sizeof(pokemon_t));
				memcpy(&info->pokemones[j+1],aux, sizeof(pokemon_t));
			}
		}
	}
}


int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	int exitos=0;

	if(ip == NULL|| f == NULL|| aux == NULL)
		return 0;

	ordenar_alfabeticamente_menor_mayor(ip);

	for(int i=0;i<ip->cantidad_pokemones;i++)
		{
		(*f)(&ip->pokemones[i], aux);
		exitos++;
		}


	return exitos;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	int exitos=0;
	if(pokemon == NULL|| f == NULL|| aux == NULL)
		return 0;

	for(int i=0;i<3;i++)
	{
	
	(*f)(&pokemon->ataque[i], aux);
	exitos++;

	}


	return exitos;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
if(ip == NULL)
		return;

	free(ip->pokemones);
	free(ip);

}



