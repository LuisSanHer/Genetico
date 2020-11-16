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

int iabs(int a){
  if(a<0)
    return -a;
  return a;
}

void funcion(INDIVIDUO *p){
  int h = 1;
  int x, y;
  int *aux = (int*)malloc(sizeof(int)*mop.nbin);
  //Transformada rapida de Walsh Hadamard
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

  //Obtener el maximo valor absoluto
  int mayor = 0;
  for (size_t i=1 ; i<mop.nbin ; i++) {
    if(mayor < iabs(aux[i])){
      mayor = iabs(aux[i]);
    }
  }
  //CLARK 2005: ECUACION 6
  //int temp = -mayor;
  //p->f = - ((pow(2, n) - temp)/2.0); //No linealidad completa R= 192
  //p->f = - ((pow(2, n) - mayor)/2.0); //No linealidad completa R= 120
  //p->f = - pow(2, n-1) - temp; //No linealidad completa sin dividir max R= 112
  p->f = -mayor/2.0; //Mayor valor absoluto/2.0 R= 64

  free(aux);
}
