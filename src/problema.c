/*
 * problema.c
 *
 *  Created on: 08/10/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "problema.h"

int xor(int a, int b){
	//Si son iguales dame 0, si no, dame 1.
	if(a == b){
		return 0;
	}else{
		return 1;
	}
	//return a==b ? 0:1;
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
		//y[i] = i%2 ? 1:0;
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
