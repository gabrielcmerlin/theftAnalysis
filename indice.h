// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração do tipo para registros de cabeçalho e de dados de índice e suas funções 
  relacionadas, como retornar e salvar campos; alocar e desalocar registro; ler e escrever registro; criar vetor.
  Além disso, funções relacionadas ao vetor de índices, como criar, preencher, carregar, 'shiftar' e desalocar.
*/

#ifndef INDICE_H
#define INDICE_H

#include <stdio.h>
#include "dados.h"
#include "cabecalho.h"
#include "criterios.h"

// Define registro de cabeçalho de índice.
typedef struct _indice_cabecalho indice_cabecalho;

// Define registro de dados do tipo 'int' de índice.
typedef struct _indice_dados_int indice_dados_int;

// Define registro de dados do tipo 'string' de índice.
typedef struct _indice_dados_string indice_dados_string;

// Aloca um registro de cabeçalho de índice.
indice_cabecalho *alocar_indice_cabecalho();

// Desaloca um registro de cabeçalho de índice.
indice_cabecalho *desalocar_indice_cabecalho(indice_cabecalho **Ci);

// Aloca um registro de dados do tipo 'int' de índice.
indice_dados_int *alocar_indice_dados_int();

// Desaloca um registro de dados do tipo 'int' de índice.
indice_dados_int *desalocar_indice_dados_int(indice_dados_int **Di);

// Aloca um registro de dados do tipo 'string' de índice.
indice_dados_string *alocar_indice_dados_string();

// Desaloca um registro de dados do tipo 'string' de índice.
indice_dados_string *desalocar_indice_dados_string(indice_dados_string **Di);

// Limpa campo 'chave_busca' de registro de dados do tipo 'string' de índice.
void limpar_campos_indice_dados_str(indice_dados_string *Di);

// Atualiza o status de um registro de cabeçalho de índice.
void salvar_status_indice(indice_cabecalho *Ci, char status);

// Atualiza a quantidade de registros no arquivo de um registro de cabeçalho de índice.
void salvar_qntd_reg_indice(indice_cabecalho *Ci, int qntd_reg);

// Retorna o status do cabeçalho de índice.
char retornar_status_indice(indice_cabecalho *C_indice);

// Retorna a quantidade de registros do cabeçalho de índice.
int retornar_qntd_reg_indice(indice_cabecalho *C_indice);

// Atualiza a chave de busca (do tipo inteiro) de um registro de dados de índice.
void salvar_chave_busca_int(indice_dados_int *Di, int chave_busca);

// Atualiza o campo 'byte_offset' de um registro de dados de índice.
void salvar_byte_offset_int(indice_dados_int *Di, long int byte_offset);

// Atualiza a chave de busca (do tipo inteiro) de um registro de dados de índice.
void salvar_chave_busca_string(indice_dados_string *Di, char* chave_busca);

// Atualiza o campo 'byte_offset' de um registro de dados de índice.
void salvar_byte_offset_string(indice_dados_string *Di, long int byte_offset);

// Retorna a chave de busca (do tipo 'string') de um registro de dados de índice.
char *retornar_chave_busca_string(indice_dados_string *Di);

// Retorna o campo 'byte_offset' de um registro de dados do tipo 'string' de índice.
long int retornar_byte_offset_string(indice_dados_string *Di);

// Retorna a chave de busca (do tipo 'int') de um registro de dados de índice.
int retornar_chave_busca_int(indice_dados_int *Di);

// Retorna o campo 'byte_offset' de um registro de dados do tipo 'int' de índice.
long int retornar_byte_offset_int(indice_dados_int *Di);

// Retorna a chave busca de um registro de dados de índice.
void *retornar_chave_busca(void *Di, char *nome_campo);

long int retornar_byte_offset(void *Di, char *nome_campo);

// Atualiza e escreve o status de um arquivo de índice.
void atualizar_status_indice(FILE *arq_indice, indice_cabecalho *C_indice, char status);

// Limpa o campo 'chave_busca' do tipo 'string' de um registor de dados de índice.
void limpar_chave_busca_string(indice_dados_string *D);

// Desaloca um vetor de registros de dados de indíce.
void desalocar_vetor_indice(void **vetor, int qntd_reg_indice, char *tipo_dado);

// Cria um arquivo de indíce, escrevendo o cabeçalho e os registros de dados de índice.
void criar_arquivo_indice(void **vetor, int qntd_reg_indice, char* nome_indice, char *tipo_dado);

// Retorna um vetor de registros de dados de índice (do tipo 'string') preenchido com um registro, dados os campos.
indice_dados_string **preencher_vetor_string(void **vetor, int *qntd_reg_indice, char* chave_busca, int bytes_percorridos);

// Retorna um vetor de registros de dados de índice (do tipo 'int') preenchido com um registro, dados os campos.
indice_dados_int **preencher_vetor_int(void **vetor, int *qntd_reg_indice, int chave_busca, int bytes_percorridos);

// Escreve um registro de cabeçalho de índice em um arquivo de índice.
int escrever_indice_cabecalho(FILE *arq_indice, indice_cabecalho *Ci);

// Escreve um registro de dados de índice em um arquivo de índice.
int escrever_indice_dados_int(FILE *arq_indice, indice_dados_int *Di);

// Escreve um registro de dados de índice do tipo 'string' em um arquivo de índice.
int escrever_indice_dados_string(FILE *arq_indice, indice_dados_string *Di);

// Escreve um registro de dados de índice em um arquivo de índice.
void escrever_indice_dados(FILE *arq_indice, void **vetor, int qntd_reg_indice, char *tipo_dado);

// Carrega o arquivo de índice para a memória principal.
void **carregar_arq_indice(FILE *arq_indice, int *tam_vetor_indice, char *tipo_dado);

// Salva o byte offset de um registro de dados de índice.
void salvar_byte_offset(void *Di, long int byte_offset, char *tipo_dado);

// Compara uma chave de busca com um valor dado. Retorna true se são iguais. Caso contrário, retorna false.
bool comparar_chave_busca(void *chave_busca, char *valor_comparador, char *nome_campo);

// Realoca memória do vetor de índice por meio da verificação dos registros logicamente removidos.
// Coloca os registros removidos no fim do vetor, reordena os registros válidos e libera a memória dos inválidos.
void **shift_vetor_indice(void **vetor_indice, int tam_vetor_indice, int qntd_reg_rem, char *tipo_dado);

// Insere um registro de dados de índice do tipo 'int' em um vetor de índices, verificando sua chave.
void inserir_vetor_indice_int(indice_dados_int **vetor, int chave_busca, int *qntd_reg, indice_dados_int *D_indice);

// Insere um registro de dados de índice do tipo 'string' em um vetor de índices, verificando sua chave.
void inserir_vetor_indice_string(indice_dados_string **vetor, char *chave_busca, int *qntd_reg, indice_dados_string *D_indice);

// Realiza a deleção de um registro de dados do arquivo de dados e do vetor de índice.
void realizar_delecao(FILE *arq_dados, reg_dados *D, long int byte_offset, void **vetor_indice, int rrn_indice, 
                      int tam_vetor_indice, char *campo_indexado, char *tipo_dado);

// Insere registros em um arquivo de dados e em um vetor de índices. Retorna o vetor.
void **realizar_insercao(FILE *arq_dados, void **vetor, int *qntd_reg, long int *prox_byte_offset, int qntd_insercoes, 
                         char *tipo_dado, char* campo_indexado);

// Atualiza os campos de um registro de dados no arquivo de dados e seu índice no vetor de índices.
void **realizar_atualizacao(FILE *arq_dados, reg_dados *D, long int byte_offset, criterio *vetor_atualizacoes, int qntd_campos_atualizados, reg_cabecalho *C, void **vetor_indice, char *tipo_dado, char *campo_indexado, int *tam_vetor_indice);

#endif