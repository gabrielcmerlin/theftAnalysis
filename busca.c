// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funções relacionadas às buscas binária e sequencial utilizadas no trabalho, 
  para cada tipo de funcionalidade (imprimir e deletar).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivos.h"
#include "dados.h"
#include "cabecalho.h"
#include "utils.h"
#include "indice.h"
#include "busca.h"
#include "b_estrela.h"

// Realiza a etapa recursiva da busca binária para dados do tipo 'int'.
int busca_binaria_int(FILE *arq_dados, indice_dados_int **vetor, int ini, int fim, int *valor) {
  // Caso base da recursão.
  if (ini > fim) {
    return -1;
  }

  // Calculando índice i da busca binária e encontrando chave a ser comparada.
  int i = (ini + fim) / 2;
  int chave_busca = retornar_chave_busca_int(vetor[i]);

  // Comparando chave com o valor buscado.
  if (chave_busca == *valor) {
    return i;
  } else if (chave_busca < *valor) {
    return busca_binaria_int(arq_dados, vetor, i + 1, fim, valor);
  } else {
    return busca_binaria_int(arq_dados, vetor, ini, i - 1, valor);
  }
}

// Realiza a etapa recursiva da busca binária para dados do tipo 'string'.
int busca_binaria_string(FILE *arq_dados, indice_dados_string **vetor, int ini, int fim, char *valor) {
  // Caso base da recursão.
  if (ini > fim) {
    return -1;
  }
  
  // Calculando índice i da busca binária e encontrando chave a ser comparada.
  int i = (ini + fim) / 2;
  char *chave_busca = retornar_chave_busca_string(vetor[i]);

  // Comparando chave com o valor buscado.
  if (strcmp(chave_busca, valor) == 0) {
    return i;
  } else if (strcmp(chave_busca, valor) < 0) {
    return busca_binaria_string(arq_dados, vetor, i + 1, fim, valor);
  } else {
    return busca_binaria_string(arq_dados, vetor, ini, i - 1, valor);
  }
}

// Realiza a chamada da etapa recursiva da busca binária.
int busca_binaria(FILE *arq_dados, void **vetor, int ini, int fim, char *valor, char *tipo_dado) {
  // Verificando tipo do dado a ser buscado.
  if (strcmp(tipo_dado, "inteiro") == 0) {
    int valor_int = atoi(valor);
    return busca_binaria_int(arq_dados, (indice_dados_int **) vetor, ini, fim, &valor_int);
  } else {
    return busca_binaria_string(arq_dados, (indice_dados_string **) vetor, ini, fim, valor);
  }
}

// Realiza a busca de um registro de acordo com um campo indexado (busca binária em um vetor) e sua impressão/deleção.
void buscar_indexado(FILE *arq_dados, void** vetor_indice, criterio *vetor_condicoes, int tam_vetor_indice, 
                              char *tipo_dado, char *nome_campo_condicao, int *contador, int qntd_condicoes,
                              char *campo_indexado, int flag){
  
  // Inicializando variável auxiliar para comparação.
  char *valor = vetor_condicoes[0].valor_buscado;

  // Calculando posição inicial da busca.
  int posicao_inicial = busca_binaria(arq_dados, vetor_indice, 0, tam_vetor_indice - 1, valor, tipo_dado);
  strcpy(nome_campo_condicao, vetor_condicoes[0].nome_campo);

  // Verificando se o retorno da busca é uma posição válida. 
  if (posicao_inicial >= 0) { 

    // Atribuindo chave de busca do registro analisado. 
    void *chave_busca = retornar_chave_busca(vetor_indice[posicao_inicial], nome_campo_condicao);

    // Encontrando o primeiro registro com a chave buscada.
    while (comparar_chave_busca(chave_busca, valor, nome_campo_condicao) && posicao_inicial > 0) {
      posicao_inicial--;

      // Liberando memória.
      if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);

      // Reatribuindo chave de busca. 
      chave_busca = retornar_chave_busca(vetor_indice[posicao_inicial], nome_campo_condicao);
    }

    // Adequando posição encontrada para o primeiro registro quando a chave comparada é diferente da buscada.
    // Isso pode ocorre porque o laço acima decrementa após comparar a chave e o valor buscado. 
    if (comparar_chave_busca(chave_busca, valor, nome_campo_condicao) == false) posicao_inicial++;
    
    // Liberando memória alocada.
    if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);

    // Declarando registro de dados auxiliar.
    reg_dados *D = alocar_reg_dados();

    // Posicionando cursor do arquivo e lendo registro de dados. 
    long int byte_offset = abs(retornar_byte_offset(vetor_indice[posicao_inicial], nome_campo_condicao));
    fseek(arq_dados, byte_offset, SEEK_SET);
    ler_reg_dados(arq_dados, D);

    // Declarando variável auxiliar.
    int condicoes_iguais;
    
    // Encontrando registro não removido com a primeira condição buscada.
    while (comparar_valor_reg_dados(D, nome_campo_condicao, valor)) {
      condicoes_iguais = 1; /*Aqui, a primeira condição buscada já está satisfeita*/

      // Verificando se as demais condições buscadas são satisfeitas pelo registro analisado.
      for (int j = 1; j < qntd_condicoes; j++) {
        if (comparar_valor_reg_dados(D, vetor_condicoes[j].nome_campo, vetor_condicoes[j].valor_buscado)) {
          condicoes_iguais++;
        }
      }

      // Realizando ação da busca.
      if (flag == 1){
        // Imprimindo registro buscado.
        if(condicoes_iguais == qntd_condicoes && retornar_removido(D) == '0') {
        (*contador)++;
        imprimir_reg_dados(D);
        }
      } else if (flag == 2){
         // Deletando registro buscado.
        if (condicoes_iguais == qntd_condicoes && retornar_removido(D) == '0') {
          (*contador)++;
          realizar_delecao(arq_dados, D, retornar_byte_offset(vetor_indice[posicao_inicial], tipo_dado), vetor_indice, posicao_inicial, tam_vetor_indice, campo_indexado, tipo_dado);
        } 
      }

      // Incrementando contador.
      posicao_inicial++;

      // Limpando campos de tamanho variável do registro de dados auxiliar.
      limpar_campos_reg_dados(D);

      // Posicionando cursor do arquivo e lendo próximo registro de dados. 
      byte_offset = abs(retornar_byte_offset_int(vetor_indice[posicao_inicial]));
      fseek(arq_dados, byte_offset, SEEK_SET);
      ler_reg_dados(arq_dados, D);
    }

    // Limpando campos de tamanho variável do registro de dados.
    limpar_campos_reg_dados(D);

    // Desalocando registro de dados auxiliar.
    desalocar_reg_dados(&D);
  }

}

// Realiza a busca de um registro de acordo com um campo não indexado (busca sequencial no arquivo) e sua impressão/deleção. 
void buscar_nao_indexado(FILE *arq_dados, criterio *vetor_condicoes, int *contador, int qntd_condicoes,
                                  int tam_vetor_indice, char *tipo_dado, char *campo_indexado, void** vetor_indice, int flag){
  // Posicionando cursor do arquivo no início para garantir o bom funcionamento da busca.
  fseek(arq_dados, 0, SEEK_SET);

  // Alocando registros auxiliares.
  reg_dados *D = alocar_reg_dados();
  reg_cabecalho *C = alocar_reg_cabecalho();

  // Inicializando variáveis auxiliares.
  int tam_cabecalho = ler_reg_cabecalho(arq_dados, C);
  int total_bytes = retornar_prox_byte_offset(C);
  int condicoes_iguais, bytes_lidos;

  // Percorrendo o arquivo de dados a partir do registro 0.
  for (int bytes_percorridos = tam_cabecalho; bytes_percorridos < total_bytes;) {
    
    // Lendo registro de dados.
    bytes_lidos = ler_reg_dados(arq_dados, D);

    // Encontrando registro não removido com a primeira condição buscada.
    if (retornar_removido(D) == '0' && comparar_valor_reg_dados(D, vetor_condicoes[0].nome_campo, vetor_condicoes[0].valor_buscado)) {
      condicoes_iguais = 1;

      // Verificando se as demais condições buscadas são satisfeitas pelo registro analisado.
      for (int j = 1; j < qntd_condicoes; j++) {
        if (comparar_valor_reg_dados(D, vetor_condicoes[j].nome_campo, vetor_condicoes[j].valor_buscado)) {
          condicoes_iguais++;
        }
      }
      
      if(flag == 1){
        // Imprimindo registro buscado.
        if (condicoes_iguais == qntd_condicoes) {
          (*contador)++;
          imprimir_reg_dados(D);
        }
      } else if (flag == 2){
        // Deletando registro buscado.
        if (condicoes_iguais == qntd_condicoes && retornar_removido(D) == '0') {
          (*contador)++;
          // Aqui, a posição do registro no vetor de índices é informada como -1, pois não é possível sabê-la. 
          // Isso porque realizou-se a busca sequencial no arquivo de dados, não sendo necessário usar o vetor.
          realizar_delecao(arq_dados, D, bytes_percorridos, vetor_indice, -1, tam_vetor_indice, campo_indexado, tipo_dado);
        }
      } 
    }

    // Reatribuindo contador de bytes percorridos no arquivo.
    bytes_percorridos += bytes_lidos;

    // Limpando campos de tamanho variável do registro de dados auxiliar.
    limpar_campos_reg_dados(D);
  }

  // Desalocando registros auxiliares.
  desalocar_reg_cabecalho(&C);
  desalocar_reg_dados(&D);
}

// Realiza a busca de um registro de acordo com um campo não indexado (busca sequencial no arquivo) e atualiza-o.
// Retorna o vetor de índice para facilitar a desalocação e evitar perda de referência.
void **buscar_indexado_atualizar(FILE *arq_dados, void** vetor_indice, criterio *vetor_condicoes, int *tam_vetor_indice, 
                               char *tipo_dado, char *nome_campo_condicao, int qntd_condicoes, char *campo_indexado,
                               criterio *vetor_atualizacoes, int qntd_campos_atualizados, reg_cabecalho *C){

  // Inicializando variável auxiliar para comparação.
  char *valor = vetor_condicoes[0].valor_buscado;

  // Calculando posição inicial da busca.
  int posicao_inicial = busca_binaria(arq_dados, vetor_indice, 0, (*tam_vetor_indice) - 1, valor, tipo_dado);
  strcpy(nome_campo_condicao, vetor_condicoes[0].nome_campo);

  // Verificando se o retorno da busca é uma posição válida.
  if (posicao_inicial >= 0) {
    
    // Atribuindo chave de busca do registro analisado. 
    void *chave_busca = retornar_chave_busca(vetor_indice[posicao_inicial], nome_campo_condicao);

    // Encontrando o primeiro registro com a chave buscada.
    while (comparar_chave_busca(chave_busca, valor, nome_campo_condicao) && posicao_inicial > 0) {
      posicao_inicial--;

      // Liberando memória.
      if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);
      
      // Reatribuindo chave de busca. 
      chave_busca = retornar_chave_busca(vetor_indice[posicao_inicial], nome_campo_condicao);
    }

    // Adequando posição encontrada para o primeiro registro quando a chave comparada é diferente da buscada.
    // Isso pode ocorre porque o laço acima decrementa após comparar a chave e o valor buscado. 
    if (comparar_chave_busca(chave_busca, valor, nome_campo_condicao) == false) posicao_inicial++;
    
    // Liberando memória alocada.
    if (strcmp(tipo_dado, "inteiro") == 0) free(chave_busca);
    
    // Declarando registro de dados auxiliar.
    reg_dados *D = alocar_reg_dados();
    
    // Posicionando cursor do arquivo e lendo registro de dados.
    long int byte_offset = abs(retornar_byte_offset(vetor_indice[posicao_inicial], nome_campo_condicao));
    fseek(arq_dados, byte_offset, SEEK_SET);
    ler_reg_dados(arq_dados, D);

    // Declarando variável auxiliar.
    int condicoes_iguais;

    // Encontrando registro não removido com a primeira condição buscada.
    while (retornar_removido(D) == '0' && comparar_valor_reg_dados(D, nome_campo_condicao, valor)) {
      condicoes_iguais = 1; /*Aqui, a primeira condição buscada já está satisfeita*/

      // Verificando se as demais condições buscadas são satisfeitas pelo registro analisado.
      for (int j = 1; j < qntd_condicoes; j++) {
        if (comparar_valor_reg_dados(D, vetor_condicoes[j].nome_campo, vetor_condicoes[j].valor_buscado)) {
          condicoes_iguais++;
        }
      }

      // Atualizando registro buscado.
      if (condicoes_iguais == qntd_condicoes && retornar_removido(D) == '0') {
        long int byte_offset = retornar_byte_offset(vetor_indice[posicao_inicial], campo_indexado);
        vetor_indice = realizar_atualizacao(arq_dados, D, byte_offset, vetor_atualizacoes, qntd_campos_atualizados, C, vetor_indice, tipo_dado, campo_indexado, tam_vetor_indice);
      }

      // Incrementando contador.
      posicao_inicial++;

      // Limpando campos de tamanho variável do registro de dados auxiliar.
      limpar_campos_reg_dados(D);

      // Posicionando cursor do arquivo e lendo próximo registro de dados. 
      byte_offset = abs(retornar_byte_offset_int(vetor_indice[posicao_inicial]));
      fseek(arq_dados, byte_offset, SEEK_SET);
      ler_reg_dados(arq_dados, D);
    }

    // Limpando campos de tamanho variável do registro de dados.
    limpar_campos_reg_dados(D);

    // Desalocando registro de dados auxiliar.
    desalocar_reg_dados(&D);
  }

  // Retornando vetor atualizado.
  return vetor_indice;
}

// Realiza a busca de um registro de acordo com um campo não indexado (busca sequencial no arquivo) e atualiza-o. 
// Retorna o vetor de índice para facilitar a desalocação e evitar perda de referência.
void **buscar_nao_indexado_atualizar(FILE *arq_dados, void **vetor_indice, criterio *vetor_condicoes, 
                                   int qntd_condicoes, int *tam_vetor_indice, char *tipo_dado, char *campo_indexado,
                                   criterio *vetor_atualizacoes, int qntd_campos_atualizados, reg_cabecalho *C, int tam_cabecalho){
  // Inicializando variáveis auxiliares.
  long int total_bytes = retornar_prox_byte_offset(C);
  int condicoes_iguais, bytes_lidos;

  // Posicionando o cursor no registro 0.
  fseek(arq_dados, tam_cabecalho, SEEK_SET);

  // Alocando registro de dados auxiliar.
  reg_dados *D = alocar_reg_dados();

  // Percorrendo o arquivo de dados a partir do registro 0.
  for (long int bytes_percorridos = tam_cabecalho; bytes_percorridos < total_bytes;) {
    // Lendo registro de dados.
    bytes_lidos = ler_reg_dados(arq_dados, D);

    // Encontrando registro não removido com a primeira condição buscada.
    if (retornar_removido(D) == '0' && comparar_valor_reg_dados(D, vetor_condicoes[0].nome_campo, vetor_condicoes[0].valor_buscado)) {
      condicoes_iguais = 1;

      // Verificando se as demais condições buscadas são satisfeitas pelo registro analisado.
      for (int j = 1; j < qntd_condicoes; j++) {
        if (comparar_valor_reg_dados(D, vetor_condicoes[j].nome_campo, vetor_condicoes[j].valor_buscado)) {
          condicoes_iguais++;
        }
      }

      // Atualizando registro buscado.
      if (condicoes_iguais == qntd_condicoes) {
        vetor_indice = realizar_atualizacao(arq_dados, D, bytes_percorridos, vetor_atualizacoes, qntd_campos_atualizados, C, vetor_indice, tipo_dado, campo_indexado, tam_vetor_indice);
      }
    }

    // Reatribuindo contador de bytes percorridos no arquivo.
    bytes_percorridos += bytes_lidos;

    // Limpando campos de tamanho variável do registro de dados.
    limpar_campos_reg_dados(D);

    // Atualizando contador.
    total_bytes = retornar_prox_byte_offset(C);
  }

  // Desalocando registro de dados auxiliar.
  desalocar_reg_dados(&D);

  // Retornando vetor de índices atualizado.
  return vetor_indice;
}

// Busca um ou mais registros de um arquivo de dados em uma árvore-b* seguindo uma ou mais condições de busca e imprime-os.
void buscar_b_estrela(FILE *arq_dados, FILE *arq_b_estrela, criterio *vetor_condicoes, int qntd_condicoes, int *registros_achados, arv_cabecalho *C_arv) {
  // Armazenando campos da chave a ser buscada.
  int chave_buscada = atoi(vetor_condicoes[0].valor_buscado);
  long int byte_offset = encontrar_byte_offset(arq_b_estrela, C_arv->no_raiz, chave_buscada);

  // A chave buscada não foi encontrada.
  if (byte_offset == -1) {
    *registros_achados = 0;
    return;
  }

  // Lendo registro de dados buscado.
  fseek(arq_dados, byte_offset, SEEK_SET);
  reg_dados *D = alocar_reg_dados();
  ler_reg_dados(arq_dados, D);

  // Verificando se as demais condições buscadas são satisfeitas pelo registro analisado.
  int condicoes_iguais = 1;
  for (int j = 1; j < qntd_condicoes; j++) {
    if (comparar_valor_reg_dados(D, vetor_condicoes[j].nome_campo, vetor_condicoes[j].valor_buscado)) {
      condicoes_iguais++;
    }
  }

  // Imprimindo registros que satisfazem às condições de busca.
  if (condicoes_iguais == qntd_condicoes) {
    *registros_achados = 1;
    imprimir_reg_dados(D);
  }

  // Limpando campos de tamanho variável do registro de dados.
  limpar_campos_reg_dados(D);

  // Desalocando registro de dados auxiliar.
  desalocar_reg_dados(&D);
}