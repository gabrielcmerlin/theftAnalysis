// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração do tipo para registro de dados e suas funções relacionadas, como retornar e 
  salvar campos; alocar e desalocar registro; ler e escrever registro; imprimir registro.
*/

#ifndef DADOS_H
#define DADOS_H
#include <stdio.h>

// Structs para registros de dados.
typedef struct _reg_dados reg_dados;

// Aloca um registro de dados.
reg_dados *alocar_reg_dados();

// Desaloca um registro de dados.
void desalocar_reg_dados(reg_dados **D);

// Atualiza o status de um registro de dados.
void salvar_removido(reg_dados *D, char removido);

// Atualiza o ID de um registro de dados.
void salvar_id(reg_dados *D, int id);

// Atualiza a data do crime de um registro de dados.
void salvar_data_crime(reg_dados *D, char *data_crime);

// Atualiza o número do artigo de um registro de dados.
void salvar_numero_artigo(reg_dados *D, int numero_artigo);

// Atualiza a marca de celular de um registro de dados.
void salvar_marca_celular(reg_dados *D, char *marca_celular);

// Atualiza o lugar do crime de um registro de dados.
void salvar_lugar_crime(reg_dados *D, char *lugar_crime);

// Atualiza descrição do crime de um registro de dados.
void salvar_descricao_crime(reg_dados *D, char *descricao_crime);

// Atualiza delimitador de registro de um registro de dados.
void salvar_delimitador_reg(reg_dados *D, char delimitador);

// Retorna o status de um registro de dados.
char retornar_removido(reg_dados *D);

// Retorna o ID de um registro de dados.
int retornar_id(reg_dados *D);

// Retorna a data do crime de um registro de dados.
char *retornar_data_crime(reg_dados *D);

// Retorna o número do artigo de um registro de dados.
int retornar_numero_artigo(reg_dados *D);

// Retorna a marca de celular de um registro de dados.
char *retornar_marca_celular(reg_dados *D);

// Retorna o lugar do crime de um registro de dados.
char *retornar_lugar_crime(reg_dados *D);

// Retorna a descrição do crime de um registro de dados.
char *retornar_descricao_crime(reg_dados *D);

// Retorna o delimitador de um registro de dados.
char retornar_delimitador(reg_dados *D);

// Limpa os campos de tamanho variável de um registro de dados.
void limpar_campos_reg_dados(reg_dados *D);

// Escreve um registro de dados em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_reg_dados(FILE *arq_dados, reg_dados **D);

// Lê todos os campos de um registro de dados e salva em um registro. Retorna o nº de bytes percorridos durante a leitura.
int ler_reg_dados(FILE *arq_dados, reg_dados *D);

// Imprime todos os campos de um registro de dados.
void imprimir_reg_dados(reg_dados *D);

// Copia os campos de um registro de dados fonte para um registro de dados destino.
void copiar_reg_dados(reg_dados *D_dest, reg_dados *D_fonte);

// Retorna um campo do tipo 'int' de um registro de dados de acordo com o nome do campo.
int retornar_campo_int(reg_dados *D, char *campo_pedido);

// Retorna um campo do tipo 'string' de um registro de dados de acordo com o nome do campo.
char *retornar_campo_string(reg_dados *D, char *campo_pedido);

// Compara um valor dado com o valor de um campo do registro de dados.
bool comparar_valor_reg_dados(reg_dados *D, char *nome_campo, char *valor_comparador);

// Lê um registro de dados para a inserção.
int ler_para_inserir(reg_dados *D);

// Marca logicamente um registro de dados como removido no arquivo de dados.
void marcar_como_removido(FILE *arq_dados, reg_dados *D, long int byte_offset);

#endif