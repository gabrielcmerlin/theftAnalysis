// Organização de Arquivos (SCC0215) - Trabalho 1
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das funções relacionadas às buscas binária e sequencial utilizadas no trabalho, 
  para cada tipo de funcionalidade (imprimir e deletar).
*/

#ifndef BUSCA_H
#define BUSCA_H

#include "utils.h"
#include "criterios.h"

// Realiza a chamada da etapa recursiva da busca binária.
int busca_binaria(FILE *arq_dados, void **vetor, int ini, int fim, char *valor, char *tipo_dado);

// Realiza a busca de um registro de acordo com um campo indexado (busca binária em um vetor) e sua impressão/deleção.
void buscar_indexado(FILE *arq_dados, void** vetor_indice, criterio *vetor_condicoes, int tam_vetor_indice, 
                              char *tipo_dado, char *nome_campo_condicao, int *reg_impressos, int qntd_condicoes,
                              char *campo_indexado, int flag);

// Realiza a busca de um registro de acordo com um campo não indexado (busca sequencial no arquivo) e sua impressão/deleção. 
void buscar_nao_indexado(FILE *arq_dados, criterio *vetor_condicoes, int *contador, int qntd_condicoes,
                                  int tam_vetor_indice, char *tipo_dado, char *campo_indexado, void** vetor_indice, int flag);

// Realiza a busca de um registro de acordo com um campo não indexado (busca binária em um vetor) e atualiza-o.
// Retorna o vetor de índice para facilitar a desalocação e evitar perda de referência.
void **buscar_indexado_atualizar(FILE *arq_dados, void** vetor_indice, criterio *vetor_condicoes, int *tam_vetor_indice, 
                               char *tipo_dado, char *nome_campo_condicao, int qntd_condicoes, char *campo_indexado,
                               criterio *vetor_atualizacoes, int qntd_campos_atualizados, reg_cabecalho *C);

// Realiza a busca de um registro de acordo com um campo não indexado (busca sequencial no arquivo) e atualiza-o. 
// Retorna o vetor de índice para facilitar a desalocação e evitar perda de referência.
void **buscar_nao_indexado_atualizar(FILE *arq_dados, void **vetor_indice, criterio *vetor_condicoes, 
                                   int qntd_condicoes, int *tam_vetor_indice, char *tipo_dado, char* campo_indexado,
                                   criterio *vetor_atualizacoes, int qntd_campos_atualizados, reg_cabecalho *C, int tam_cabecalho);

void buscar_b_estrela();                      

#endif