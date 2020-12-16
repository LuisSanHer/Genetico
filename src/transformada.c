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

void fwht_transform(const int *src, int *dst, int n) {
    int *a = (int*)malloc(sizeof(int)*n);
    int *b = (int*)malloc(sizeof(int)*n);
    void *tmp;
    memcpy(a, src, sizeof(int)*n);
    int max_unex=-1;
    // Fast Walsh Hadamard Transform.
    int i, j, s;
    int t;
    for (i = n>>1; i > 0; i>>=1) // i=4  ----->  i<<1 :     00000000000000001000
    {
        for (j = 0; j < n; j++)
        {
            s = j/i%2;
            b[j]=a[(s?-i:0)+j]+(s?-1:1)*a[(s?0:i)+j];
        }
        tmp = a;
        a = b;
        b = tmp;
    }
    memcpy(dst, a, sizeof(int)*n);
    free(a);
    free(b);
}

void funcion(INDIVIDUO *p){
  int h = 1;
  int x, y;
  int *aux = (int*)malloc(sizeof(int)*mop.nbin);
  //Transformada rapida de Walsh Hadamard
  fwht_transform(p->x, aux, mop.nbin);
  //Obtener el maximo valor absoluto, sin tomar en cuenta el primero
  int mayor = 0;
  for (size_t i=1 ; i<mop.nbin ; i++) {
    if(mayor < iabs(aux[i])){
      mayor = iabs(aux[i]);
    }
  }
  //CLARK 2005: ECUACION 6
  int temp = -mayor;
  p->f = - pow(2, n-1) - temp; //No linealidad completa sin dividir max R= 112
                               //(CORRECTA) hasta cierto punto...
                               //Es la que se acerca a los resultados de Burnett.

  //p->f = - ((pow(2, n) - temp)/2.0); //No linealidad completa R= 192
  //p->f = - ((pow(2, n) - mayor)/2.0); //No linealidad completa R= 120
  //p->f = -mayor/2.0; //Mayor valor absoluto/2.0 R= 64

  free(aux);
}
