// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração do tipo para registro de cabeçalho e suas funções relacionadas, como retornar e 
  salvar campos; alocar e desalocar registro; ler e escrever registro; imprimir registro.
*/

#ifndef CABECALHO_H
#define CABECALHO_H

#include <stdio.h>

// Define registro de cabeçalho.
typedef struct _reg_cabecalho reg_cabecalho;

// Aloca um registro de cabecalho.
reg_cabecalho *alocar_reg_cabecalho();

// Desaloca um registro de cabeçalho.
void desalocar_reg_cabecalho(reg_cabecalho **C);

// Atualiza o status de um registro de cabeçalho.
void salvar_status(reg_cabecalho *C, char status);

// Atualiza o próximo byte offset de um registro de cabeçalho.
void salvar_prox_byte_offset(reg_cabecalho *C, long int prox_byte_offset);

// Atualiza o número de registros em um arquivo de um registro de cabeçalho.
void salvar_nro_reg_arq(reg_cabecalho *C, int nro_reg_arq);

// Atualiza o número de registros logicamente removidos de um registro de cabeçalho.
void salvar_nro_reg_rem(reg_cabecalho *C, int nro_reg_rem);

// Retorna o status de um registro de cabeçalho.
char retornar_status(reg_cabecalho *C);

// Retorna o próximo byte offset de um registro de cabeçalho.
long int retornar_prox_byte_offset(reg_cabecalho *C);

// Retorna o número de registros em um arquivo de um registro de cabeçalho.
int retornar_nro_reg_arq(reg_cabecalho *C);

// Retorna o número de registros logicamente removidos de um registro de cabeçalho.
int retornar_nro_reg_rem(reg_cabecalho *C);

// Escreve um registro de cabeçalho em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_reg_cabecalho(FILE *arq_dados, reg_cabecalho *C);

// Lê todos os campos de um registro de cabeçalho de um arquivo binário e salva em um registro. Retorna o nº de bytes percorridos durante a leitura.
int ler_reg_cabecalho(FILE *arq_dados, reg_cabecalho *C);

// Atualiza todos os campos de um registro de cabeçalho.
void atualizar_reg_cabecalho(reg_cabecalho* C, char status, long int offset, int reg_arq, int reg_rem);

#endif