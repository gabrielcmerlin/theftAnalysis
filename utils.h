// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das funções auxiliares ao programa principal, que não pertencem ao escopo dos
  demais módulos, como funções fornecidas, impressão de erros e manipulação de strings em geral.
*/

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "dados.h"

#define TAM_STRING 50

// Função padrão do trabalho.
void binarioNaTela(char *nomeArquivoBinario);

// Função padrão do trabalho.
void scan_quote_string(char *str);

// Retorna a quantidade de dígitos, em decimal, de um número n.
int qntd_digitos(int n);

// Inverte a posição dos caracteres de uma string.
void inverter(char *string);

// Transforma um valor inteiro em uma string.
void int_para_string(int n, char **string);

// Trunca uma string para 12 caracteres. Caso a string seja menor, preenche o restante com '$'.
char *truncar_string_doze(char *str);

// Preenche uma string com '$'.
void colocar_cifrao(char *string, int tam_max);

// Tira o primeiro caractere '$' de uma string.
void tirar_cifrao(char *string);

// Imprime mensagem de erro específica para cada falha.
void printar_erro(int flag);

// Verifica se um arquivo está vazio. Se sim, imprime erro e retorna true. Caso contrário, retorna false.
bool verificar_arq_vazio(int total_bytes, int tam_cabecalho);

// Verifica se um arquivo está inconsistente. Se sim, imprime erro e retorna true. Caso contrário, retorna false.
bool verificar_arq_inconsistente(char status);

// Conta a quantidade de '$' entre o delimitador do último campo variável ('|') e o delimitador de registro ('#').
int contar_lixo(FILE *arq_dados, reg_dados *D);

#endif