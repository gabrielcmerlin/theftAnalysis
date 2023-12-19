// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração da função do Quick Sort para ordenação de vetores e da função de troca (swap).
*/

#ifndef ORDENACAO_H
#define ORDENACAO_H
#include <stdio.h>

// Ordena os elementos de um vetor utilizando o algoritmo de Quick Sort.
void quick_sort(void **vetor, int inicio, int fim, char *tipo_dado);

// Troca a posição de dois elementos de um vetor, dados seus índices.
void swap(void **vetor, int a, int b, char *tipo_dado);

#endif