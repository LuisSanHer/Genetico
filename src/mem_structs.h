/*
 * mem_structs.h
 *
 *  Created on: 08/10/2020
 *
 */
#ifndef MEM_STRUCTS_H_
#define MEM_STRUCTS_H_

/*Estructura con un vector binario*/
/*y con una aptidud*/
typedef struct{
	int *x;		//Cadena binaria (Genotipo)
	double f;	//Valor función objetivo (Fenotipo)
}INDIVIDUO;

typedef struct{
	int size; 	//Tamaño de la población
	INDIVIDUO *ind; //Arreglo de individuos
}POBLACION;

struct{
	int nbin;	//Tamaño candena binaria (Genotipo)
	int nobj;	//Número de objetivos.
}mop;

struct{
	int psize;	//Tamaño de la población
	int Gmax;	//Máximo de generaciones
	double Pc;	//Probabilidad de cruza
	double Pm;	//Probabilidad de mutación
}ga;

void alloc_pop(POBLACION *P, int psize);
void free_pop(POBLACION *P);
void cpy_pop(POBLACION *P, POBLACION *Q);
void cpy_ind(INDIVIDUO *A, INDIVIDUO *B);

#endif /* MEM_STRUCTS_H */