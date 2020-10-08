#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "rand.h"

/*Estructura con un vector binario*/
/*y con una aptidud*/
typedef struct{
	int *x;
	double f;
}INDIVIDUO;

typedef struct{
	int size;
	INDIVIDUO *ind;
}POBLACION;

struct{
	int nbin;
	int nobj;
}mop;

struct{
	int psize;
	int Gmax;
	double Pc;
	double Pm;
}ga;

void Mutacion(POBLACION *Q, double Pm);
void Evaluacion(POBLACION *Q);
void Crossover(POBLACION *P, POBLACION *Q, double Pc);
void one_point_crossover(INDIVIDUO *P1, INDIVIDUO *P2, INDIVIDUO *H1, INDIVIDUO *H2, double Pc);
void bit_wise_mutation(INDIVIDUO *Q, double Pm);
void funcion(INDIVIDUO *p);
int xor(int a, int b);
void Inicializar(POBLACION *P);
void Display_pop(POBLACION *P);
void alloc_pop(POBLACION *P, int psize);
void free_pop(POBLACION *P);
void cpy_pop(POBLACION *P, POBLACION *Q); //
void cpy_ind(INDIVIDUO *A, INDIVIDUO *B); //


int main(){
	POBLACION P,Q;
	size_t i=0;

	mop.nbin = 16;
	mop.nobj = 1;

	ga.psize = 10;
	ga.Pc = 0.9;
	ga.Pm = 1.0 / mop.nbin;
	ga.Gmax = 100;

	alloc_pop(&P, ga.psize);
	alloc_pop(&Q, ga.psize);

	randomize(0.5);

	Inicializar(&P);
	Evaluacion(&P);
	Display_pop(&P);
	printf("Genetico Simple\n");
	for(i=0 ; i<ga.Gmax ; i++){
		printf("Generación: %zu\n", i);
		Crossover(&P,&Q, ga.Pc);
		Mutacion(&Q, ga.Pm);
		Evaluacion(&Q);
		cpy_pop(&P, &Q);
	}
	printf("Genetico Simple terminando\n");
	Display_pop(&P);

	free_pop(&P);
	free_pop(&Q);
	return 0;
}

void cpy_pop(POBLACION *P, POBLACION *Q){
	size_t i;
	assert(P->size == Q->size);
	for(i=0 ; i<P->size ; i++){
		cpy_ind(&P->ind[i], &Q->ind[i]);
	}
}

void cpy_ind(INDIVIDUO *A, INDIVIDUO *B){
	A->f = B->f;
	memcpy(A->x, B->x, sizeof(int) * mop.nbin);
}


void Inicializar(POBLACION *P){
	size_t i,j;
	for(i=0 ; i< P->size ; i++){
		for(j=0 ; j<mop.nbin ; j++){
			if(randomperc() < 0.5){
				P->ind[i].x[j] = 0;
			}else{
				P->ind[i].x[j] = 1;
			}
		}
	}
}

void Display_pop(POBLACION *P){
	size_t i,j;
	for(i=0 ; i<P->size ; i++){
		printf("Individuo %zu: f: %lf \t x: ", i, P->ind[i].f);
		for(j=0 ; j<mop.nbin ; j++){
			printf("%d", P->ind[i].x[j]);
		}
		printf("\n");
	}
}

void alloc_pop(POBLACION *P, int psize){
	size_t i;
	P->size = psize;
	P->ind = (INDIVIDUO*)malloc(sizeof(INDIVIDUO) * psize);
	for(i=0 ; i<psize ; i++){
		P->ind[i].x=(int*)malloc(sizeof(int) * mop.nbin);
	}
}

void free_pop(POBLACION *P){
	size_t i;
	size_t psize = P->size;
	for(i=0 ; i< psize ; i++){
		free(P->ind[i].x);
	}
	free(P->ind);
}

void Mutacion(POBLACION *Q, double Pm){
	size_t i;
	size_t n = Q->size;
	for(i=0 ; i<n ; i++){
		bit_wise_mutation(&Q->ind[i], Pm);
	}
}

void Evaluacion(POBLACION *Q){
	size_t i;
	int n = Q->size;
	for(i=0 ; i<n ; i++){
		funcion(&Q->ind[i]);
	}
}

void Crossover(POBLACION *P, POBLACION *Q, double Pc){
	size_t i,j;
	size_t n = P->size ;
	int *A;
	int *p;

	A = (int*)malloc(sizeof(int) * n);
	p = (int*)malloc(sizeof(int) * n);
	for(i=0 ; i<n ; i++){
		A[i] = i;
	}
	shuffle(A, n);
	//Primer Torneo binario
	j = 0;
	for(i=0 ; i<n ; i=i+2){
		if( P->ind[A[i]].f < P->ind[A[i+1]].f){
			p[j] = A[i];
		}else{
			p[j] = A[i+1];
		}
		j++;
	}
	shuffle(A, n);
	//Segundo Torneo binario
	for(i=0 ; i<n ; i=i+2){
		if( P->ind[A[i]].f < P->ind[A[i+1]].f){
			p[j] = A[i];
		}else{
			p[j] = A[i+1];
		}
		j++;
	}
	//Crossver P1 x P2 regresando 2 hijos.
	for(i=0 ; i<n ; i=i+2){
		one_point_crossover( &P->ind[p[i]], &P->ind[p[i+1]], &Q->ind[i], &Q->ind[i+1], Pc);
	}
	free(A);
	free(p);
}

void one_point_crossover(INDIVIDUO *P1, INDIVIDUO *P2, INDIVIDUO *H1, INDIVIDUO *H2, double Pc){
	int p, i;
	if(randomperc() < Pc){
		p = rndint(1, mop.nbin-2);
		for(i=0 ; i<mop.nbin ; i++){
			if(i<p){
				H1->x[i] = P1->x[i];
				H2->x[i] = P2->x[i];
			}else{
				H1->x[i] = P2->x[i];
				H2->x[i] = P1->x[i];
			}
		}
	}else{
		/*for(i=0 ; i<mop.nbin ; i++){
			H1->x[i] = P1->x[i];
			H2->x[i] = P2->x[i];
		}*/
		memcpy(H1->x, P1->x, sizeof(int)*mop.nbin);
		memcpy(H2->x, P2->x, sizeof(int)*mop.nbin);
	}


}

void bit_wise_mutation(INDIVIDUO *Q, double Pm){
	size_t i;
	for(i=0 ; i<mop.nbin ; i++){
		if(randomperc() < Pm){
			Q->x[i] = 1 - Q->x[i];
		}
	}
}

int xor(int a, int b){
	if(a == b){
		return 0;
	}else{
		return 1;
	}
}

void funcion(INDIVIDUO *p){
	size_t i,hamming;
	int *y, *r;
	y = (int*)malloc(sizeof(int) * mop.nbin);
	r = (int*)malloc(sizeof(int) * mop.nbin);
	for(i=0 ; i<mop.nbin ; i++){
		//Si es par dame 1, si no, dame 0.
		if((i%2)==0){
			y[i] = 1;
		}else{
			y[i] = 0;
		}
	}
	//Aplicando el XOR
	for(i=0 ; i<mop.nbin ; i++){
		r[i] = xor(p->x[i], y[i]);
	}
	//Distancia de Hamming
	hamming=0.0;
	for(i=0 ; i<mop.nbin ; i++){
		if(r[i] == 1){
			hamming = hamming + 1.0;
		}
	}
	p->f = hamming;
	free(y);
	free(r);
}
