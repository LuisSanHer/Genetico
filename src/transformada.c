/*  Implementación de la transformada rápida de Wals Hadammar
 * transformada.c
 *
 *  Created on: 03/11/2020
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "transformada.h"

void funcion(INDIVIDUO *p){
  int h = 1;
  int x, y;
  int *aux = (int*)malloc(sizeof(int)*mop.nbin);
  memcpy(aux, p->x,sizeof(int)*mop.nbin);
  while(h < mop.nbin){
    for (size_t i=0 ; i<mop.nbin ; i = (i+h*2)) {
      for (size_t j=i ; j<i+h ; j++) {
        x = aux[j];
        y = aux[j+h];
        aux[j] = x+y;
        aux[j+h] = x-y;
      }
    }
    h *= 2;
  }
  int mayor = 0;
  for (size_t i=0 ; i<mop.nbin ; i++) {
    if(mayor < labs(aux[i])){
      mayor = labs(aux[i]);
    }
  }
  //CLARK 2005: ECUACIÓN 6
  p->f = -1 * ( pow(2, n) - mayor)/2.0 ;
  free(aux);
}
