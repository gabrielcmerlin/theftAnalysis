// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação do tipo para registro de cabeçalho e suas funções relacionadas, como retornar e 
  salvar campos; alocar e desalocar registro; ler e escrever registro; imprimir registro.
*/

#include <stdlib.h>

#include "cabecalho.h"
#include "utils.h"

// Define registro de cabeçalho.
struct _reg_cabecalho {
  char status;
  long int prox_byte_offset;
  int nro_reg_arq;
  int nro_reg_rem;
};

// Aloca um registro de cabecalho.
reg_cabecalho *alocar_reg_cabecalho(){
  reg_cabecalho *C = (reg_cabecalho *) calloc(1, sizeof(reg_cabecalho));

  return C;
}

// Desaloca um registro de cabeçalho.
void desalocar_reg_cabecalho(reg_cabecalho **C) {
  if(C != NULL && *C != NULL) {
    free(*C);
    *C = NULL;
  }
}

// Atualiza o status de um registro de cabeçalho.
void salvar_status(reg_cabecalho *C, char status) {
  C->status = status;
}

// Atualiza o próximo byte offset de um registro de cabeçalho.
void salvar_prox_byte_offset(reg_cabecalho *C, long int prox_byte_offset) {
  C->prox_byte_offset = prox_byte_offset;
}

// Atualiza o número de registros em um arquivo de um registro de cabeçalho.
void salvar_nro_reg_arq(reg_cabecalho *C, int nro_reg_arq) {
  C->nro_reg_arq = nro_reg_arq;
}

// Atualiza o número de registros logicamente removidos de um registro de cabeçalho.
void salvar_nro_reg_rem(reg_cabecalho *C, int nro_reg_rem) {
  C->nro_reg_rem = nro_reg_rem;
}

// Retorna o status de um registro de cabeçalho.
char retornar_status(reg_cabecalho *C) {
  return C->status;
}

// Retorna o próximo byte offset de um registro de cabeçalho.
long int retornar_prox_byte_offset(reg_cabecalho *C) {
  return C->prox_byte_offset;
}

// Retorna o número de registros em um arquivo de um registro de cabeçalho.
int retornar_nro_reg_arq(reg_cabecalho *C) {
  return C->nro_reg_arq;
}

// Retorna o número de registros logicamente removidos de um registro de cabeçalho.
int retornar_nro_reg_rem(reg_cabecalho *C) {
  return C->nro_reg_rem;
}

// Escreve status de um registro de cabeçalho em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_status(FILE *arq_dados, reg_cabecalho *C, int *bytes_escritos){
  char status = retornar_status(C);
  *bytes_escritos += fwrite(&status, sizeof(char), 1, arq_dados);
}

// Escreve campo 'proxByteOffset' de um registro de cabeçalho em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_prox_byte_offset(FILE *arq_dados, reg_cabecalho *C, int *bytes_escritos){
  long int prox_byte_offset = retornar_prox_byte_offset(C);
  *bytes_escritos += sizeof(long int) * fwrite(&prox_byte_offset, sizeof(long int), 1, arq_dados);
}

// Escreve o número de registros (armazenado no cabeçalho) em um arquivo binário. Retorna o nº de bytes escritos. 
int escrever_nro_reg_arq(FILE *arq_dados, reg_cabecalho *C, int *bytes_escritos){
  int nro_reg_arq = retornar_nro_reg_arq(C);
  *bytes_escritos += sizeof(int) * fwrite(&nro_reg_arq, sizeof(int), 1, arq_dados);
}

// Escreve o número de registros removidos (armazenado no cabeçalho) em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_nro_reg_rem(FILE *arq_dados, reg_cabecalho *C, int *bytes_escritos){
  int nro_reg_rem = retornar_nro_reg_rem(C);
  *bytes_escritos += sizeof(int) * fwrite(&nro_reg_rem, sizeof(int), 1, arq_dados);
}

// Escreve um registro de cabeçalho em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_reg_cabecalho(FILE *arq_dados, reg_cabecalho *C) {
  int bytes_escritos = 0; /*Contador para bytes escritos*/ 

  // Escrevendo informações do registro de cabeçalho no arquivo binário
  // e contando a quantidade de bytes escritos.
  escrever_status(arq_dados, C, &bytes_escritos);
  escrever_prox_byte_offset(arq_dados, C, &bytes_escritos);
  escrever_nro_reg_arq(arq_dados, C, &bytes_escritos);
  escrever_nro_reg_rem(arq_dados, C, &bytes_escritos);

  // Retornando a quantidade de bytes escritos.
  return bytes_escritos;
}

// Lê o campo 'proxByteOffset' de um arquivo binário e salva em um registro de cabecalho.
void ler_prox_byte_offset(FILE *arq_dados, reg_cabecalho *C, int *bytes_percorridos) {
  long int *offset = (long int *) malloc(1 * sizeof(long int));

  *bytes_percorridos += sizeof(long int) * fread(offset, sizeof(long int), 1, arq_dados);
  salvar_prox_byte_offset(C, *offset);

  free(offset);
}

// Lê o campo 'nro_reg_arq' de um arquivo binário e salva em um registro de dados.
void ler_nro_reg_arq(FILE *arq_dados, reg_cabecalho *C, int *bytes_percorridos) {
  int *aux = (int *) malloc(1 * sizeof(int));

  *bytes_percorridos += sizeof(int) * fread(aux, sizeof(int), 1, arq_dados);
  salvar_nro_reg_arq(C, *aux);

  free(aux);
}

// Lê o campo 'nro_reg_rem' de um arquivo binário e salva em um registro de dados.
void ler_nro_reg_rem(FILE *arq_dados, reg_cabecalho *C, int *bytes_percorridos) {
  int *aux = (int *) malloc(1 * sizeof(int));

  *bytes_percorridos += sizeof(int) * fread(aux, sizeof(int), 1, arq_dados);
  salvar_nro_reg_rem(C, *aux);

  free(aux);
}

// Lê todos os campos de um registro de cabeçalho de um arquivo binário e salva em um registro. Retorna o nº de bytes percorridos durante a leitura.
int ler_reg_cabecalho(FILE *arq_dados, reg_cabecalho *C) {
  // Declarando variável auxiliar para leitura.
  char *caractere = (char *) malloc(1 * sizeof(char));

  // Lendo o campo status do registro de cabeçalho. 
  fread(caractere, sizeof(char), 1, arq_dados);
  int bytes_percorridos = 1; 

  // Salvando o status do arquivo.
  salvar_status(C, *caractere);

  // Lendo do arquivo binário e salvando os outros campos do registro de cabeçalho.
  ler_prox_byte_offset(arq_dados, C, &bytes_percorridos);
  ler_nro_reg_arq(arq_dados, C, &bytes_percorridos);
  ler_nro_reg_rem(arq_dados, C, &bytes_percorridos);
  
  // Liberando memória alocada.
  free(caractere);

  // Retornando a quantidade de bytes percorridos durante a leitura.
  return bytes_percorridos;
}

// Atualiza todos os campos de um registro de cabeçalho.
void atualizar_reg_cabecalho(reg_cabecalho* C, char status, long int offset, int reg_arq, int reg_rem){
  // Verificando se o registro de cabeçalho existe.
  if(C == NULL){
    printar_erro(4);
    return;
  }

  // Salvando os campos do registro de cabeçalho.
  salvar_status(C, status);
  salvar_prox_byte_offset(C, offset);
  salvar_nro_reg_arq(C, reg_arq);
  salvar_nro_reg_rem(C, reg_rem);
}