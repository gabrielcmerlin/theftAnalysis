// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funções relacionadas à manipulação de arquivos .csv e binários,
  como abertura, fechamento, leitura, escrita, impressão e (in)consistência. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivos.h"
#include "cabecalho.h"
#include "dados.h"
#include "indice.h"
#include "utils.h"

// Abre um arquivo. 
FILE* abrir_arquivo(char* nome_arq, char* modo){
  FILE* arquivo = fopen(nome_arq, modo);
  if(arquivo == NULL){
    printar_erro(1);
    exit(0);
  }

  return arquivo;
}

// Verifica a consistência de um arquivo. Se for inconsistente, fecha os arquivos abertos, imprime erro e encerra o programa.
void verificar_consistencia(FILE *arquivo, FILE *arquivo_aberto){
  char *status = (char *) malloc (1 * sizeof(char));
  fread(status, sizeof(char), 1, arquivo);
  if(*status == '1'){ // Arquivo consistente
    fseek(arquivo, 0, SEEK_SET);
    free(status);
  }else{ // Arquivo inconsistente
    free(status);
    fechar_arquivo(arquivo);
    if(arquivo_aberto != NULL) fechar_arquivo(arquivo_aberto);
    printar_erro(0);
    exit(0);
  }
}

// Marca um arquivo como inconsistente no registro de cabeçalho.
void marcar_como_inconsistente(FILE *arquivo){
  char *status = (char *) malloc(1 * sizeof(char));
  *status = '0';
  fseek(arquivo, 0, SEEK_SET);
  fwrite(status, sizeof(char), 1, arquivo);
  fseek(arquivo, 0, SEEK_SET);
  free(status);
}

// Fecha um arquivo.
void fechar_arquivo(FILE* arquivo){
  fclose(arquivo);
}

// Pula uma linha de um arquivo .csv.
void pular_linha_csv(FILE *arq_csv) {
  // Lendo, char a char, até o fim da linha.
  char c;
  do{
      c = fgetc(arq_csv);
  } while(c != '\n' && c != EOF);
}

// Lê, byte a byte, um campo do registro em um arquivo .csv. Retorna o campo em forma de string.
char *ler_campo(FILE *arq_csv) {
  char *buffer = NULL;
  char c;
  int n_carac_lidos = 0;

  // Lendo, byte a byte, até encontrar caracteres que demarcam fim de campo.
  c = fgetc(arq_csv);
  while(c != ',' && c != '\n' && c != EOF) {
    buffer = (char *) realloc(buffer, (n_carac_lidos + 1) * sizeof(char));
    buffer[n_carac_lidos] = c;

    n_carac_lidos++;

    c = fgetc(arq_csv);
  }

  // Se o campo não for nulo, finalizando a string com '\0'.
  if(buffer != NULL) {
    buffer = (char *) realloc(buffer, (n_carac_lidos + 1) * sizeof(char));
    buffer[n_carac_lidos] = '\0';
  }

  // Retornando o campo em forma de string.
  return buffer;
}

// Lê o campo 'ID' de um arquivo .csv e salva em um registro de dados.
void ler_id_csv(FILE *arq_csv, reg_dados **D) {
  // Verificando se o ID é nulo.
  // Se sim, o registro não existe (pela especificação do trabalho). 
  // Então, desalocando o registro de dados e finalizando a execução da função. 
  char *str_id = ler_campo(arq_csv);
  if(str_id == NULL) {
    free(*D);
    *D = NULL;
    return;
  }
  salvar_id(*D, atoi(str_id));
  free(str_id);
}

// Lê o campo 'dataCrime' de um arquivo .csv e salva em um registro de dados.
void ler_data_crime_csv(FILE *arq_csv, reg_dados *D) {
  char *data_crime = ler_campo(arq_csv);
  salvar_data_crime(D, data_crime);
}

// Lê o campo 'numeroArtigo' de um arquivo .csv e salva em um registro de dados.
void ler_numero_artigo_csv(FILE *arq_csv, reg_dados *D) {
  char *numero_artigo = ler_campo(arq_csv);
  if(numero_artigo == NULL) {
    numero_artigo = "-1";
  }
  salvar_numero_artigo(D, atoi(numero_artigo));
  if(numero_artigo != "-1") {
    free(numero_artigo);
  }
}

// Lê o campo 'lugarCrime' de um arquivo .csv e salva em um registro de dados.
void ler_lugar_crime_csv(FILE *arq_csv, reg_dados *D) {
  char *lugar_crime = ler_campo(arq_csv);
  salvar_lugar_crime(D, lugar_crime);
}

// Lê o campo 'descricaoCrime' de um arquivo .csv e salva em um registro de dados.
void ler_descricao_crime_csv(FILE *arq_csv, reg_dados *D) {
  char *descricao_crime = ler_campo(arq_csv);
  salvar_descricao_crime(D, descricao_crime);
}

// Lê o campo 'marcaCelular' de um arquivo .csv e salva em um registro de dados.
void ler_marca_celular_csv(FILE *arq_csv, reg_dados *D) {
  char *marca_celular = ler_campo(arq_csv);
  salvar_marca_celular(D, marca_celular);
}

// Lê, campo a campo, uma linha de um arquivo .csv.
void ler_linha_csv(FILE *arq_csv, reg_dados **D) {
  // Atribuindo campo 'removido'.
  salvar_removido(*D, '0');

  // Lendo do arquivo .csv e salvando informações no registro de dados.
  ler_id_csv(arq_csv, D);
  if(*D == NULL) return;
  ler_data_crime_csv(arq_csv, *D);
  ler_numero_artigo_csv(arq_csv, *D);
  ler_lugar_crime_csv(arq_csv, *D);
  ler_descricao_crime_csv(arq_csv, *D);
  ler_marca_celular_csv(arq_csv, *D);

  salvar_delimitador_reg(*D, '#');
}

// Imprime, campo a campo, todos os registros não nulos de um arquivo binário.
void imprimir_arq_dados(FILE *arq_dados, int total_bytes, int tam_cabecalho) {
  // Alocando registro de dados.
  reg_dados *dados = alocar_reg_dados();

  // Percorrendo todos os bytes do arquivo binário a partir do registro de cabeçalho.
  for(int bytes_percorridos = tam_cabecalho; bytes_percorridos < total_bytes;) {
    // Lendo um registro de dados.
    bytes_percorridos += ler_reg_dados(arq_dados, dados);

    // Imprimindo registros logicamente não removidos.
    if(retornar_removido(dados) == '0'){
      imprimir_reg_dados(dados);
    }

    // Limpando campos de tamanho variável do registro de dados auxiliar.
    limpar_campos_reg_dados(dados);
  }

  // Desalocando registro de dados.
  desalocar_reg_dados(&dados);
}

// Escreve todos os registros de dados de um arquivo .csv em um arquivo binário.
void escrever_arq_dados(FILE *arq_csv, FILE *arq_dados, int *nro_bytes_escritos, int *nro_reg_arq, int *nro_reg_rem) {
  // Declarando registro de dados auxiliar.
  reg_dados *dados_aux = alocar_reg_dados();

  // Lendo uma linha do arquivo de entrada e armazenando os dados no registro.
  ler_linha_csv(arq_csv, &dados_aux);

  // Lendo todos os registros do arquivo .csv
  while (dados_aux != NULL) {
    // Escrevendo registro de dados no arquivo binário e atualizando a quantidade de bytes escritos.
    *nro_bytes_escritos += escrever_reg_dados(arq_dados, &dados_aux);

    // Atualizando contadores.
    (*nro_reg_arq)++;
    if(retornar_removido(dados_aux) == '1') {
      (*nro_reg_rem)++;
    }

    // Liberando memória alocada.
    limpar_campos_reg_dados(dados_aux);

    // Lendo a próxima linha do arquivo de entrada e armazenando os dados no registro.
    ler_linha_csv(arq_csv, &dados_aux);
  }

  // Desalocando registro de dados.
  desalocar_reg_dados(&dados_aux);
}