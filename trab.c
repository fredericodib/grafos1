/*
Amanda Oliveira Alves 15/0116276
Frederico Pinheiro Dib 15/0125925
Copilar: gcc -ansi -Wall -o trab trab.c readgml.c
*/

#include <stdio.h>
#include "readgml.h"
#include <stdlib.h>


/*Funcao que copia o vetor de vertices, ordena de forma decrescente de acordo com o
grau e imprime na tela*/
void decrescente_degree(VERTEX *vertex, int tam) {
	int i, j;
	VERTEX *vertexaux, aux;
	vertexaux = malloc(tam*sizeof(VERTEX));
	for(i=0;i<tam;i++) { /*Copia vetor de vertices para um vetor auxiliar*/
		vertexaux[i] = vertex[i];
	}
	for(i=0;i<tam;i++){ /*Ordena vetor auxiliar*/
		for(j=0;j<tam-1;j++){
			if(vertexaux[j+1].degree > vertexaux[j].degree){
				aux = vertexaux[j];
				vertexaux[j] = vertexaux[j+1];
				vertexaux[j+1] = aux;
			}
		}
	}
	printf("Ordem dos vertices decrescente por grau:\n");
	for(i=0;i<tam;i++) { /*Imprime na tela vetor ordenado*/
		printf("grau: %d, id: %d\n", vertexaux[i].degree, vertexaux[i].id);
	}
}

/*Função que ordena o vetor recebido*/
void ordena_vetor_int(int *vetor, int size) {
	int i, j, aux;

	for(i=0;i<size;i++){ /*Ordena vetor auxiliar*/
		for(j=0;j<size-1;j++){
			if(vetor[j+1] < vetor[j]){
				aux = vetor[j];
				vetor[j] = vetor[j+1];
				vetor[j+1] = aux;
			}
		}
	}
}

/*Função que compara dois vetores e retorna 0 se forem iguais e 1 se forem diferentes*/
int compara_vetor(int *vetor1, int *vetor2, int tamanho) {
	int i;

	for(i=0;i<=tamanho;i++) {
		if(vetor1[i] != vetor2[i]) {
			return 1;
		}
	}

	return 0;
}

/*Função que inicializa o vetor "r" e "p"*/
void iniciar_pr(int *p, int *r, NETWORK network, int vertice) {
	int i, size;

	for(i=0;i<34;i++) {
		p[i] = -1;
		r[i] = -1;
	}

	r[0] = vertice; /*Grava no "r" o indice do vertice inicial*/
	size = network.vertex[vertice].degree;
	for(i=0;i<size;i++) { /*Grava em "p" todos os ligantes do vertice inicial.*/
		p[i] = network.vertex[vertice].edge[i].target;
	}
}

/*Função que dado o vetor "p" e "r", ele retorna um clique maximal do grafo dentro do vetor "r"*/
void acha_maximal(int *p, int *r, NETWORK network) {
	int i=0, j, g, h, cont=0, sizeR=1;

	while(p[i]!=-1) { /*Checa para cada vertice gravado em "p"*/
		g = p[i];
		cont = 0;
		for(j=0;j<network.vertex[g].degree;j++) {
			for(h=0;h<sizeR;h++) {
				if (r[h] == network.vertex[g].edge[j].target) {
					cont++; /*A cada ligação do vertice "g"
					com algum vertice gravado no vetor "r", o contador aumenta*/
				}
			}
		}
		if (cont == sizeR) {  /*Caso o tamanho do contador seja igual ao tamanho de "r", 
			significa que o vertice "g" se liga a todos os vertices do clique, logo, ele 
			se junta ao clique.*/
			r[sizeR] = g;
			sizeR++;
		}
		i++;
	}

}

/*Recebe um vetor r que representa um clique e imprime na tela.*/
void imprime_maximal(int *r, int *aux ,NETWORK network, int *flag) {
	int i, cont=0;

	i=0;
	while(r[i] != -1) {
		cont++;
		i++;
	}

	if(cont >=5) { /*So faz a impressão se o clique tiver ao menos tamanho 5*/
		ordena_vetor_int(r, cont); /*Função que ordena o vetor "r"*/

		if((*flag == 0)  || (compara_vetor(r, aux, cont) != 0)) { /*So ira imprimir o primeiro 
		clique ou algum que seja diferente do primeiro*/
			printf("\nClique encontrado: { ");
			i=0;
			while(r[i] != -1) {
				printf("%d ", r[i] + 1);
				aux[i] = r[i];
				i++;
			}
			aux[i] = -1;
			printf("}\n");
			*flag = 1;
		}
	}
}

int main() {
	system("clear");
	NETWORK network; 
	FILE *fp;
	int *p, *r, *aux, i, flag=0;
	fp = fopen("karate.gml", "r");
	read_network(&network, fp); /*Gera o grafo com base no arquivo karate.gml*/
	p = malloc(network.nvertices*sizeof(int));
	r = malloc(network.nvertices*sizeof(int));
	aux = malloc(network.nvertices*sizeof(int));

	decrescente_degree(network.vertex, network.nvertices);

	for(i=0;i<34;i++) {
		iniciar_pr(p, r, network, i);
		acha_maximal(p, r, network);
		imprime_maximal(r, aux, network, &flag);
	}
	
	return 0;
}