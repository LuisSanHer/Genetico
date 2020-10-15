#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "rand.h"
#include "genetico.h"
#include "problema.h"
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

int main(int argc, char *argv[]){

	srand(time(NULL));

	POBLACION P,Q,T;
	size_t i=0, mejor_p, peor_q;
	FILE* file;

	int algoritmo, r;
	algoritmo = *argv[1] - 48;
	printf("Ingrese la longitud (32,64,128) de la cadena binaria (genotipo): ");
	scanf("%d", &r);


	mop.nbin = r;
	mop.nobj = 1;

	ga.psize = 100;
	ga.Pc = 0.9;
	ga.Pm = 1.0 / mop.nbin;
	ga.Gmax = 100;

	alloc_pop(&P, ga.psize);
	alloc_pop(&Q, ga.psize);
	alloc_pop(&T, ga.psize*2);

	int semilla = rand()%10; //Aleatorio entre 0 y 10
	randomize(semilla/10);

	Inicializar(&P);
	Evaluacion(&P);

	switch(algoritmo){
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
			printf("\n\t\t\t \033[1;42m Genetico Miu plus Lambda terminado \033[0m \n\n");
		break;
		default:
			printf("Opción inválida\n");
		break;
	}

	fclose(file);
	free_pop(&T);
	free_pop(&P);
	free_pop(&Q);
	return 0;
}
