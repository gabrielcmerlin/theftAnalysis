// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das funções relacionadas aos critérios de busca, como criar vetor, desalocar vetor
  e ler critério. Um critério armazena um nome de campo e um valor para o campo e é utilizado como critério de busca e
  atualização.
*/

#ifndef CRITERIOS_H
#define CRITERIOS_H

// Definindo struct para critério.
typedef struct _criterio criterio;
struct _criterio {
  char *nome_campo;
  char *valor_buscado;
};

// Cria e preenche um vetor de condições. Caso haja uma condição igual ao campo indexado, coloca em primeiro no vetor.
criterio *criar_vetor_criterios(char *nome_campo_criterio, char *valor_campo_criterio_string, int qntd_criterios, char *campo_indexado);

// Desaloca um vetor de condições.
void desalocar_vetor_criterios(criterio *vetor_criterios, int qntd_criterios);

#endif