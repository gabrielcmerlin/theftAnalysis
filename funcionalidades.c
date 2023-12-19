// Organização de Arquivos (SCC0215) - Trabalho 2 
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funcionalidades especificadas nos trabalhos introdutório, 1 e 2,
  correspondentes aos comandos da linguagem SQL, sendo eles:
    1. CREATE TABLE;
    2. SELECT;
    3. CREATE INDEX;
    4. SELECT (WHERE);
    5. DELETE;
    6. INSERT INTO;
    7. UPDATE;
    8. CREATE INDEX [b*-tree];
    9. SELECT (WHERE) [b*-tree];
    10. INSERT INTO [b*-tree].
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"
#include "arquivos.h"
#include "dados.h"
#include "cabecalho.h"
#include "criterios.h"
#include "utils.h"
#include "indice.h"
#include "busca.h"
#include "ordenacao.h"
#include "b_estrela.h"

// FUNCIONALIDADE 1: Lê registros de dados de um arquivo .csv e escreve-os em um arquivo binário.
void criar_tabela(char *nome_csv, char *nome_dados) {
  int bytes_escritos = 0;  /*Quantidade de bytes escritos no arquivo binário*/
  int nro_reg_arq = 0;     /*Quantidade de registros no arquivo binário*/
  int nro_reg_rem = 0;     /*Quantidade de registros removidos no arquivo binário*/

  // Abrindo arquivo de entrada .csv e pulando primeira linha.
  FILE *arq_csv = abrir_arquivo(nome_csv, "r");
  pular_linha_csv(arq_csv);

  // Abrindo arquivo de saída binário.
  FILE *arq_dados = abrir_arquivo(nome_dados, "wb");

  // Declarando e inicializando um registro de cabeçalho auxiliar.
  reg_cabecalho *cabecalho_aux = alocar_reg_cabecalho();
  atualizar_reg_cabecalho(cabecalho_aux, '0', 0, 0, 0);

  // Escrevendo registro de cabeçalho no arquivo binário e armazenando a quantidade de bytes escritos.
  bytes_escritos += escrever_reg_cabecalho(arq_dados, cabecalho_aux);

  // Escrevendo registros de dados no arquivo binário.
  escrever_arq_dados(arq_csv, arq_dados, &bytes_escritos, &nro_reg_arq, &nro_reg_rem);

  // Atualizando informações do registro de cabeçalho.
  atualizar_reg_cabecalho(cabecalho_aux, '1', bytes_escritos, nro_reg_arq, nro_reg_rem);

  // Escrevendo registro de cabecalho atualizado no arquivo binário.
  fseek(arq_dados, 0, SEEK_SET);
  escrever_reg_cabecalho(arq_dados, cabecalho_aux);

  // Liberando memória alocada.
  desalocar_reg_cabecalho(&cabecalho_aux);

  // Fechando arquivos de entrada e de saída.
  fechar_arquivo(arq_csv);
  fechar_arquivo(arq_dados);

  // Executando a função padrão do trabalho
  binarioNaTela(nome_dados);
}

// FUNCIONALIDADE 2: Imprime de forma organizada os dados de um arquivo binário.
void selecionar(char *nome_dados) {
  // Abrindo arquivo binário.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb");
  verificar_consistencia(arq_dados, NULL);

  // Alocando um registro de cabeçalho auxiliar.
  reg_cabecalho *cabecalho = alocar_reg_cabecalho();

  // Lendo registro de cabeçalho e armazenando o tamanho, em bytes, do cabeçalho.
  int tam_cabecalho = ler_reg_cabecalho(arq_dados, cabecalho);

  // Armazenando a quantidade total de bytes do arquivo binário.
  int total_bytes = retornar_prox_byte_offset(cabecalho);
  
  // Verificando se o arquivo não está vazio. 
  if(!verificar_arq_vazio(total_bytes, tam_cabecalho)){
    // Imprimindo todos os registros de dados do arquivo.
    imprimir_arq_dados(arq_dados, total_bytes, tam_cabecalho);
  }

  // Desalocando registro de cabeçalho.
  desalocar_reg_cabecalho(&cabecalho);

  // Fechando arquivo binário.
  fechar_arquivo(arq_dados);
}

// FUNCIONALIDADE 3: Cria um arquivo de índice ordenado com registros de dados de índice.
void criar_indice(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice) {

  // Abrindo arquivo de dados.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb");
  verificar_consistencia(arq_dados, NULL);

  // Alocando registros auxiliares.
  reg_cabecalho *C = alocar_reg_cabecalho();
  reg_dados *D = alocar_reg_dados();

  // Alocando vetor de registro de dados de índice.
  void **vetor = NULL;
  
  // Declarando e inicializando variáveis.
  int qntd_reg_indice = 0;                             /*Quantidade de registros no arquivo de índice*/
  int tam_cabecalho = ler_reg_cabecalho(arq_dados, C); /*Tamanho do cabeçalho do arquivo de dados em bytes*/
  int total_bytes = retornar_prox_byte_offset(C);      /*Tamanho do arquivo de dados em bytes*/
  int bytes_reg_dados_atual;                           /*Tamanho do registro lido em cada iteração do laço*/
  
  // Percorrendo o arquivo de dados a partir do registro 0.
  for(int bytes_percorridos = tam_cabecalho; bytes_percorridos < total_bytes; bytes_percorridos += bytes_reg_dados_atual) {        
    // Lendo o registro de dados e armazenando seu tamanho em bytes.
    bytes_reg_dados_atual = ler_reg_dados(arq_dados, D);

    // Verificando se o registro é valido (não removido e com chave não nula). Então, preenchendo o vetor.
    if(retornar_removido(D) == '0'){
      if (strcmp(tipo_dado, "string") == 0){
        char *chave_busca = retornar_campo_string(D, campo_indexado);
        if(strcmp(chave_busca, "NULO\0") != 0){
          vetor = (void**) preencher_vetor_string(vetor, &qntd_reg_indice, chave_busca, bytes_percorridos);
        }
      }
      else{
        int chave_busca = retornar_campo_int(D, campo_indexado);
        if(chave_busca != -1){
          vetor = (void**) preencher_vetor_int(vetor, &qntd_reg_indice, chave_busca, bytes_percorridos);
        }
      }
    }   

    // Limpando campos de tamanho variável do registro auxiliar.
    limpar_campos_reg_dados(D);       
  }

  // Fechando arquivo de dados.
  fechar_arquivo(arq_dados);

  // Desalocando registro de cabeçalho e de dados auxiliares.
  desalocar_reg_cabecalho(&C);
  desalocar_reg_dados(&D);

  // Ordenando vetor.
  quick_sort(vetor, 0, qntd_reg_indice - 1, tipo_dado);

  // Criando arquivo de índices e escrevendo o vetor.
  criar_arquivo_indice(vetor, qntd_reg_indice, nome_indice, tipo_dado);
  
  // Desalocando vetor de registros de dados de índice.
  desalocar_vetor_indice(vetor, qntd_reg_indice, tipo_dado);

  // Função padrão do trabalho.
  binarioNaTela(nome_indice);
}

// FUNCIONALIDADE 4: Imprime os registros de um arquivo de dados seguindo um ou mais critérios de busca.
void selecionar_com_condicoes(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas) {
  // Abrindo arquivos para leitura.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb");
  verificar_consistencia(arq_dados, NULL);
  
  FILE *arq_indice = abrir_arquivo(nome_indice, "rb");
  verificar_consistencia(arq_indice, arq_dados);

  // Declarando variáveis auxiliares.
  char nome_campo_condicao[TAM_STRING], valor_campo_condicao_string[TAM_STRING];
  int qntd_condicoes, tam_vetor_indice, registros_achados;

  // Inicializando vetor de índices.
  void **vetor_indice = carregar_arq_indice(arq_indice, &tam_vetor_indice, tipo_dado);

  // Lendo cada linha de busca.
  for (int i = 0; i < qntd_buscas; i++) {
    scanf(" %d", &qntd_condicoes);
    printf("Resposta para a busca %d\n", i + 1);
  
    registros_achados = 0;

    // Lendo todas as condições e guardando-as num vetor.
    criterio *vetor_condicoes = criar_vetor_criterios(nome_campo_condicao, valor_campo_condicao_string, qntd_condicoes, campo_indexado);

    // Verificando se há uma condição dada correspondente ao campo indexado. 
    // Se sim, realiza busca binária. Se não, realiza busca sequencial.
    if (strcmp(vetor_condicoes[0].nome_campo, campo_indexado) == 0) { 
      
      // Realizando busca binária e imprimindo os registros encontrados.
      buscar_indexado(arq_dados, vetor_indice, vetor_condicoes, tam_vetor_indice, tipo_dado, nome_campo_condicao, &registros_achados, qntd_condicoes,
                      NULL, 1);

      // Se nenhum registro foi encontrado, imprimindo mensagem padrão.
      if (registros_achados == 0) {
        printar_erro(5);
      }
    } else { 

      // Realizando busca sequencial e imprimindo os registros encontrados.
      buscar_nao_indexado(arq_dados, vetor_condicoes, &registros_achados, qntd_condicoes, -1, NULL, NULL, NULL, 1);

      // Se nenhum registro foi encontrado, imprimindo mensagem padrão.
      if (registros_achados == 0) {
        printar_erro(5);
      }
    }

    // Desalocando vetor de condições.
    desalocar_vetor_criterios(vetor_condicoes, qntd_condicoes);
  }

  // Fechando arquivos.
  fechar_arquivo(arq_dados);
  fechar_arquivo(arq_indice);

  // Desalocando vetor de índices.
  desalocar_vetor_indice(vetor_indice, tam_vetor_indice, tipo_dado);
}

// FUNCIONALIDADE 5: Deleta os registros de um arquivo de dados seguindo um ou mais critérios de busca.
void deletar(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas) {
  // Abrindo arquivo de dados para leitura e escrita.
  FILE *arq_dados = abrir_arquivo(nome_dados, "r+b");
  verificar_consistencia(arq_dados, NULL);

  // Abrindo arquivo de índice para leitura.
  FILE *arq_indice = abrir_arquivo(nome_indice, "rb");
  verificar_consistencia(arq_indice, arq_dados);

  // Marcando arquivo de dados como inconsistente.
  marcar_como_inconsistente(arq_dados);

  // Declarando variáveis auxiliares.
  char nome_campo_condicao[TAM_STRING], valor_campo_condicao_string[TAM_STRING];
  int qntd_reg_rem = 0, tam_vetor_indice, qntd_condicoes;

  // Declarando e inicializando vetor de índices.
  void **vetor_indice = carregar_arq_indice(arq_indice, &tam_vetor_indice, tipo_dado);

  // Lendo cada linha de busca.
  for (int i = 0; i < qntd_buscas; i++) {
    scanf(" %d", &qntd_condicoes);
  
    // Lendo todas as condições e guardando-as num vetor.
    criterio *vetor_condicoes = criar_vetor_criterios(nome_campo_condicao, valor_campo_condicao_string, qntd_condicoes, campo_indexado);

    // Verificando se há uma condição dada correspondente ao campo indexado. 
    // Se sim, realiza busca binária. Se não, realiza busca sequencial.
    if (strcmp(vetor_condicoes[0].nome_campo, campo_indexado) == 0) { 
      // Realizando busca binária e deletando, logicamente, os registros encontrados.      
      buscar_indexado(arq_dados, vetor_indice, vetor_condicoes, tam_vetor_indice, tipo_dado, nome_campo_condicao, &qntd_reg_rem, qntd_condicoes, campo_indexado, 2);
    
    } else { 
      // Realizando busca sequencial e deletando, logicamente, os registros encontrados.
      buscar_nao_indexado(arq_dados, vetor_condicoes, &qntd_reg_rem, qntd_condicoes, tam_vetor_indice, tipo_dado, campo_indexado, vetor_indice, 2);
    } 

    // Desalocando vetor de condições.
    desalocar_vetor_criterios(vetor_condicoes, qntd_condicoes);
  }

  // Fechando arquivo.
  fechar_arquivo(arq_indice);

  // Realocando memória do vetor de índice por meio da verificação dos registros logicamente removidos.
  vetor_indice = (void**) shift_vetor_indice(vetor_indice, tam_vetor_indice, qntd_reg_rem, tipo_dado);

  // Atualizando arquivo de indíce após a deleção.
  criar_arquivo_indice(vetor_indice, tam_vetor_indice - qntd_reg_rem, nome_indice, tipo_dado);

  // Desalocando vetor de índices.
  desalocar_vetor_indice(vetor_indice, tam_vetor_indice - qntd_reg_rem, tipo_dado);

  // Alocando registro de cabeçalho auxiliar.
  reg_cabecalho *C = alocar_reg_cabecalho();

  // Marcando arquivo de índice como consistente.
  fseek(arq_dados, 0, SEEK_SET);
  ler_reg_cabecalho(arq_dados, C);
  atualizar_reg_cabecalho(C, '1', retornar_prox_byte_offset(C), retornar_nro_reg_arq(C), retornar_nro_reg_rem(C) + qntd_reg_rem);
  fseek(arq_dados, 0, SEEK_SET);
  escrever_reg_cabecalho(arq_dados, C);

  // Desalocando registro auxiliar.
  desalocar_reg_cabecalho(&C);

  // Fechando arquivo.
  fechar_arquivo(arq_dados);

  // Função padrão do trabalho.
  binarioNaTela(nome_dados);
  binarioNaTela(nome_indice);
}

// FUNCIONALIDADE 6: Insere registros no fim do arquivo de dados e, se necessário, no arquivo de índices.
void inserir(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_insercoes) {
  // Abrindo arquivo de dados para leitura e escrita.
  FILE *arq_dados = abrir_arquivo(nome_dados, "r+b");
  verificar_consistencia(arq_dados, NULL);
  
  // Abrindo arquivo de índice para leitura.
  FILE *arq_indice = abrir_arquivo(nome_indice, "rb");
  verificar_consistencia(arq_indice, arq_dados);
  
  // Marcando arquivo de dados como inconsistente.
  marcar_como_inconsistente(arq_dados);

  // Alocando registro de cabeçalho auxiliar.
  reg_cabecalho *C_aux = alocar_reg_cabecalho();
  
  // Lendo registro de cabeçalho.
  ler_reg_cabecalho(arq_dados, C_aux);

  int qntd_reg = 0;  /*Quantidade de registros no arquivo*/

  // Declarando e inicializando vetor de índices.
  void **vetor_indice = carregar_arq_indice(arq_indice, &qntd_reg, tipo_dado);
 
  // Fechando arquivo de índices.
  fechar_arquivo(arq_indice);

  // Posicionando o cursor do arquivo de dados no local de inserção.
  long int prox_byte_offset = retornar_prox_byte_offset(C_aux);
  fseek(arq_dados, prox_byte_offset, SEEK_SET);
  
  // Lendo e inserindo registros no arquivo de dados e no vetor de índices.
  vetor_indice = realizar_insercao(arq_dados, vetor_indice, &qntd_reg, &prox_byte_offset, qntd_insercoes, tipo_dado, campo_indexado);

  // Ordenando vetor de índices na memória. 
  quick_sort(vetor_indice, 0, qntd_reg - 1, tipo_dado);

  // Atualizando cabeçalho do arquivo de dados (quantidade de registros e staus)
  fseek(arq_dados, 0, SEEK_SET);
  atualizar_reg_cabecalho(C_aux, '1', prox_byte_offset, retornar_nro_reg_arq(C_aux) + qntd_insercoes, retornar_nro_reg_rem(C_aux));
  escrever_reg_cabecalho(arq_dados, C_aux);

  // Fechando arquivo de dados.
  fechar_arquivo(arq_dados);

  // Desalocando registro de cabeçalho auxiliar.
  desalocar_reg_cabecalho(&C_aux);

  // Abrindo arquivo de índices para escrita.
  arq_indice = abrir_arquivo(nome_indice, "wb");

  // Inicializando cabeçalho do arquivo de índices.
  indice_cabecalho *C_indice = alocar_indice_cabecalho();
  salvar_status_indice(C_indice, '0');
  salvar_qntd_reg_indice(C_indice, qntd_reg);

  // Escrevendo arquivo de índices.
  escrever_indice_cabecalho(arq_indice, C_indice);
  escrever_indice_dados(arq_indice, vetor_indice, qntd_reg, tipo_dado);

  // Marcando arquivo de índices como consistente.
  salvar_status_indice(C_indice, '1');
  fseek(arq_indice, SEEK_SET, 0);
  escrever_indice_cabecalho(arq_indice, C_indice);

  // Fechando arquivo de índices.
  fechar_arquivo(arq_indice);

  // Desalocando registro auxiliar.
  desalocar_indice_cabecalho(&C_indice);
  
  // Desalocando vetor de índices.
  desalocar_vetor_indice(vetor_indice, qntd_reg, tipo_dado);
  
  // Função padrão do trabalho.
  binarioNaTela(nome_dados);
  binarioNaTela(nome_indice);
}

// FUNCIONALIDADE 7: Atualiza os campos dos registros de um arquivo seguindo um ou mais critério de busca.
void atualizar(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas) {
  // Abrindo arquivo de dados para leitura e escrita.
  FILE *arq_dados = abrir_arquivo(nome_dados, "r+b");
  verificar_consistencia(arq_dados, NULL);

  // Abrindo arquivo de índices para leitura.
  FILE *arq_indice = abrir_arquivo(nome_indice, "rb");
  verificar_consistencia(arq_indice, arq_dados);

  // Marcando arquivo de dados como inconsistente.
  marcar_como_inconsistente(arq_dados);  

  // Declarando variáveis auxiliares.
  char nome_campo_condicao[TAM_STRING], valor_campo_condicao_string[TAM_STRING];
  int tam_vetor_indice, qntd_condicoes, qntd_campos_atualizados, qntd_reg_rem = 0;

  // Declarando e inicializando vetor de índices.
  void **vetor_indice = carregar_arq_indice(arq_indice, &tam_vetor_indice, tipo_dado);

  // Alocando e lendo registro de cabeçalho do arquivo de dados.
  reg_cabecalho *C = alocar_reg_cabecalho();
  int tam_cabecalho = ler_reg_cabecalho(arq_dados, C);

  // Lendo cada linha de busca.
  for (int i = 0; i < qntd_buscas; i++) {
    scanf(" %d", &qntd_condicoes);
  
    // Lendo todas as condições e guardando-as num vetor.
    criterio *vetor_condicoes = criar_vetor_criterios(nome_campo_condicao, valor_campo_condicao_string, qntd_condicoes, campo_indexado);
    
    scanf(" %d", &qntd_campos_atualizados);
    // Lendo todos os campos a serem atualizados e guardando-os num vetor.
    criterio *vetor_atualizacoes = criar_vetor_criterios(nome_campo_condicao, valor_campo_condicao_string, qntd_campos_atualizados, campo_indexado);

    // Verificando se há uma condição dada correspondente ao campo indexado. 
    // Se sim, realiza busca binária. Se não, realiza busca sequencial.
    if (strcmp(vetor_condicoes[0].nome_campo, campo_indexado) == 0) { //fazer busca binária
      // Realizando busca binária e atualizando os registros encontrados.   
      vetor_indice = buscar_indexado_atualizar(arq_dados, vetor_indice, vetor_condicoes, &tam_vetor_indice, tipo_dado, nome_campo_condicao, 
                                qntd_condicoes, campo_indexado, vetor_atualizacoes, qntd_campos_atualizados, C);

    } else {
      // Realizando busca sequencial e atualizando os registros encontrados.
      vetor_indice = buscar_nao_indexado_atualizar(arq_dados, vetor_indice, vetor_condicoes, qntd_condicoes, &tam_vetor_indice, tipo_dado, 
                                   campo_indexado, vetor_atualizacoes, qntd_campos_atualizados, C, tam_cabecalho);
    }
    
    // Desalocando vetor de condições e vetor de atualizações.
    desalocar_vetor_criterios(vetor_condicoes, qntd_condicoes);
    desalocar_vetor_criterios(vetor_atualizacoes, qntd_campos_atualizados);
  }

  // Fechando arquivo de índice.
  fechar_arquivo(arq_indice);

  // Ordenando vetor de índices na memória. 
  quick_sort(vetor_indice, 0, tam_vetor_indice - 1, tipo_dado);

  // Atualizando arquivo de indíce após a atualização.
  criar_arquivo_indice(vetor_indice, tam_vetor_indice, nome_indice, tipo_dado);

  // Desalocando vetor de índices.
  desalocar_vetor_indice(vetor_indice, tam_vetor_indice - qntd_reg_rem, tipo_dado);

  // Marcando arquivo de dados como consistente.
  fseek(arq_dados, 0, SEEK_SET);
  salvar_status(C, '1');
  escrever_reg_cabecalho(arq_dados, C);

  // Desalocando registro auxiliar.
  desalocar_reg_cabecalho(&C);

  // Fechando arquivo.
  fechar_arquivo(arq_dados);

  // Função padrão do trabalho.
  binarioNaTela(nome_dados);
  binarioNaTela(nome_indice);
}

// FUNCIONALIDADE 8: Cria um arquivo de índice para um arquivos de dados, utilizando árvore-b*.
void criar_indice_arv_b(char *nome_dados, char *nome_arv_b){
  // Abrindo arquivo de dados.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb");
  verificar_consistencia(arq_dados, NULL);

  // Criando arquivo de árvore-b*.
  FILE *arq_arv = abrir_arquivo(nome_arv_b, "wb+");
  marcar_como_inconsistente(arq_arv);

  // Alocando e inicializando cabeçalho da árvore-b*.
  arv_cabecalho *C_arv = alocar_arv_cabecalho();
  inicializar_arv_cabecalho(C_arv);

  // Escrevendo cabeçalho do arquivo de árvore-b*.
  escrever_arv_cabecalho(arq_arv, C_arv);

  // Alocando registros auxiliares.
  reg_cabecalho *C = alocar_reg_cabecalho();
  reg_dados *D = alocar_reg_dados();

  // Declarando e inicializando variáveis auxiliares.
  int tam_cabecalho = ler_reg_cabecalho(arq_dados, C); /* Tamanho do cabeçalho do arquivo de dados em bytes */
  int total_bytes = retornar_prox_byte_offset(C);      /* Tamanho do arquivo de dados em bytes */
  int bytes_reg_dados_atual;                           /* Tamanho do registro lido em cada iteração do laço */
  
  int i = 0;
  // Percorrendo o arquivo de dados a partir do registro 0.
  for(int bytes_percorridos = tam_cabecalho; bytes_percorridos < total_bytes; bytes_percorridos += bytes_reg_dados_atual) {    

    // Lendo o registro de dados e armazenando seu tamanho em bytes.
    bytes_reg_dados_atual = ler_reg_dados(arq_dados, D);

    // Verificando se o registro é valido (não removido). Então, inserindo na árvore-b*.
    if(retornar_removido(D) == '0'){
      inserir_b_estrela(arq_arv, C_arv, D, bytes_percorridos);
      i++;
    }   

    // Limpando campos de tamanho variável do registro auxiliar.
    limpar_campos_reg_dados(D);     
  }

  // Fechando arquivo de dados.
  fechar_arquivo(arq_dados);

  // Desalocando registro de cabeçalho e de dados auxiliares.
  desalocar_reg_cabecalho(&C);
  desalocar_reg_dados(&D);

  // Marcando arquivo de índice como consistente e atualizando cabeçalho.
  C_arv->status = '1';
  fseek(arq_arv, 0, SEEK_SET);
  escrever_arv_cabecalho(arq_arv, C_arv);
  desalocar_arv_cabecalho(&C_arv);

  // Fechando arquivo de árvore-b*.
  fechar_arquivo(arq_arv);

  // Função padrão do trabalho.
  binarioNaTela(nome_arv_b);
}

// FUNCIONALIDADE 9: Imprime os registros de um arquivo de dados seguindo um ou mais critérios de busca, 
//                   utilizando árvore-b* para buscas cujo critério é um campo indexado.
void selecionar_arv_b(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_arv_b, int qntd_buscas){
  // Abrindo arquivos para leitura.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb");
  verificar_consistencia(arq_dados, NULL);
  
  FILE *arq_arv = abrir_arquivo(nome_arv_b, "rb");
  verificar_consistencia(arq_arv, arq_dados);

  // Declarando variáveis auxiliares.
  char nome_campo_condicao[TAM_STRING], valor_campo_condicao_string[TAM_STRING];
  int qntd_condicoes, registros_encontrados;

  // Lendo registro de cabeçalho da árvore-b*.
  arv_cabecalho *C_arv = alocar_arv_cabecalho();
  ler_arv_cabecalho(arq_arv, C_arv);

  // Lendo cada linha de busca.
  for (int i = 0; i < qntd_buscas; i++) {
    scanf(" %d", &qntd_condicoes);
    printf("Resposta para a busca %d\n", i + 1);
  
    registros_encontrados = 0;

    // Lendo todas as condições e guardando-as num vetor.
    criterio *vetor_condicoes = criar_vetor_criterios(nome_campo_condicao, valor_campo_condicao_string, qntd_condicoes, campo_indexado);

    // Verificando se há uma condição dada correspondente ao campo indexado. 
    // Se sim, realizando busca na árvore-b*. Se não, realizando busca sequencial no arquivo de dados.
    if (strcmp(vetor_condicoes[0].nome_campo, campo_indexado) == 0) { 
      buscar_b_estrela(arq_dados, arq_arv, vetor_condicoes, qntd_condicoes, &registros_encontrados, C_arv);
    } else { 
      // Realizando busca sequencial e imprimindo os registros encontrados.
      buscar_nao_indexado(arq_dados, vetor_condicoes, &registros_encontrados, qntd_condicoes, -1, NULL, NULL, NULL, 1);
    }

    // Se nenhum registro foi encontrado, imprimindo mensagem padrão.
    if (registros_encontrados == 0) {
      printar_erro(5);
    }

    // Desalocando vetor de condições.
    desalocar_vetor_criterios(vetor_condicoes, qntd_condicoes);
  }

  // Desalocando registro de cabeçalho da árvore-b*.
  desalocar_arv_cabecalho(&C_arv);

  // Fechando arquivos.
  fechar_arquivo(arq_dados);
  fechar_arquivo(arq_arv);
}

// FUNCIONALIDADE 10: Insere um ou mais registros de dados em um arquivo de índice de árvore-b*, lidos do usuário.
void inserir_arv_b(char *nome_dados, char *nome_arv_b, int qntd_insercoes) {
  // Abrindo arquivo de dados.
  FILE *arq_dados = abrir_arquivo(nome_dados, "rb+");
  verificar_consistencia(arq_dados, NULL);
  marcar_como_inconsistente(arq_dados);

  // Abrindo arquivo de árvore-b*.
  FILE *arq_arv = abrir_arquivo(nome_arv_b, "rb+");
  verificar_consistencia(arq_arv, arq_dados);
  marcar_como_inconsistente(arq_arv);

  // Alocando e lendo cabeçalho da árvore-b*.
  arv_cabecalho *C_arv = alocar_arv_cabecalho();
  ler_arv_cabecalho(arq_arv, C_arv);

  // Alocando e lendo cabeçalho do arquivo de dados.
  reg_cabecalho *C = alocar_reg_cabecalho();
  ler_reg_cabecalho(arq_dados, C);

  // Armazenando próximo byte offset do arquivo de dados.
  long int byte_offset = retornar_prox_byte_offset(C);
  fseek(arq_dados, byte_offset, SEEK_SET);
  int bytes_escritos, nro_reg_arq = retornar_nro_reg_arq(C);

  // Alocando registro auxiliar.
  reg_dados *D = alocar_reg_dados();

  // Realizando inserções no arquivo de índice.
  for(int i = 0; i < qntd_insercoes; i++) {    

    // Lendo o registro de dados e armazenando seu tamanho em bytes.
    ler_para_inserir(D);

    // Verificando se o registro é valido (não removido). Então, inserindo na árvore-b*.
    if(retornar_removido(D) == '0'){
      // Escrevendo registro no arquivo de dados.
      bytes_escritos = escrever_reg_dados(arq_dados, &D);
      nro_reg_arq++;

      // Indexando registro na árvore-b*.
      inserir_b_estrela(arq_arv, C_arv, D, byte_offset);

      // Reatribuindo próximo byte offset do arquivo de dados.
      byte_offset += bytes_escritos;
    }

    // Limpando campos de tamanho variável do registro auxiliar.
    limpar_campos_reg_dados(D);     
  }

  // Marcando arquivo de dados como consistente e atualizando cabeçalho.
  fseek(arq_dados, 0, SEEK_SET);
  salvar_status(C, '1');
  salvar_prox_byte_offset(C, byte_offset);
  salvar_nro_reg_arq(C, nro_reg_arq);
  escrever_reg_cabecalho(arq_dados, C);

  // Fechando arquivo de dados.
  fechar_arquivo(arq_dados);

  // Desalocando registro de cabeçalho e de dados auxiliares.
  desalocar_reg_cabecalho(&C);
  desalocar_reg_dados(&D);

  // Marcando arquivo de índice como consistente e atualizando cabeçalho.
  C_arv->status = '1';
  fseek(arq_arv, 0, SEEK_SET);
  escrever_arv_cabecalho(arq_arv, C_arv);
  desalocar_arv_cabecalho(&C_arv);

  // Fechando arquivo de árvore-b*.
  fechar_arquivo(arq_arv);

  // Função padrão do trabalho.
  binarioNaTela(nome_dados);
  binarioNaTela(nome_arv_b);
}