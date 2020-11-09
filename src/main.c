#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "rand.h"
#include "genetico.h"
#include "transformada.h"
#include "mem_structs.h"

/**************COMANDO GNUPLOT******************/
/*
 * set title 'Genotipo de 32 bits'
 * set grid
 * set autoscale
 * plot '32bits.txt' u 1:2 w lp lt 1 lw 2 t "Genetico Simple",'32bits.txt' u 1:3 w lp lt 2 lw 3 t "Genetico elitismo", '32bits.txt' u 1:4 w lp lt 3 lw 2 t "Genetico Miu+Lambda"
 *
 * set title 'Genotipo de 64 bits'
 * set grid
 * set autoscale
 * plot '64bits.txt' u 1:2 w lp lt 1 lw 2 t "Genetico Simple",'64bits.txt' u 1:3 w lp lt 2 lw 3 t "Genetico elitismo", '64bits.txt' u 1:4 w lp lt 3 lw 2 t "Genetico Miu+Lambda"
 *
 * set title 'Genotipo de 128 bits'
 * set grid
 * set autoscale
 * plot '128bits.txt' u 1:2 w lp lt 1 lw 2 t "Genetico Simple",'128bits.txt' u 1:3 w lp lt 2 lw 3 t "Genetico elitismo", '128bits.txt' u 1:4 w lp lt 3 lw 2 t "Genetico Miu+Lambda"
*/

MOP mop;
GA ga;
int n;

int main(int argc, char *argv[]){

	POBLACION P,Q,T;
	size_t i=0, mejor_p, peor_q;
	int alg, longitud, poblacion, gmax;
	char algoritmo, r, p[10], max[10];
	FILE* file;

	printf("\t1.- Genetico simple\n");
	printf("\t2.- Genetico con elitismo\n");
	printf("\t3.- Genetico Mu + Lambda\n");
	do {
		printf("Elige el algoritmo a ejecutar (1,2,3): ");
		scanf("%c%*c", &algoritmo);
		alg = atoi(&algoritmo);
		//printf("%d\n", alg);
		if (alg<1 || alg>3)
			printf("\tOpción invalida\n");
	} while(alg<1 || alg>3 );


	do {
		printf("Ingrese exponente (4,6,8) para la longitud de la cadena binaria (genotipo): ");
		scanf("%c%*c", &r);
		longitud = atoi(&r);
		//printf("%d\n", longitud);
		if (longitud!=4 && longitud!=6 && longitud!=8)
			printf("\tOpción invalida\n");
	} while(longitud!=4 && longitud!=6 && longitud!=8);


	do {
		printf("\nIngrese el tamaño de la población: ");
		scanf("%s", p);
		poblacion = atoi(p);
		//printf("%d\n", poblacion);
		if (poblacion%2 != 0 || poblacion == 0 )
			printf("\tIngrese un número par\n");
	} while(poblacion%2 != 0 || poblacion == 0);


	do {
		printf("\nIngrese el máximo de generaciones: ");
		scanf("%s", max);
		gmax = atoi(max);
		//printf("%d\n", gmax);
		if (gmax == 0 )
			printf("\tIngrese un número \n");
	} while(gmax == 0);


	n = longitud;
	mop.nbin = pow(2, n);
	mop.nobj = 1;

	ga.psize = poblacion;
	ga.Pc = 0.9;
	ga.Pm = 1.0/mop.nbin;
	ga.Gmax = gmax;

	alloc_pop(&P, ga.psize);
	alloc_pop(&Q, ga.psize);
	alloc_pop(&T, ga.psize*2);

	randomize(0.5);

	Inicializar(&P);
	Evaluacion(&P);

	switch(alg){
		case 1:
			file = fopen("simple.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico Simple \033[0m \n\n");
				for(i=0 ; i<ga.Gmax ; i++){
					fprintf(file,"%zu\t", i);
					estadisticas(&P, i, file);
					Crossover(&P,&Q, ga.Pc);
					Mutacion(&Q, ga.Pm);
					Evaluacion(&Q);
					cpy_pop(&P, &Q);
				}
				fclose(file);
				printf("\n\t\t\t \033[1;42m Genetico Simple terminado \033[0m \n\n");
		break;
		case 2:
			file = fopen("elitismo.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico con elitismo \033[0m \n\n");
			for(i=0 ; i<ga.Gmax ; i++){
				fprintf(file,"%zu\t", i);
				estadisticas(&P, i, file);
				Crossover(&P,&Q, ga.Pc);
				Mutacion(&Q, ga.Pm);
				Evaluacion(&Q);
				/*********************APLICANDO ELITISMO**************************/
				mejor_p = Mejor_solucion(&P); 				//Obtener indice de la mejor solucion en P
				peor_q = Peor_solucion(&Q); 				//Obtener indice de la peor solucion en Q
				cpy_ind(&Q.ind[peor_q], &P.ind[mejor_p]);	//Reemplazando.
				cpy_pop(&P, &Q);
			}
			fclose(file);
			printf("\n\t\t\t \033[1;42m Genetico con elitismo terminado \033[0m \n\n");
		break;
		case 3:
			file = fopen("miulambda.txt", "w");
			printf("\n\t\t\t \033[1;42m Genetico Miu plus Lambda \033[0m \n\n");
			for(i=0 ; i<ga.Gmax; i++){
				fprintf(file,"%zu\t", i);
				estadisticas(&P, i, file);
				Crossover(&P,&Q, ga.Pc);
				Mutacion(&Q, ga.Pm);
				Evaluacion(&Q);
				/*********************APLICANDO Miu + Lambda**************************/
				Unir_poblaciones(&P,&Q,&T);
				Seleccionar_mejores(&T,&P);
			}
			fclose(file);
			printf("\n\t\t\t \033[1;42m Genetico Miu plus Lambda terminado \033[0m \n\n");
		break;
		default:
			printf("Opción inválida\n");
		break;
	}

	free_pop(&T);
	free_pop(&P);
	free_pop(&Q);
	return 0;
}
