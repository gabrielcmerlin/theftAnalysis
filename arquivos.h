// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das funções relacionadas à manipulação de arquivos .csv e binários,
  como abertura, fechamento, leitura, escrita, impressão e (in)consistência. 
*/

#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdbool.h>

#include "dados.h"

// Abre um arquivo.
FILE* abrir_arquivo(char* nome_arq, char* modo);

// Verifica a consistência de um arquivo. Se for inconsistente, fecha os arquivos abertos, imprime erro e encerra o programa.
void verificar_consistencia(FILE *arquivo, FILE *arquivo_aberto);

// Marca um arquivo como inconsistente no registro de cabeçalho.
void marcar_como_inconsistente(FILE *arquivo);

// Fecha um arquivo.
void fechar_arquivo(FILE* arquivo);

// Pula uma linha de um arquivo .csv.
void pular_linha_csv(FILE *arq_csv);

// Lê, campo a campo, uma linha de um arquivo .csv.
void ler_linha_csv(FILE *arq_csv, reg_dados **D);

// Imprime, campo a campo, todos os registros não nulos de um arquivo binário.
void imprimir_arq_dados(FILE *arq_dados, int total_bytes, int tam_cabecalho);

// Escreve todos os registros de dados de um arquivo .csv em um arquivo binário.
void escrever_arq_dados(FILE *arq_csv, FILE *arq_dados, int *nro_bytes_escritos, int *nro_reg_arq, int *nro_reg_rem);

#endif