// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação do algoritmo de Quick Sort para ordenação de vetores e suas funções auxiliares.
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "ordenacao.h"
#include "indice.h"
#include "utils.h"

// Compara dois elementos de um vetor, dados seus índices. Retorna true se o primeiro elemento é maior. Caso contrário, retorna false.
bool compara(void **vetor, int a, int b, char *tipo_dado) {
    if (strcmp(tipo_dado, "inteiro") == 0) {
        indice_dados_int ** vetor_aux = (indice_dados_int **) vetor;

        if (retornar_chave_busca_int(vetor_aux[a]) > retornar_chave_busca_int(vetor_aux[b])) {
            return true;
        } else if(retornar_chave_busca_int(vetor_aux[a]) == retornar_chave_busca_int(vetor_aux[b])) {
            if (retornar_byte_offset_int(vetor_aux[a]) > retornar_byte_offset_int(vetor_aux[b])) {
                return true;
            }
        }

        return false;
    } else { // tipo_dado == string
        indice_dados_string ** vetor_aux = (indice_dados_string **) vetor;

        if (strncmp(retornar_chave_busca_string(vetor_aux[a]), retornar_chave_busca_string(vetor_aux[b]), 12) > 0) {
            return true;
        } else if(strncmp(retornar_chave_busca_string(vetor_aux[a]), retornar_chave_busca_string(vetor_aux[b]), 12) == 0) {
            if (retornar_byte_offset_string(vetor_aux[a]) > retornar_byte_offset_string(vetor_aux[b])) {
                return true;
            }
            return false;
        }

        return false;
    }
}

// Troca a posição de dois elementos de um vetor, dados seus índices.
void swap(void **vetor, int a, int b, char *tipo_dado){
    if (strcmp(tipo_dado, "inteiro") == 0) {
        indice_dados_int ** vetor_aux = (indice_dados_int **) vetor;

        indice_dados_int *temp = vetor_aux[a];
        vetor_aux[a] = vetor_aux[b];
        vetor_aux[b] = temp;
    } else { // tipo_dado == string
        indice_dados_string ** vetor_aux = (indice_dados_string **) vetor;

        indice_dados_string *temp = vetor_aux[a];
        vetor_aux[a] = vetor_aux[b];
        vetor_aux[b] = temp;
    }
}

// Retorna um número aleatório para ser o pivô do Quick Sort.
int escolher_pivo_aleatorio(int inicio, int fim){
    return (rand() % (fim - inicio + 1) + inicio);
}

// Realiza a etapa de partição do algoritmo de Quick Sort e retorna o novo pivô.
int particionar(void **vetor, int inicio, int fim, int pivo, char *tipo_dado){
    // Escolhemos um pivo que pode estar em qualquer posição do vetor, então realizamos esse swap a fim transformar todos os casos naquele que usamos a posição inicial
    swap(vetor, pivo, inicio, tipo_dado);

    // Inicializando as variáveis que contêm a posição do pivo, e dos iteradores 'i' e 'j'
    pivo = inicio;
    int i = inicio + 1;
    int j = fim;
  
    // Loop que encontra a posição certa para o pivo
	while (i <= j) {
        // Incrementamos 'i' enquanto o pivo for maior que 'i'
		while (i <= fim && !compara(vetor, i, pivo, tipo_dado)) i++;
        // Decrementamos 'j' enquanto o pivo for menor que 'j'
		while (compara(vetor, j, pivo, tipo_dado)) j--;

        // Caso o iterador 'j' tenha superado 'i', fazemos um swap entre seus números
	    if (j > i){
			swap(vetor, i, j, tipo_dado);
		}
	}

    // Atualizamos o pivo e realizamos um swap entre os números do novo pivo e de inicio (velho pivo) 
    pivo = j;
    swap(vetor, pivo, inicio, tipo_dado);

    return pivo;
}

// Ordena os elementos de um vetor utilizando o algoritmo de Quick Sort.
void quick_sort(void **vetor, int inicio, int fim, char *tipo_dado){
    // Parte 1: Condição de parada
    if (vetor == NULL || fim <= inicio) return;

    // Parte 2: Escolher pivo
    int pivo = escolher_pivo_aleatorio(inicio, fim);

    // Parte 3: Particionar
    pivo = particionar(vetor, inicio, fim, pivo, tipo_dado);

    // Parte 4: Passo recursivo
    quick_sort(vetor, inicio, pivo - 1, tipo_dado);
    quick_sort(vetor, pivo + 1, fim, tipo_dado);
}
