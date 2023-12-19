// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação do tipo para registros de cabeçalho e de dados de índice e suas funções 
  relacionadas, como retornar e salvar campos; alocar e desalocar registro; ler e escrever registro; criar vetor.
  Além disso, funções relacionadas ao vetor de índices, como criar, preencher, carregar, 'shiftar' e desalocar.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "indice.h"
#include "arquivos.h"
#include "busca.h"
#include "ordenacao.h"
#include "utils.h"

// Define registro de cabeçalho de índice.
struct _indice_cabecalho {
    char status;
    int qntd_reg;
} ;

// Define registro de dados do tipo 'int' de índice.
struct _indice_dados_int {
    int chave_busca;
    long int byte_offset;
} ;

// Define registro de dados do tipo 'string' de índice.
struct _indice_dados_string {
    char *chave_busca;
    long int byte_offset;
} ;

// Aloca um registro de cabeçalho de índice.
indice_cabecalho *alocar_indice_cabecalho() {
  indice_cabecalho *C = (indice_cabecalho *) calloc(1, sizeof(indice_cabecalho));

  return C;
}

// Desaloca um registro de cabeçalho de índice.
indice_cabecalho *desalocar_indice_cabecalho(indice_cabecalho **Ci) {
  if(Ci != NULL && *Ci != NULL) {
    free(*Ci);
    *Ci = NULL;
  }
}

// Aloca um registro de dados do tipo 'int' de índice.
indice_dados_int *alocar_indice_dados_int() {
  indice_dados_int *C = (indice_dados_int *) calloc(1, sizeof(indice_dados_int));

  return C;
}

// Desaloca um registro de dados do tipo 'int' de índice.
indice_dados_int *desalocar_indice_dados_int(indice_dados_int **Di) {
  if(Di != NULL && *Di != NULL) {
    free(*Di);
    *Di = NULL;
  }
}

// Aloca um registro de dados do tipo 'string' de índice.
indice_dados_string *alocar_indice_dados_string() {
  indice_dados_string *C = (indice_dados_string *) calloc(1, sizeof(indice_dados_string));

  return C;
}

// Desaloca um registro de dados do tipo 'string' de índice.
indice_dados_string *desalocar_indice_dados_string(indice_dados_string **Di) {
  if(Di != NULL && *Di != NULL) {
    free(*Di);
    *Di = NULL;
  }
}

// Limpa campo 'chave_busca' de registro de dados do tipo 'string' de índice.
void limpar_campos_indice_dados_str(indice_dados_string *Di) {
    free(Di->chave_busca);
}

// Atualiza o status de um registro de cabeçalho de índice.
void salvar_status_indice(indice_cabecalho *Ci, char status){
    Ci->status = status;
}

// Atualiza a quantidade de registros no arquivo de um registro de cabeçalho de índice.
void salvar_qntd_reg_indice(indice_cabecalho *Ci, int qntd_reg){
    Ci->qntd_reg = qntd_reg;
}

// Retorna a quantidade de registros do cabeçalho de índice.
int retornar_qntd_reg_indice(indice_cabecalho *C_indice) {
    return C_indice->qntd_reg;
}

// Retorna o status do cabeçalho de índice.
char retornar_status_indice(indice_cabecalho *C_indice){
  return C_indice->status;
}

// Atualiza a chave de busca (do tipo 'int') de um registro de dados de índice.
void salvar_chave_busca_int(indice_dados_int *Di, int chave_busca){
    Di->chave_busca = chave_busca;
}

// Atualiza o campo 'byte_offset' de um registro de dados do tipo 'int' de índice.
void salvar_byte_offset_int(indice_dados_int *Di, long int byte_offset){
    Di->byte_offset = byte_offset;
}

// Atualiza a chave de busca (do tipo 'string') de um registro de dados de índice.
void salvar_chave_busca_string(indice_dados_string *Di, char* chave_busca){
    Di->chave_busca = chave_busca;
}

// Atualiza o campo 'byte_offset' de um registro de dados do tipo 'string' de índice.
void salvar_byte_offset_string(indice_dados_string *Di, long int byte_offset){
    Di->byte_offset = byte_offset;
}

// Retorna a chave de busca (do tipo 'int') de um registro de dados de índice.
int retornar_chave_busca_int(indice_dados_int *Di) {
    return Di->chave_busca;
}

// Retorna o campo 'byte_offset' de um registro de dados do tipo 'int' de índice.
long int retornar_byte_offset_int(indice_dados_int *Di) {
    return Di->byte_offset;
}

// Retorna a chave de busca (do tipo 'string') de um registro de dados de índice.
char *retornar_chave_busca_string(indice_dados_string *Di) {
    return Di->chave_busca;
}

// Retorna o campo 'byte_offset' de um registro de dados do tipo 'string' de índice.
long int retornar_byte_offset_string(indice_dados_string *Di) {
    return Di->byte_offset;
}

// Retorna a chave busca de um registro de dados de índice.
void *retornar_chave_busca(void *Di, char *nome_campo) {
  void *retorno;

  // Retornando chave de busca do tipo 'int'
  if (strcmp(nome_campo, "idCrime") == 0 || strcmp(nome_campo, "numeroArtigo") == 0) { //é inteiro
    int *chave_busca = (int *) malloc (1 * sizeof(int));
    *chave_busca = retornar_chave_busca_int((indice_dados_int *) Di);
    retorno = (void *) chave_busca;
  } else { // do tipo 'string'
    char *chave_busca = retornar_chave_busca_string((indice_dados_string *) Di);
    retorno = (void *) chave_busca;
  }
  
  // Retonrando chave de busca como tipo 'void'
  return retorno;
}

// Salva o byte offset de um registro de dados de índice.
void salvar_byte_offset(void *Di, long int byte_offset, char *tipo_dado) {
    if (strcmp(tipo_dado, "inteiro") == 0) { //é inteiro
        salvar_byte_offset_int(Di, byte_offset);
    } else {
        salvar_byte_offset_string(Di, byte_offset);
    }
}

// Salva o byte offset de um registro de dados de índice.
long int retornar_byte_offset(void *Di, char *nome_campo) {
  long int byte_offset;

  if (strcmp(nome_campo, "idCrime") == 0 || strcmp(nome_campo, "numeroArtigo") == 0) { //é inteiro
    byte_offset = retornar_byte_offset_int((indice_dados_int *) Di);
  } else {
    byte_offset = retornar_byte_offset_string((indice_dados_string *) Di);
  }
  
  return byte_offset;
}

// Compara uma chave de busca com um valor dado. Retorna true se são iguais. Caso contrário, retorna false.
bool comparar_chave_busca(void *chave_busca, char *valor_comparador, char *nome_campo) {
  if(strcmp(nome_campo, "idCrime") == 0 || strcmp(nome_campo, "numeroArtigo") == 0) { // é inteiro
    if ( (*(int *) chave_busca) == atoi(valor_comparador))
      return true;
  } else {
    if (strcmp((char *) chave_busca, valor_comparador) == 0)
      return true;
  }

  return false;
}

// Limpa o campo 'chave_busca' do tipo 'string' de um registor de dados de índice.
void limpar_chave_busca_string(indice_dados_string *D){
    free(D->chave_busca);
}

// Atualiza e escreve o status de um arquivo de índice.
void atualizar_status_indice(FILE *arq_indice, indice_cabecalho *C_indice, char status){
    fseek(arq_indice, SEEK_SET, 0);
    salvar_status_indice(C_indice, status);
    fwrite(&(status), sizeof(char), 1, arq_indice);
}

// Cria um arquivo de indíce, escrevendo o cabeçalho e os registros de dados de índice.
void criar_arquivo_indice(void **vetor, int qntd_reg_indice, char* nome_indice, char *tipo_dado){
    // Abrindo arquivo de índice.
    FILE *arq_indice = abrir_arquivo(nome_indice, "wb");

    // Alocando registro de cabeçalho de índice auxiliar.
    indice_cabecalho *C_indice = alocar_indice_cabecalho();

    // Preenchendo campos do registro de cabeçalho de índice.
    salvar_status_indice(C_indice, '0');
    salvar_qntd_reg_indice(C_indice, qntd_reg_indice);

    // Escrevendo o cabeçalho e os registros de dados de índice no arquivo de índice,
    escrever_indice_cabecalho(arq_indice, C_indice);
    escrever_indice_dados(arq_indice, vetor, qntd_reg_indice, tipo_dado);

    // Atualizando status do arquivo de índice.
    atualizar_status_indice(arq_indice, C_indice, '1');

    // Desalocando registro de cabeçalho de índice auxiliar.
    desalocar_indice_cabecalho(&C_indice);
   
    // Fechando arquivo.
    fechar_arquivo(arq_indice);
}

// Retorna um vetor de registros de dados de índice (do tipo 'string') preenchido com um registro, dados os campos.
indice_dados_string **preencher_vetor_string(void **vetor, int *qntd_reg_indice, char* chave_busca, int bytes_percorridos){

  // Adequando o tipo do vetor.
  indice_dados_string **vetor_string = (indice_dados_string **) vetor;

  // Realocando memória para o vetor receber mais um registro.
  vetor_string = (indice_dados_string **) realloc(vetor_string, ((*qntd_reg_indice) + 1) * sizeof(indice_dados_string *));
  
  // Alocando registro de dados do índice auxiliar.
  indice_dados_string *D_indice = (indice_dados_string *) malloc(1 * sizeof(indice_dados_string));
  
  // Armazenando chave de busca.
  char* chave_aux = (char *) malloc((strlen(chave_busca) + 1) * sizeof(char));
  strcpy(chave_aux, chave_busca);

  // Preenchendo os campos do registro de dados do índice.
  salvar_chave_busca_string(D_indice, chave_aux);
  salvar_byte_offset_string(D_indice, bytes_percorridos);

  // Colocando registro de dados do índice lido no vetor.
  vetor_string[(*qntd_reg_indice)] = D_indice;

  // Incrementando contador de registros no arquivo de índice.
  (*qntd_reg_indice)++;

  // Retornando vetor preenchido.
  return vetor_string;
}

// Retorna um vetor de registros de dados de índice (do tipo 'int') preenchido com um registro, dados os campos.
indice_dados_int **preencher_vetor_int(void **vetor, int *qntd_reg_indice, int chave_busca, int bytes_percorridos){

  // Adequando o tipo do vetor.
  indice_dados_int **vetor_int = (indice_dados_int **)vetor;

  // Realocando memória para o vetor receber mais um registro.
  vetor_int = (indice_dados_int **) realloc(vetor_int, ((*qntd_reg_indice) + 1) * sizeof(indice_dados_int *));

  // Alocando registro de dados do índice auxiliar.
  indice_dados_int *D_indice = (indice_dados_int *) malloc(1 * sizeof(indice_dados_int));

  // Preenchendo os campos do registro de dados do índice.
  salvar_byte_offset_int(D_indice, bytes_percorridos);
  salvar_chave_busca_int(D_indice, chave_busca);

  // Colocando registro de dados do índice lido no vetor.
  vetor_int[(*qntd_reg_indice)] = D_indice;

  // Incrementando contador de registros no arquivo de índice.
  (*qntd_reg_indice)++;

  // Retornando o vetor preenchido.
  return vetor_int;
}

// Desaloca um vetor de registros de dados de indíce.
void desalocar_vetor_indice(void **vetor, int qntd_reg_indice, char *tipo_dado){

  // Verificando se o vetor é não nulo.
  if(vetor == NULL) return;

  // Desalocando vetor de registros de dados de índice de acordo com o tipo de dados.
  if(strcmp(tipo_dado, "string") == 0){
      for(int i = 0; i < qntd_reg_indice; i++) {
          indice_dados_string** vetor_aux = (indice_dados_string **) vetor;
          limpar_chave_busca_string(vetor_aux[i]);
          desalocar_indice_dados_string(&vetor_aux[i]);
      }
  }else{
      indice_dados_int** vetor_aux = (indice_dados_int **) vetor;
      for(int i = 0; i < qntd_reg_indice; i++) {
          desalocar_indice_dados_int(&vetor_aux[i]);
      }
  }
  free(vetor);
}

// Escreve o campo 'status' de um registro de cabeçalho de índice.
void escrever_status_indice(FILE *arq_indice, indice_cabecalho *Ci, int *bytes_escritos){
    *bytes_escritos += fwrite(&(Ci->status), sizeof(char), 1, arq_indice);
}

// Escreve o campo 'qntd_reg' de um registro de cabeçalho de índice.
void escrever_qntd_reg_indice(FILE *arq_indice, indice_cabecalho *Ci, int *bytes_escritos){
    bytes_escritos += fwrite(&(Ci->qntd_reg), sizeof(int), 1, arq_indice);
}

// Escreve um registro de cabeçalho de índice em um arquivo de índice.
int escrever_indice_cabecalho(FILE *arq_indice, indice_cabecalho *Ci) {
    int bytes_escritos = 0;

    escrever_status_indice(arq_indice, Ci, &bytes_escritos);
    escrever_qntd_reg_indice(arq_indice, Ci, &bytes_escritos);

    return bytes_escritos;
}

// Escreve o campo 'chave_busca' do tipo 'int' de um registro de dados de índice.
void escrever_chave_busca_dados_int(FILE *arq_indice, indice_dados_int *Di, int *bytes_escritos){
    *bytes_escritos += fwrite(&(Di->chave_busca), sizeof(int), 1, arq_indice);
}

// Escreve o campo 'byteOffset' de um registro de dados de índice do tipo 'int'.
void escrever_byte_offset_dados_int(FILE *arq_indice, indice_dados_int *Di, int *bytes_escritos){
    *bytes_escritos += fwrite(&(Di->byte_offset), sizeof(long int), 1, arq_indice);
}

// Escreve um registro de dados de índice do tipo 'int' em um arquivo de índice.
int escrever_indice_dados_int(FILE *arq_indice, indice_dados_int *Di) {
    int bytes_escritos = 0;

    escrever_chave_busca_dados_int(arq_indice, Di, &bytes_escritos);
    escrever_byte_offset_dados_int(arq_indice, Di, &bytes_escritos);

    return bytes_escritos;
}

// Escreve o campo 'chave_busca' do tipo 'string' de um registro de dados de índice.
void escrever_chave_busca_dados_string(FILE *arq_indice, indice_dados_string *Di, int *bytes_escritos, char *str){
    *bytes_escritos += fwrite(str, sizeof(char), 12, arq_indice);
}

// Escreve o campo 'byteOffset' de um registro de dados de índice do tipo 'string'.
void escrever_byte_offset_dados_string(FILE *arq_indice, indice_dados_string *Di, int *bytes_escritos){
    *bytes_escritos += fwrite(&(Di->byte_offset), sizeof(long int), 1, arq_indice);
}

// Escreve um registro de dados de índice do tipo 'string' em um arquivo de índice.
int escrever_indice_dados_string(FILE *arq_indice, indice_dados_string *Di) {
    int bytes_escritos = 0;

    char *str = truncar_string_doze(retornar_chave_busca_string(Di));

    escrever_chave_busca_dados_string(arq_indice, Di, &bytes_escritos, str);
    escrever_byte_offset_dados_string(arq_indice, Di, &bytes_escritos);

    free(str);

    return bytes_escritos;
}

// Escreve um registro de dados de índice em um arquivo de índice.
void escrever_indice_dados(FILE *arq_indice, void **vetor, int qntd_reg_indice, char *tipo_dado){
    
  // Verificando se o vetor é nulo.
  if(vetor == NULL) return;

  // Escrevendo registros de dados de índice no arquivo de índice de acordo com o tipo de dado.
  if(strcmp(tipo_dado, "string") == 0){
      indice_dados_string **vetor_aux = (indice_dados_string **)vetor;
      for (int i = 0; i < qntd_reg_indice; i++) {
          escrever_indice_dados_string(arq_indice, vetor_aux[i]);
      }
  }else{
      indice_dados_int **vetor_aux = (indice_dados_int **)vetor;
      for(int i = 0; i < qntd_reg_indice; i++) {
          escrever_indice_dados_int(arq_indice, vetor_aux[i]);
      }
  }
}

// Lê o registro de cabeçalho de um arquivo de índice.
void ler_cabecalho_indice(FILE *arq_indice, indice_cabecalho *C_indice){
  char *status = (char *) malloc(1 * sizeof(char));;
  fread(status, sizeof(char), 1, arq_indice);
  salvar_status_indice(C_indice, *status);
  free(status);

  int *qntd_reg_indice = (int *) malloc(1 * sizeof(int));
  fread(qntd_reg_indice, sizeof(int), 1, arq_indice);
  salvar_qntd_reg_indice(C_indice, *qntd_reg_indice);
  free(qntd_reg_indice);
}

// Carrega o arquivo de índice para a memória principal.
void **carregar_arq_indice(FILE *arq_indice, int *tam_vetor_indice, char *tipo_dado){
    // Declarando vetor para retorno.
    void **vetor_indice_retorno;

    // Alocando registro de cabeçalho de índice auxiliar.
    indice_cabecalho *C_indice = alocar_indice_cabecalho();

    // Lendo o cabeçalho do arquivo de índice
    ler_cabecalho_indice(arq_indice, C_indice);

    // Armazenando a quantidade de registros do arquivo de índice.  
    int qntd_reg = retornar_qntd_reg_indice(C_indice);
    *tam_vetor_indice = qntd_reg;

    // Desalocando registro auxiliar.
    desalocar_indice_cabecalho(&C_indice);

    if (strcmp(tipo_dado, "inteiro") == 0) {
        indice_dados_int **vetor_indice = (indice_dados_int **) malloc(qntd_reg * (sizeof(indice_dados_int *)));

        for (int i = 0; i < qntd_reg; i++) {
            // Alocando e inicializando registro de dados de índice.
            indice_dados_int *D_indice = alocar_indice_dados_int();
            int *chave_busca_int = (int *) malloc(1 * sizeof(int));
            fread(chave_busca_int, sizeof(int), 1, arq_indice);
            salvar_chave_busca_int(D_indice, *chave_busca_int);
            free(chave_busca_int);

            long int *byte_offset = (long int *) malloc(1 * sizeof(long int));
            fread(byte_offset, sizeof(long int), 1, arq_indice);
            salvar_byte_offset_int(D_indice, *byte_offset);
            free(byte_offset);

            vetor_indice[i] = D_indice;
        }

        vetor_indice_retorno = (void **) vetor_indice;
    } else { // tipo_dado == string
        indice_dados_string **vetor_indice = (indice_dados_string **) malloc(qntd_reg * (sizeof(indice_dados_string *)));

        // Modularizar isso aq numa funcao de nome +- "passando_arq_indice_para _mem_principal" e colocar ela antes de chamar a busca binaria em si
        // pq assim deixa a funcao busca binaria mais certa semanticamente
        for (int i = 0; i < qntd_reg; i++) {
            // alocando e inicializando o D_indice
            indice_dados_string *D_indice = alocar_indice_dados_string();
            char *chave_busca_str = (char *) malloc(13 * sizeof(char));
            fread(chave_busca_str, sizeof(char), 12, arq_indice);
            chave_busca_str[12] = '\0';
            tirar_cifrao(chave_busca_str);
            salvar_chave_busca_string(D_indice, chave_busca_str);

            long int *byte_offset = (long int *) malloc(1 * sizeof(long int));
            fread(byte_offset, sizeof(long int), 1, arq_indice);
            salvar_byte_offset_string(D_indice, *byte_offset);
            free(byte_offset);

            vetor_indice[i] = D_indice;
        }

        vetor_indice_retorno = (void **) vetor_indice;
    }

    return vetor_indice_retorno;
}

// Realoca memória do vetor de índice por meio da verificação dos registros logicamente removidos.
// Coloca os registros removidos no fim do vetor, reordena os registros válidos e libera a memória dos inválidos.
void **shift_vetor_indice(void **vetor_indice, int tam_vetor_indice, int qntd_reg_rem, char *tipo_dado){
  if (strcmp(tipo_dado, "inteiro") == 0) {
    indice_dados_int **vetor_aux = (indice_dados_int **) vetor_indice;
    indice_dados_int *aux;

    // Encontrando registro removido logicamente e colocando-o mais próximo possível do fim do vetor.
    for (int i = 0; i < (tam_vetor_indice - qntd_reg_rem); i++) {
      if (retornar_byte_offset_int(vetor_aux[i]) < 0) {
        int j = i + 1;
        while (retornar_byte_offset_int(vetor_aux[j]) < 0) {
          j++;
        }

        // Trocando posição dos elementos do vetor.
        swap(vetor_indice, i, j, tipo_dado);
      }
    }

    // Desalocando memória dos registros removidos.
    for (int i = (tam_vetor_indice - qntd_reg_rem); i < tam_vetor_indice; i++) {
      aux = vetor_aux[i];
      desalocar_indice_dados_int(&aux);
    }

    // Realocando memória do vetor com apenas registros não removidos.
    vetor_indice = (void *) realloc(vetor_indice, (tam_vetor_indice - qntd_reg_rem) * sizeof(indice_dados_int *));
  } else {
    indice_dados_string **vetor_aux = (indice_dados_string **) vetor_indice;
    indice_dados_string *aux;

    // Encontrando registro removido logicamente e colocando-o mais próximo possível do fim do vetor.
    for (int i = 0; i < (tam_vetor_indice - qntd_reg_rem); i++) {
      if (retornar_byte_offset_string(vetor_aux[i]) < 0) {
        int j = i + 1;
        while (retornar_byte_offset_string(vetor_aux[j]) < 0) {
          j++;
        }

        // Trocando posição dos elementos do vetor.
        swap(vetor_indice, i, j, tipo_dado);
      }
    }

    // Desalocando memória dos registros removidos.
    for (int i = (tam_vetor_indice - qntd_reg_rem); i < tam_vetor_indice; i++) {
      aux = vetor_aux[i];
      free(retornar_chave_busca_string(aux));
      desalocar_indice_dados_string(&aux);
    }

    // Realocando memória do vetor com apenas registros não removidos.
    vetor_indice = (void *) realloc(vetor_indice, (tam_vetor_indice - qntd_reg_rem) * sizeof(indice_dados_string *));
  }

  // Retornando vetor de índices apenas com registros não removidos.
  return vetor_indice;
}

// Insere um registro de dados de índice do tipo 'int' em um vetor de índices, verificando sua chave.
void inserir_vetor_indice_int(indice_dados_int **vetor, int chave_busca, int *qntd_reg, indice_dados_int *D_indice){
  // Inserindo registro lido no vetor de índice caso a chave de busca seja válida.
  // Para chaves inteiras, basta verificar se é não nula.
  if (chave_busca != -1) {
    vetor[(*qntd_reg)++] = D_indice;
  } else {
    desalocar_indice_dados_int(&D_indice);
  }
}

// Insere um registro de dados de índice do tipo 'string' em um vetor de índices, verificando sua chave.
void inserir_vetor_indice_string(indice_dados_string **vetor, char *chave_busca, int *qntd_reg, indice_dados_string *D_indice){
  // Inserindo registro lido no vetor de índice caso a chave de busca seja válida.
  // Para chaves do tipo string, basta verificar se é não vazia.
  if (strcmp(chave_busca, "") != 0) {
    vetor[(*qntd_reg)++] = D_indice;
  } else {
    desalocar_indice_dados_string(&D_indice);
  }
}

// Encontra o RRN de um registro de dados no vetor de índice.
void encontrar_rrn(FILE *arq_dados, reg_dados *D, long int byte_offset, void **vetor_indice, int *rrn_indice, 
                   int tam_vetor_indice, char *campo_indexado, char *tipo_dado){
  
  // Encontrando RRN após uma busca sequencial. 
  if ((*rrn_indice) == -1) {
 
    // Encontrando valor buscado.
    char *valor;
    if (strcmp(campo_indexado, "idCrime") == 0 || strcmp(campo_indexado, "numeroArtigo") == 0) {

      int valor_int = retornar_campo_int(D, campo_indexado);
      valor = (char *) malloc((qntd_digitos(valor_int) + 1) * sizeof(char));
      int_para_string(valor_int, &valor);
    } else {
      valor = retornar_campo_string(D, campo_indexado);
    }
 
    // Encontrando RRN.
    (*rrn_indice) = busca_binaria(arq_dados, vetor_indice, 0, tam_vetor_indice - 1, valor, tipo_dado);

    // Verificando se o RRN encontrado é válido.
    if ((*rrn_indice) >= 0) {
      void *chave_busca = retornar_chave_busca(vetor_indice[(*rrn_indice)], campo_indexado);

      // Encontrando o primeiro registro do vetor com o valor buscado.
      while (comparar_chave_busca(chave_busca, valor, campo_indexado) && (*rrn_indice) > 0) {
        (*rrn_indice)--;
        if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);
        chave_busca = retornar_chave_busca(vetor_indice[(*rrn_indice)], campo_indexado);
      }
  
      // Encontramos registro com a chave diferente da buscada.
      // Então, adequando o contador de RRN.
      if (comparar_chave_busca(chave_busca, valor, campo_indexado) == false) (*rrn_indice)++;

      // Liberando memória alocada.
      if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);

      // Encontrando registro com o byte offset buscado.
      while (retornar_byte_offset_int(vetor_indice[(*rrn_indice)]) != byte_offset) {
        (*rrn_indice)++;
      }
    }

    // Liberando memória alocada.
    if (strcmp(tipo_dado, "inteiro") == 0) free(valor);
  }

}

// Remove, logicamente, o registro do vetor de índice marcando o byte offset como negativo (para posterior realocação de memória).
// Dessa forma, é possível evitar repetição de realocação de memória, realizando apenas uma realocação ao final das remoções lógicas.
void remover_registro_vetor(void **vetor_indice, int rrn_indice, char *campo_indexado, char *tipo_dado){
  long int byte_offset_anterior = retornar_byte_offset(vetor_indice[rrn_indice], campo_indexado);
  salvar_byte_offset(vetor_indice[rrn_indice], -1 * abs(byte_offset_anterior), tipo_dado);
}

// Realiza a deleção de um registro de dados do arquivo de dados e do vetor de índice.
void realizar_delecao(FILE *arq_dados, reg_dados *D, long int byte_offset, void **vetor_indice, int rrn_indice, 
                      int tam_vetor_indice, char *campo_indexado, char *tipo_dado){
  // Marcando registro como removido. 
  marcar_como_removido(arq_dados, D, byte_offset);

  // Encontrando RRN do registro no arquivo de índice. 
  encontrar_rrn(arq_dados, D, byte_offset, vetor_indice, &rrn_indice, tam_vetor_indice, campo_indexado, tipo_dado);

  // Removendo registro do arquivo de índices, caso ele esteja indexado.
  if (rrn_indice != -1) {
    remover_registro_vetor(vetor_indice, rrn_indice, campo_indexado, tipo_dado);
  }
}

// Insere no vetor os índices correspondentes aos registros de dados de um arquivo de dados.
void **realizar_insercao(FILE *arq_dados, void **vetor, int *qntd_reg, long int *prox_byte_offset, int qntd_insercoes, 
                         char *tipo_dado, char* campo_indexado){
  // Alocando registro de dados auxiliar.
  reg_dados *D = alocar_reg_dados();

  // Declarando vetor de índices a ser preenhcido.
  void **vetor_indice = NULL;

  // Declarando variável auxiliar para escrita.
  long int bytes_escritos; 

  if (strcmp(tipo_dado, "inteiro") == 0) { // tipo_dado == inteiro
    
    // Adequando tipo do vetor e realocando memória.
    indice_dados_int **vetor_indice_aux = (indice_dados_int **) vetor;
    vetor_indice_aux = (indice_dados_int **) realloc(vetor_indice_aux, ((*qntd_reg) + qntd_insercoes) * sizeof(indice_dados_int *));

    // Inserindo cada registro no vetor.
    for (int i = 0; i < qntd_insercoes; i++) {
      // Lendo registro a ser inserido.
      ler_para_inserir(D);

      // Escrevendo registro de dados lido no arquivo de dados.
      bytes_escritos = escrever_reg_dados(arq_dados, &D);

      // Inicializando o registro de dados do índice.
      indice_dados_int *D_indice = alocar_indice_dados_int();
      int chave_busca_int = retornar_campo_int(D, campo_indexado);
      salvar_chave_busca_int(D_indice, chave_busca_int);
      salvar_byte_offset_int(D_indice, *prox_byte_offset);
      
      // Inserindo registro lido no vetor de índice caso a chave de busca seja válida.
      inserir_vetor_indice_int(vetor_indice_aux, chave_busca_int, &(*qntd_reg), D_indice);

      // Atualizando o próximo byte offset do arquivo de dados após a inserção.
      *prox_byte_offset += bytes_escritos;

      // Limpando campos de tamanho variável do registro auxiliar.
      limpar_campos_reg_dados(D);
    }

    // Adequando o tipo do vetor.
    vetor_indice = (void **)vetor_indice_aux;
  } 
  else { // tipo_dado == string
    // Alocando registro de dados auxiliar.
    indice_dados_string **vetor_indice_aux = (indice_dados_string **) vetor;
    vetor_indice_aux = (indice_dados_string **) realloc(vetor_indice_aux, ((*qntd_reg) + qntd_insercoes) * sizeof(indice_dados_string *));

    // Inserindo cada registro no vetor.
    for (int i = 0; i < qntd_insercoes; i++) {
      // Lendo registro a ser inserido.
      ler_para_inserir(D);

      // Escrevendo registro de dados lido no arquivo de dados.
      bytes_escritos += escrever_reg_dados(arq_dados, &D);

      // Inicializando o registro de dados do índice.
      indice_dados_string *D_indice = alocar_indice_dados_string();
      char *chave_busca_string = retornar_campo_string(D, campo_indexado);
      salvar_chave_busca_string(D_indice, chave_busca_string);
      salvar_byte_offset_string(D_indice, *prox_byte_offset);
      
      // Inserindo registro lido no vetor de índice caso a chave de busca seja válida.
      inserir_vetor_indice_string(vetor_indice_aux, chave_busca_string, &(*qntd_reg), D_indice);

      // Atualizando o próximo byte offset do arquivo de dados após a inserção.
      *prox_byte_offset += bytes_escritos;

      // Limpando campos de tamanho variável do registro auxiliar.
      limpar_campos_reg_dados(D);
    }

    // Adequando o tipo do vetor.
    vetor_indice = (void **)vetor_indice_aux;
  }

  // Desalocando registro de dados auxiliar.  
  desalocar_reg_dados(&D);

  // Retornando vetor de índices.
  return vetor_indice;
}

// Atualiza os campos do registro e retorna a diferença de tamanho do registro antes e depois da atualização.
int atualizar_registro(FILE *arq_dados, reg_dados *D, reg_dados *D_novo, criterio* vetor_atualizacoes, int qntd_campos_atualizados){
  // Declarando variáveis auxiliares.
  int tam_campos_variaveis_velho = contar_lixo(arq_dados, D);
  int tam_campos_variaveis_novo = 0;
  char *lugar_crime_velho = retornar_lugar_crime(D);
  char *descricao_crime_velho = retornar_descricao_crime(D);

  // Atualizando os campos do registros e contando a diferença de bytes entre antes e após a atualização.
  for (int i = 0; i < qntd_campos_atualizados; i++) {
    char *nome_campo = vetor_atualizacoes[i].nome_campo;
    char *valor_buscado = vetor_atualizacoes[i].valor_buscado;

    char *valor_buscado_aux = (char *) malloc((strlen(valor_buscado) + 1) * sizeof(char));
    strcpy(valor_buscado_aux, valor_buscado);

    if (strcmp(nome_campo, "idCrime") == 0) {
      salvar_id(D_novo, atoi(valor_buscado_aux));
      free(valor_buscado_aux);
    } else if (strcmp(nome_campo, "dataCrime") == 0) {
      free(retornar_data_crime(D_novo));
      salvar_data_crime(D_novo, valor_buscado_aux);
    } else if (strcmp(nome_campo, "numeroArtigo") == 0) {
      salvar_numero_artigo(D_novo, atoi(valor_buscado_aux));
      free(valor_buscado_aux);
    } else if (strcmp(nome_campo, "marcaCelular") == 0) {
      free(retornar_marca_celular(D_novo));
      salvar_marca_celular(D_novo, valor_buscado_aux);
    } else if (strcmp(nome_campo, "lugarCrime") == 0) {
      if (strcmp(lugar_crime_velho, "NULO") != 0) {
        tam_campos_variaveis_velho += strlen(lugar_crime_velho);
      }
      tam_campos_variaveis_novo += strlen(valor_buscado);

      free(retornar_lugar_crime(D_novo));
      salvar_lugar_crime(D_novo, valor_buscado_aux);
    } else if (strcmp(nome_campo, "descricaoCrime") == 0) { //descricaoCrime
      if (strcmp(descricao_crime_velho, "NULO") != 0) {
        tam_campos_variaveis_velho += strlen(descricao_crime_velho);
      }
      tam_campos_variaveis_novo += strlen(valor_buscado);

      free(retornar_descricao_crime(D_novo));
      salvar_descricao_crime(D_novo, valor_buscado_aux);
    }
  }

  // Retornando diferença entre o tamanho antigo e o tamanho novo do registro.
  return tam_campos_variaveis_velho - tam_campos_variaveis_novo;
}

// Atualiza os campos de um registro de dados no arquivo de dados e seu índice no vetor de índices.
void **realizar_atualizacao(FILE *arq_dados, reg_dados *D, long int byte_offset, criterio *vetor_atualizacoes, int qntd_campos_atualizados, 
                            reg_cabecalho *C, void **vetor_indice, char *tipo_dado, char *campo_indexado, int *tam_vetor_indice) {
  // Alocando registro de dados auxiliar.
  reg_dados *D_novo = alocar_reg_dados();
  char *valor; /*Variável auxiliar*/

  // Copiando campos do registro a ser atualizado.
  copiar_reg_dados(D_novo, D);
  
  // Atualizando os campos do registro e calculando a diferença de tamanho do registro antes e depois da atualização.
  int dif_tam_campos_variavies = atualizar_registro(arq_dados, D, D_novo, vetor_atualizacoes, qntd_campos_atualizados);

  // Verificando se o registro atualizado cabe no lugar do registro antigo. Se sim, editando informações no mesmo lugar.
  if (dif_tam_campos_variavies >= 0) { 

    // Encontrando RRN do registro a ser atualizado.
    int rrn_indice = -1;
    encontrar_rrn(arq_dados, D, byte_offset, vetor_indice, &rrn_indice, (*tam_vetor_indice), campo_indexado, tipo_dado);

    // Verificando se o registro está no vetor.
    if (rrn_indice != -1) { 
      if (strcmp(tipo_dado, "inteiro") == 0) {
        int valor_int = retornar_campo_int(D_novo, campo_indexado);

        // Caso o novo valor de indíce seja nulo, apagando logicamente do vetor de índices (byte offset negativo). Será apagado fisicamente depois.
        if (valor_int == -1) {
          long int byte_offset = retornar_byte_offset(vetor_indice[rrn_indice], campo_indexado);
          salvar_byte_offset(vetor_indice[rrn_indice], -1 * abs(byte_offset), campo_indexado);
          (*tam_vetor_indice)--;
        } 
        // Caso o novo valor seja não nulo, trocando a chave de busca.
        else { 
          salvar_chave_busca_int(vetor_indice[rrn_indice], valor_int);
        }
      } else { // tipo_dado == string
        valor = retornar_campo_string(D_novo, campo_indexado);
        tirar_cifrao(valor);

        // Caso o novo valor de indíce seja nulo, apagando logicamente do vetor de índices (byte offset negativo). Será apagado fisicamente depois.
        if (strcmp(valor, "NULO") == 0 || valor[0] == '$') { 
          long int byte_offset = retornar_byte_offset(vetor_indice[rrn_indice], campo_indexado);
          salvar_byte_offset(vetor_indice[rrn_indice], -1 * abs(byte_offset), campo_indexado);
          (*tam_vetor_indice)--;
        } 
        // Caso o novo valor seja não nulo, trocando a chave de busca.
        else { 
          char *string_aux = (char *) malloc((strlen(valor) + 1) * sizeof(char));
          strcpy(string_aux, valor);
          free(retornar_chave_busca_string(vetor_indice[rrn_indice]));
          salvar_chave_busca_string(vetor_indice[rrn_indice], string_aux);
        }
      }
    } 
    // O registro não está no vetor.
    else { 
      if (strcmp(tipo_dado, "inteiro") == 0) {
        int valor_int = retornar_campo_int(D_novo, campo_indexado);

        // Caso o novo valor de índice seja não nulo, criando e inserindo no vetor de índices.
        if (valor_int != -1) { 
          vetor_indice = (void**) preencher_vetor_int(vetor_indice, tam_vetor_indice, valor_int, byte_offset);
        }
      } else { // tipo_dado == string
        valor = retornar_campo_string(D_novo, campo_indexado);

        // Caso o novo valor de índice seja não nulo, criando e inserindo no vetor de índices.
        if (strcmp(valor, "NULO") != 0 && valor[0] != '$') { 
          vetor_indice = (void **) preencher_vetor_string(vetor_indice, tam_vetor_indice, valor, byte_offset);
        }
      }
    }

    // Escrevendo o registro atualizado no mesmo lugar no arquivo de dados.
    fseek(arq_dados, byte_offset, SEEK_SET);
    escrever_reg_dados(arq_dados, &D_novo);
    fseek(arq_dados, -1, SEEK_CUR);

    // Escrevendo lixo, se necessário.
    char *char_auxiliar = (char *) malloc(1 * sizeof(char));
    *char_auxiliar = '$';
    for (int i = 0; i < dif_tam_campos_variavies; i++) {
      fwrite(char_auxiliar, sizeof(char), 1, arq_dados);
    }

    // Escrevendo delimitador de campo.
    *char_auxiliar = '#';
    fwrite(char_auxiliar, sizeof(char), 1, arq_dados);

    // Liberando memória.
    free(char_auxiliar);
  } 
  
  // Verificando se o registro atualizado cabe no lugar do registro antigo. Se não, apagando o antigo e escrevendo o atualizado no fim.
  else { 

    // Encontrando RRN do registro a ser atualizado.
    int rrn_indice = -1;
    encontrar_rrn(arq_dados, D, byte_offset, vetor_indice, &rrn_indice, (*tam_vetor_indice), campo_indexado, tipo_dado);

    // Verificando se o registro está no vetor.
    if (rrn_indice != -1) { 
    
      if (strcmp(tipo_dado, "inteiro") == 0) {
        int valor_int = retornar_campo_int(D_novo, campo_indexado);

        // Caso o novo valor de indíce seja nulo, apagando logicamente do vetor de índices (byte offset negativo). Será apagado fisicamente depois.
        if (valor_int == -1) { 
          long int byte_offset = retornar_byte_offset(vetor_indice[rrn_indice], campo_indexado);
          salvar_byte_offset(vetor_indice[rrn_indice], -1 * abs(byte_offset), campo_indexado);
          (*tam_vetor_indice)--;
        } 
        // Caso o novo valor seja não nulo, trocando a chave de busca e byte offset.
        else { 
          salvar_chave_busca_int(vetor_indice[rrn_indice], valor_int);
          long int byte_offset = retornar_prox_byte_offset(C);
          salvar_byte_offset_int(vetor_indice[rrn_indice], byte_offset);
        }
      } else { // tipo_dado == string
        valor = retornar_campo_string(D_novo, campo_indexado);

        // Caso o novo valor de indíce seja nulo, apagando logicamente do vetor de índices (byte offset negativo). Será apagado fisicamente depois.
        if (strcmp(valor, "NULO") == 0 || valor[0] == '$') {
          long int byte_offset = retornar_byte_offset(vetor_indice[rrn_indice], campo_indexado);
          salvar_byte_offset(vetor_indice[rrn_indice], -1 * abs(byte_offset), campo_indexado);
          (*tam_vetor_indice)--;
        } 
        // Caso o novo valor seja não nulo, trocando a chave de busca e o byte offset.
        else {
          char *string_aux = (char *) malloc((strlen(valor) + 1) * sizeof(char));
          strcpy(string_aux, valor);
          free(retornar_chave_busca_string(vetor_indice[rrn_indice]));
          salvar_chave_busca_string(vetor_indice[rrn_indice], string_aux);
          long int byte_offset = retornar_prox_byte_offset(C);
          salvar_byte_offset_string(vetor_indice[rrn_indice], byte_offset);
        }
      }
    } 
    // O registro não está no vetor.
    else { 
      if (strcmp(tipo_dado, "inteiro") == 0) {
        int valor_int = retornar_campo_int(D_novo, campo_indexado);

        // Caso o novo valor de índice seja não nulo, criando e inserindo no vetor de índices.
        if (valor_int != -1) { 
         vetor_indice = (void **) preencher_vetor_int(vetor_indice, tam_vetor_indice, valor_int, byte_offset);
        }
      } else { // tipo_dado == string
        valor = retornar_campo_string(D_novo, campo_indexado);

        // Caso o novo valor de índice seja não nulo, criando e inserindo no vetor de índices.
        if (strcmp(valor, "NULO") != 0 && valor[0] != '$') {
          vetor_indice = (void **) preencher_vetor_string(vetor_indice, tam_vetor_indice, valor, byte_offset);
        }
      }
    }

    // Escrevendo o registro atualizado no fim do arquivo de dados.
    fseek(arq_dados, 0, SEEK_END);
    int bytes_escritos = escrever_reg_dados(arq_dados, &D_novo);

    // Atualizando o cabeçalho do arquivo de dados.
    atualizar_reg_cabecalho(C, retornar_status(C), retornar_prox_byte_offset(C) + bytes_escritos, retornar_nro_reg_arq(C) + 1, retornar_nro_reg_rem(C) + 1);

    // Marcando registro anterior como removido.
    fseek(arq_dados, byte_offset, SEEK_SET);
    salvar_removido(D, '1');
    escrever_reg_dados(arq_dados, &D);
  }

  // Limpando campos de tamanho variável.
  limpar_campos_reg_dados(D_novo);

  // Desalocando registro auxiliar.
  desalocar_reg_dados(&D_novo);

  // Retornando vetor de índices após a atualização.
  return vetor_indice;
}