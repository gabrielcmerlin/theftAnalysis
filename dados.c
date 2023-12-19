// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação do tipo para registro de dados e suas funções relacionadas, como retornar e 
  salvar campos; alocar e desalocar registro; ler e escrever registro; imprimir, copiar e comparar registros.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "dados.h"
#include "utils.h"
#include "indice.h" 

// Define registro de dados.
struct _reg_dados {
  char removido;
  int id_crime;
  char *data_crime;
  int numero_artigo;
  char *marca_celular;
  char *lugar_crime;
  char *descricao_crime;
  char delimitador;
};

// Aloca um registro de dados.
reg_dados *alocar_reg_dados(){
  reg_dados *D = (reg_dados *) calloc(1, sizeof(reg_dados));

  return D;
}

// Desaloca um registro de dados.
void desalocar_reg_dados(reg_dados **D) {
  if(D != NULL && *D != NULL) {
    free(*D);
    *D = NULL;
  }
}

// Atualiza o status de um registro de dados.
void salvar_removido(reg_dados *D, char removido) {
  D->removido = removido;
}

// Atualiza o ID de um registro de dados.
void salvar_id(reg_dados *D, int id) {
  D->id_crime = id;
}

// Atualiza a data do crime de um registro de dados.
void salvar_data_crime(reg_dados *D, char *data_crime) {
  D->data_crime = data_crime;
}

// Atualiza o número do artigo de um registro de dados.
void salvar_numero_artigo(reg_dados *D, int numero_artigo) {
  D->numero_artigo = numero_artigo;
}

// Atualiza a marca de celular de um registro de dados.
void salvar_marca_celular(reg_dados *D, char *marca_celular) {
  D->marca_celular = marca_celular;
}

// Atualiza o lugar do crime de um registro de dados.
void salvar_lugar_crime(reg_dados *D, char *lugar_crime) {
  D->lugar_crime = lugar_crime;
}

// Atualiza descrição do crime de um registro de dados.
void salvar_descricao_crime(reg_dados *D, char *descricao_crime) {
  D->descricao_crime = descricao_crime;
}

// Atualiza delimitador de registro de um registro de dados.
void salvar_delimitador_reg(reg_dados *D, char delimitador){
  D->delimitador = delimitador;
}

// Retorna o status de um registro de dados.
char retornar_removido(reg_dados *D){
  return D->removido;
}

// Retorna o ID de um registro de dados.
int retornar_id(reg_dados *D){
  return D->id_crime;
}

// Retorna a data do crime de um registro de dados.
char *retornar_data_crime(reg_dados *D){
  return D->data_crime;
}

// Retorna o número do artigo de um registro de dados.
int retornar_numero_artigo(reg_dados *D){
  return D->numero_artigo;
}

// Retorna a marca de celular de um registro de dados.
char *retornar_marca_celular(reg_dados *D){
  return D->marca_celular;
}

// Retorna o lugar do crime de um registro de dados.
char *retornar_lugar_crime(reg_dados *D){
  return D->lugar_crime;
}

// Retorna a descrição do crime de um registro de dados.
char *retornar_descricao_crime(reg_dados *D){
  return D->descricao_crime;
}

// Retorna o delimitador de um registro de dados.
char retornar_delimitador(reg_dados *D){
  return D->delimitador;
}

// Limpa os campos de tamanho variável de um registro de dados.
void limpar_campos_reg_dados(reg_dados *D){
  free(D->data_crime);
  free(D->lugar_crime);
  free(D->descricao_crime);
  free(D->marca_celular);
}

// Escreve o campo 'removido' de um registro de dados em um arquivo binário. 
int escrever_removido(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  char removido = retornar_removido(D);
  *bytes_escritos += fwrite(&removido, sizeof(char), 1, arq_dados);
}

// Escreve o ID de um registro de dados em um arquivo binário. 
int escrever_id(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  int id = retornar_id(D);
  *bytes_escritos += sizeof(int) * fwrite(&id, sizeof(int), 1, arq_dados);
}

// Escreve a data do crime de um registro de dados em um arquivo binário. 
int escrever_data_crime(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  char *data_crime = retornar_data_crime(D);

  // Verificando se data do crime é uma informação nula. 
  // Se não, escrevendo a data. Se sim, escrevendo 'lixo'.
  if(data_crime != NULL && strcmp(data_crime, "NULO") != 0) {
    *bytes_escritos += fwrite(data_crime, sizeof(char), 10, arq_dados);
  } else {
    char data_nula[10] = "$$$$$$$$$$";
    *bytes_escritos += fwrite(&data_nula, sizeof(char), 10, arq_dados);
  }
}

// Escreve o número do artigo de um registro de dados em um arquivo binário. 
int escrever_numero_artigo(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  int numero_artigo = retornar_numero_artigo(D);
  *bytes_escritos += sizeof(int) * fwrite(&numero_artigo, sizeof(int), 1, arq_dados);
}

// Escreve a marca do celular de um registro de dados em um arquivo binário. 
int escrever_marca_celular(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  char *marca_celular = retornar_marca_celular(D);
  int tam_string;

  // Verificando se marca do celular é uma informação nula. 
  // Se não, escrevendo a marca e completando com 'lixo' se necessário. Se sim, escrevendo 'lixo'.
  if(marca_celular != NULL && strcmp(marca_celular, "NULO") != 0) {
    tam_string = strlen(marca_celular);
    if(tam_string < 12){
        marca_celular = (char *) realloc(marca_celular, sizeof(char) * 12);
        for(int i = tam_string; i < 12; i++){
            marca_celular[i] = '$';
        }
        salvar_marca_celular(D, marca_celular);
    }
    *bytes_escritos += fwrite(marca_celular, sizeof(char), 12, arq_dados);
  } else {
    char marca_nula[12] = "$$$$$$$$$$$$";
    *bytes_escritos += fwrite(&marca_nula, sizeof(char), 12, arq_dados);
  }
}

// Escreve o lugar do crime de um registro de dados em um arquivo binário. 
int escrever_lugar_crime(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  char *lugar_crime = retornar_lugar_crime(D);
  char delimitador = '|';

  // Escrevendo lugar do crime (campo de tamanho variável) e delimitador de campo;
  if(lugar_crime != NULL && strcmp(lugar_crime, "NULO") != 0) {
      *bytes_escritos += fwrite(lugar_crime, sizeof(char), strlen(lugar_crime), arq_dados);
  }
  *bytes_escritos += fwrite(&delimitador, sizeof(char), 1, arq_dados);
}

// Escreve a descrição do crime de um registro de dados em um arquivo binário. 
int escrever_descricao_crime(FILE *arq_dados, reg_dados *D, int *bytes_escritos){
  char *descricao_crime = retornar_descricao_crime(D);
  char delimitador = '|';  /*Delimitador de campo*/

  // Escrevendo lugar do crime (campo de tamanho variável) e delimitador de campo.
  if(descricao_crime != NULL && strcmp(descricao_crime, "NULO") != 0) {
      *bytes_escritos += fwrite(descricao_crime, sizeof(char), strlen(descricao_crime), arq_dados);
  }
  *bytes_escritos += fwrite(&delimitador, sizeof(char), 1, arq_dados);
}

// Escreve o delimitador de um registro de dados em um arquivo binário. 
int escrever_delimitador_reg(FILE *arq_dados, int *bytes_escritos){
  char delimitador = '#'; /*Delimitador de registro*/
  *bytes_escritos += fwrite(&delimitador, sizeof(char), 1, arq_dados);
}

// Escreve um registro de dados em um arquivo binário. Retorna o nº de bytes escritos.
int escrever_reg_dados(FILE *arq_dados, reg_dados **D) {
  int bytes_escritos = 0; /*Contador para bytes escritos*/

  // Escrevendo informações do registro de dados no arquivo binário
  // e contando a quantidade de bytes escritos. 
  escrever_removido(arq_dados, *D, &bytes_escritos);
  escrever_id(arq_dados, *D, &bytes_escritos);
  escrever_data_crime(arq_dados, *D, &bytes_escritos);
  escrever_numero_artigo(arq_dados, *D, &bytes_escritos);
  escrever_marca_celular(arq_dados, *D, &bytes_escritos);
  escrever_lugar_crime(arq_dados, *D, &bytes_escritos);
  escrever_descricao_crime(arq_dados, *D, &bytes_escritos);
  escrever_delimitador_reg(arq_dados, &bytes_escritos); 

  // Retornando a quantidade de bytes escritos.
  return bytes_escritos;
}

// Lê o campo 'removido' de um arquivo binário e salva em um registro de dados. 
void ler_removido_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *removido = (char *) malloc(1 * sizeof(char));

  *bytes_percorridos += fread(removido, sizeof(char), 1, arq_dados);
  salvar_removido(D, *removido);

  free(removido);
}

// Lê o campo 'ID' de um arquivo binário e salva em um registro de dados. 
void ler_id_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  int *id = (int *) malloc(1 * sizeof(int));

  *bytes_percorridos += sizeof(int) * fread(id, sizeof(int), 1, arq_dados);
  salvar_id(D, *id);

  free(id);
}

// Lê o campo 'dataCrime' de um arquivo binário e salva em um registro de dados. 
void ler_data_crime_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *data = (char *) malloc(11 * sizeof(char));

  *bytes_percorridos += fread(data, sizeof(char), 10, arq_dados);
  // Verificando se data do crime é uma informação nula.
  // Se sim, atribuindo mensagem padrão "NULO".
  if(data[0] == '$') {
    strcpy(data, "NULO\0");
  }
  data[10] = '\0';
  salvar_data_crime(D, data);
}

// Lê o campo 'numeroArtigo' de um arquivo binário e salva em um registro de dados. 
void ler_numero_artigo_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  int *artigo = (int *) malloc(1 * sizeof(int));

  *bytes_percorridos += sizeof(int) * fread(artigo, sizeof(int), 1, arq_dados);
  salvar_numero_artigo(D, *artigo);

  free(artigo);
}

// Lê o campo 'marcaCelular' de um arquivo binário e salva em um registro de dados. 
void ler_marca_celular_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *marca = (char *) malloc(13 * sizeof(char));

  *bytes_percorridos += fread(marca, sizeof(char), 12, arq_dados);
  // Verificando se marca é uma informação nula.
  // Se sim, atribuindo mensagem padrão "NULO". Depois, limpando 'lixo'.
  if(marca[0] == '$') {
    strcpy(marca, "NULO\0");
  }
  marca[12] = '\0';
  tirar_cifrao(marca);
  salvar_marca_celular(D, marca);
}

// Lê o campo 'lugarCrime' de um arquivo binário e salva em um registro de dados. 
void ler_lugar_crime_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *c = (char *) malloc(1 * sizeof(char));
  char *lugar = NULL;
  int qntd_chars = 0;

  *bytes_percorridos += fread(c, sizeof(char), 1, arq_dados);

  // Lendo o lugar até o delimitador de campo.
  while(*c != '|'){
    qntd_chars++;
    lugar = (char *) realloc(lugar, qntd_chars * sizeof(char));
    lugar[qntd_chars - 1] = *c;
    *bytes_percorridos += fread(c, sizeof(char), 1, arq_dados);
  }
  // Verificando se lugar é uma informação nula.
  // Se sim, atribuindo mensagem padrão "NULO".
  if(qntd_chars == 0) {
    lugar = (char *) realloc(lugar, 5 * sizeof(char));
    strcpy(lugar, "NULO\0");
  } else {
    lugar = (char *) realloc(lugar, (qntd_chars + 1) * sizeof(char));
    lugar[qntd_chars] = '\0';
  }
  salvar_lugar_crime(D, lugar);

  // Liberando memória alocada.
  free(c);
}

// Lê o campo 'descricaoCrime' de um arquivo binário e salva em um registro de dados. 
void ler_descricao_crime_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *c = (char *) malloc(1 * sizeof(char));
  char *descricao = NULL;
  int qntd_chars = 0;

  *bytes_percorridos += fread(c, sizeof(char), 1, arq_dados);

  // Lendo a descrição até o delimitador de campo.
  while(*c != '|'){
    qntd_chars++;
    descricao = (char *) realloc(descricao, qntd_chars * sizeof(char));
    descricao[qntd_chars - 1] = *c;
    *bytes_percorridos += fread(c, sizeof(char), 1, arq_dados);
  }
  // Verificando se descrição é uma informação nula.
  // Se sim, atribuindo mensagem padrão "NULO". 
  if(qntd_chars == 0) {
    descricao = (char *) realloc(descricao, 5 * sizeof(char));
    strcpy(descricao, "NULO\0");
  } else {
    descricao = (char *) realloc(descricao, (qntd_chars + 1) * sizeof(char));
    descricao[qntd_chars] = '\0';
  }
  salvar_descricao_crime(D, descricao);

  // Liberando memória alocada.
  free(c);
}

// Lê o o delimitador de registro de um arquivo binário e salva em um registro de dados. 
void ler_delimitador_reg_dados(FILE *arq_dados, reg_dados *D, int *bytes_percorridos) {
  char *c = (char *) malloc(1 * sizeof(char));

  *bytes_percorridos += fread(c, sizeof(char), 1, arq_dados);
  salvar_delimitador_reg(D, *c);

  free(c);
}

// Lê todos os campos de um registro de dados de um arquivo binário e salva em um registro. Retorna o nº de bytes percorridos durante a leitura.
int ler_reg_dados(FILE *arq_dados, reg_dados *D) {
  int bytes_percorridos = 0;

  // Lendo campos do registro no arquivo binário.
  ler_removido_dados(arq_dados, D, &bytes_percorridos);
  ler_id_dados(arq_dados, D, &bytes_percorridos);
  ler_data_crime_dados(arq_dados, D, &bytes_percorridos);
  ler_numero_artigo_dados(arq_dados, D, &bytes_percorridos);
  ler_marca_celular_dados(arq_dados, D, &bytes_percorridos);
  ler_lugar_crime_dados(arq_dados, D, &bytes_percorridos);
  ler_descricao_crime_dados(arq_dados, D, &bytes_percorridos);
  ler_delimitador_reg_dados(arq_dados, D, &bytes_percorridos);

  // Retornando a quantidade de bytes percorridos durante a leitura do arquivo.
  return bytes_percorridos;
}

// Imprime o número do artigo de um registro de dados.  
void imprimir_num_artigo(int artigo) {
  char *artigo_string;  /*String auxiliar*/

  // Verificando se número do artigo é uma informação nula.
  // Se sim, atribuindo mensagem padrão "NULO".
  // Se não, convertendo seu valor (int) para string.
  // Dessa forma, numero_artigo será sempre uma string, facilitando a impressão.
  if(artigo == -1) {
    artigo_string = (char *) malloc(5 * sizeof(char));
    strcpy(artigo_string, "NULO\0");
  } else {
    artigo_string = (char *) malloc((qntd_digitos(artigo) + 1) * sizeof(char));
    int_para_string(artigo, &artigo_string);
  }

  // Imprimindo número do artigo.
  printf("%s, ", artigo_string);

  // Liberando memória alocada.
  free(artigo_string);
}

// Imprime todos os campos de um registro de dados.
void imprimir_reg_dados(reg_dados *D) {
  printf("%d, ", retornar_id(D));
  printf("%s, ", retornar_data_crime(D));
  imprimir_num_artigo(retornar_numero_artigo(D));
  printf("%s, ", retornar_lugar_crime(D));
  printf("%s, ", retornar_descricao_crime(D));
  printf("%s\n", retornar_marca_celular(D));
}

// Copia os campos de um registro de dados fonte para um registro de dados destino.
void copiar_reg_dados(reg_dados *D_dest, reg_dados *D_fonte) {

  // Alocando memória e copiando campos de tamanho variável.
  char *data_crime = (char *) malloc((strlen(retornar_data_crime(D_fonte)) + 1) * sizeof(char));
  strcpy(data_crime, retornar_data_crime(D_fonte));
  char *marca_celular = (char *) malloc((strlen(retornar_marca_celular(D_fonte)) + 1) * sizeof(char));
  strcpy(marca_celular, retornar_marca_celular(D_fonte));
  char *lugar_crime = (char *) malloc((strlen(retornar_lugar_crime(D_fonte)) + 1) * sizeof(char));
  strcpy(lugar_crime, retornar_lugar_crime(D_fonte));
  char *descricao_crime = (char *) malloc((strlen(retornar_descricao_crime(D_fonte)) + 1) * sizeof(char));
  strcpy(descricao_crime, retornar_descricao_crime(D_fonte));

  // Copiando, campo a campo, um registro de dados para outro.
  salvar_removido(D_dest, retornar_removido(D_fonte));
  salvar_id(D_dest, retornar_id(D_fonte));
  salvar_data_crime(D_dest, data_crime);
  salvar_numero_artigo(D_dest, retornar_numero_artigo(D_fonte));
  salvar_marca_celular(D_dest, marca_celular);
  salvar_lugar_crime(D_dest, lugar_crime);
  salvar_descricao_crime(D_dest, descricao_crime);
  salvar_delimitador_reg(D_dest, retornar_delimitador(D_fonte));
}

// Retorna um campo do tipo 'int' de um registro de dados de acordo com o nome do campo.
int retornar_campo_int(reg_dados *D, char *campo_pedido) {
    if (strcmp(campo_pedido, "idCrime") == 0) {
        return retornar_id(D);
    } else { // numeroArtigo
        return retornar_numero_artigo(D);
    }
}

// Retorna um campo do tipo 'string' de um registro de dados de acordo com o nome do campo.
char *retornar_campo_string(reg_dados *D, char *campo_pedido) {
    if (strcmp(campo_pedido, "dataCrime") == 0) {
        return retornar_data_crime(D);
    } else if (strcmp(campo_pedido, "marcaCelular") == 0) {
        return retornar_marca_celular(D);
    } else if (strcmp(campo_pedido, "lugarCrime") == 0) {
        return retornar_lugar_crime(D);
    } else { // descricaoCrime
        return retornar_descricao_crime(D);
    }
}

// Compara um valor dado com o valor de um campo do registro de dados.
bool comparar_valor_reg_dados(reg_dados *D, char *nome_campo, char *valor_comparador){
  if(strcmp(nome_campo, "idCrime") == 0 || strcmp(nome_campo, "numeroArtigo") == 0) {
    int valor_real = retornar_campo_int(D, nome_campo);

    if(valor_real == atoi(valor_comparador)) return true;
  } else {
    char *valor_real = retornar_campo_string(D, nome_campo);

    if(strcmp(valor_real, valor_comparador) == 0) return true;
  }

  return false;
}

// Lê um registro de dados para a inserção.
int ler_para_inserir(reg_dados *D){
  // Declarando variáveis auxiliares para leitura.
  int bytes_lidos = 1;
  int id_crime, num_artigo;
  char string_aux[TAM_STRING];

  // Lendo cada campo de acordo com as especificações do trabalho.
  scanf(" %d", &id_crime);
  bytes_lidos += 4;
  scan_quote_string(string_aux);
  bytes_lidos += (strlen(string_aux) + 1);
  char *data_crime = (char *) malloc((strlen(string_aux) + 1) * sizeof(char));
  strcpy(data_crime, string_aux);
  scanf(" %s", string_aux);
  if (strcmp(string_aux, "NULO") == 0) {
    num_artigo = -1;
  } else {
    num_artigo = atoi(string_aux);
  }
  bytes_lidos += 4;
  scan_quote_string(string_aux);
  bytes_lidos += (strlen(string_aux) + 1);
  char *lugar_crime = (char *) malloc((strlen(string_aux) + 1) * sizeof(char));
  strcpy(lugar_crime, string_aux);
  scan_quote_string(string_aux);
  bytes_lidos += (strlen(string_aux) + 1);
  char *descricao_crime = (char *) malloc((strlen(string_aux) + 1) * sizeof(char));
  strcpy(descricao_crime, string_aux);
  scan_quote_string(string_aux);
  bytes_lidos += (strlen(string_aux) + 1);
  char *marca_celular = (char *) malloc((strlen(string_aux) + 1) * sizeof(char));
  strcpy(marca_celular, string_aux);

  // Salvando as informações lidas no registro.
  salvar_removido(D, '0');
  salvar_id(D, id_crime);
  salvar_data_crime(D, data_crime);
  salvar_numero_artigo(D, num_artigo);
  salvar_lugar_crime(D, lugar_crime);
  salvar_descricao_crime(D, descricao_crime);
  salvar_marca_celular(D, marca_celular);

  return bytes_lidos;
}

// Marca logicamente um registro de dados como removido no arquivo de dados.
void marcar_como_removido(FILE *arq_dados, reg_dados *D, long int byte_offset){
  fseek(arq_dados, byte_offset, SEEK_SET);
  salvar_removido(D, '1');
  escrever_reg_dados(arq_dados, &D);
}