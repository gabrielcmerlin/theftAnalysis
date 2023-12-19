// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funções relacionadas aos critérios de busca, como criar vetor, desalocar vetor
  e ler critério. Um critério armazena um nome de campo e um valor para o campo e é utilizado como critério de busca e
  atualização.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "criterios.h"

// Lê e trata um critério.
void ler_criterio(char *nome_campo_criterio, char *valor_campo_criterio) {
  scanf(" %s", nome_campo_criterio);

  if (strcmp(nome_campo_criterio, "idCrime") == 0 || strcmp(nome_campo_criterio, "numeroArtigo") == 0) { // tipo_dado == int
    int aux;
    scanf(" %d", &aux);
    int_para_string(aux, &valor_campo_criterio);
  } else { // tipo_dado == string
    scan_quote_string((char *) valor_campo_criterio);
  }
}

// Cria e preenche um vetor de critério. Caso haja uma critério igual ao campo indexado, coloca em primeiro no vetor.
criterio *criar_vetor_criterios(char *nome_campo_criterio, char *valor_campo_criterio_string, int qntd_criterios, char *campo_indexado){
  criterio *vetor_criterios = (criterio *) malloc(qntd_criterios * sizeof(criterio));

  // Preenchendo vetor de critérios.
  for (int i = 0; i < qntd_criterios; i++) {
    ler_criterio(nome_campo_criterio, valor_campo_criterio_string);

    vetor_criterios[i].nome_campo = (char *) malloc((strlen(nome_campo_criterio) + 1) * sizeof(char));
    strcpy(vetor_criterios[i].nome_campo, nome_campo_criterio);
    vetor_criterios[i].valor_buscado = (char *) malloc((strlen(valor_campo_criterio_string) + 1) * sizeof(char));
    strcpy(vetor_criterios[i].valor_buscado, valor_campo_criterio_string);
  }

  // Colocando crtiério igual ao campo indexado em primeiro, caso exista.
  criterio aux;
  for (int i = 1; i < qntd_criterios; i++){
    if(strcmp(vetor_criterios[i].nome_campo, campo_indexado) == 0){
      aux = vetor_criterios[0];
      vetor_criterios[0] = vetor_criterios[i];
      vetor_criterios[i] = aux;
    }
  }

  // Retornando vetor de critérios.
  return vetor_criterios;
}

// Desaloca um vetor de critérios.
void desalocar_vetor_criterios(criterio *vetor_criterios, int qntd_criterios){
  for(int j = 0; j < qntd_criterios; j++) {
      free(vetor_criterios[j].nome_campo);
      free(vetor_criterios[j].valor_buscado);
    }
    free(vetor_criterios);
}