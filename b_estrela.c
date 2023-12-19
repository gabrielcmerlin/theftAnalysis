// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funções relacionadas à árvore-B*, como alocação, desalocação, busca, 
  leitura, escrita e inserção.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "b_estrela.h"

// Define um grupo de páginas acompanhadas de seu RRN. Implementado para melhorar a legibilidade do código.
typedef struct _grupo_paginas grupo_paginas;
struct _grupo_paginas {
    pagina *pag_esq;
    int rrn_esq;
    pagina *pag_dir;
    int rrn_dir;
    pagina *pag_pai;
    int rrn_pai;
} ;

// Aloca e preenche um grupo de páginas com os ponteiros adequados e com os valores de RRNs.
grupo_paginas *criar_grupo_paginas(pagina *pag_esq, int rrn_esq, pagina *pag_dir, int rrn_dir, pagina *pag_pai, int rrn_pai) {
    grupo_paginas *grupo = (grupo_paginas *) malloc(1 * sizeof(grupo_paginas));

    grupo->pag_esq = pag_esq;
    grupo->rrn_esq = rrn_esq;
    grupo->pag_dir = pag_dir;
    grupo->rrn_dir = rrn_dir;
    grupo->pag_pai = pag_pai;
    grupo->rrn_pai = rrn_pai;

    return grupo;
}

// Aloca registro de cabeçalho da árvore-b*.
arv_cabecalho *alocar_arv_cabecalho() {
    arv_cabecalho *C_arv = (arv_cabecalho *) malloc(1 * sizeof(arv_cabecalho));
    assert(C_arv != NULL);

    // Inicializando o cabeçalho.
    inicializar_arv_cabecalho(C_arv);

    return C_arv;
}

// Desaloca registro de cabeçalho da árvore-b*.
void desalocar_arv_cabecalho(arv_cabecalho **C_arv) {
    if(C_arv != NULL && *C_arv != NULL) {
        free(*C_arv);
        *C_arv = NULL;
    }
}

// Salva um par (chave de busca e byteoffset) de uma chave.
void salvar_chave(chave *chave, int chave_busca, int byte_offset) {
    chave->chave_busca = chave_busca;
    chave->byte_offset = byte_offset;
}

// Aloca página de disco da árvore-b*.
pagina *alocar_pagina() {
    pagina *pag = (pagina *) malloc(1 * sizeof(pagina));
    assert(pag != NULL);

    return pag;
}

// Desaloca página de disco da árvore-b*.
void desalocar_pagina(pagina **pag){
    if(pag != NULL && *pag != NULL) {        
        free(*pag);
        *pag = NULL;
    }
}

// Desaloca várias páginas de disco da árvore-B*. Implementado para melhorar a legibilidade do código.
void desalocar_varias_paginas(pagina *pag1, pagina *pag2, pagina *pag3) {
    if (pag1 != NULL) desalocar_pagina(&pag1);
    if (pag2 != NULL) desalocar_pagina(&pag2);
    if (pag3 != NULL) desalocar_pagina(&pag3);
}

// Inicizaliza os vetores de uma página de disco.
void inicializar_vetores_pagina(pagina *pag) {
    for (int i = 0; i < ORDEM; i++) {
        pag->ponteiros[i] = -1;

        if (i < ORDEM - 1) salvar_chave(&pag->chaves[i], -1, -1);
    }
}

// Salva os campos de uma página da árvore-b*, utilizando vetores auxiliares de chaves e de ponteiros.
void atualizar_pagina(pagina *pag, int nivel, int qntd_chaves, chave *vetor_chaves, int *vetor_ponteiros, int ini, int fim) {
    pag->nivel = nivel;
    pag->qntd_chaves = qntd_chaves;
    inicializar_vetores_pagina(pag);

    int i, j;
    for (i = 0, j = ini; j < fim; i++, j++) {
        pag->chaves[i] = vetor_chaves[j];
        pag->ponteiros[i] = vetor_ponteiros[j];
    }
    pag->ponteiros[i] = vetor_ponteiros[j];
}

// Lê, campo a campo, o registro de cabeçalho da árvore-b* a partir de um arquivo.
void ler_arv_cabecalho(FILE *arq_arv, arv_cabecalho *C_arv) {
    // Lendo campos.
    char c;
    fread(&c, sizeof(char), 1, arq_arv);
    C_arv->status = c;
    int aux;
    fread(&aux, sizeof(int), 1, arq_arv);
    C_arv->no_raiz = aux;
    fread(&aux, sizeof(int), 1, arq_arv);
    C_arv->prox_rrn = aux;
    fread(&aux, sizeof(int), 1, arq_arv);
    C_arv->nro_niveis = aux;
    fread(&aux, sizeof(int), 1, arq_arv);
    C_arv->nro_chaves = aux;

    // Preenchendo com lixo.
    for (int i = 0; i < 59; i++) {
        fread(&c, sizeof(char), 1, arq_arv);
        C_arv->lixo[i] = c;
    }
}

// Escreve, campo a campo, o registro de cabeçalho da árvore-b* num arquivo.
void escrever_arv_cabecalho(FILE *arq_arv, arv_cabecalho *C_arv) {
    // Escrevendo campos.
    fwrite(&(C_arv->status), sizeof(char), 1, arq_arv);
    fwrite(&(C_arv->no_raiz), sizeof(int), 1, arq_arv);
    fwrite(&(C_arv->prox_rrn), sizeof(int), 1, arq_arv);
    fwrite(&(C_arv->nro_niveis), sizeof(int), 1, arq_arv);
    fwrite(&(C_arv->nro_chaves), sizeof(int), 1, arq_arv);
    // Preenchendo com lixo.
    fwrite(&C_arv->lixo, sizeof(char), 59, arq_arv);
}

// Inicializa os campos do registro de cabeçalho da árvore-b*.
void inicializar_arv_cabecalho(arv_cabecalho *C_arv){
  C_arv->status = 0;
  C_arv->no_raiz = -1;
  C_arv->prox_rrn = 0;
  C_arv->nro_niveis = 0;
  C_arv->nro_chaves = 0;
  for (int i = 0; i < 59; i++) {
    C_arv->lixo[i] = '$';
  }
}

// Atualiza o cabeçalho de uma árvore-b*.
void atualizar_arv_cabecalho(arv_cabecalho *C_arv, int no_raiz, int prox_rrn, int nro_niveis, int nro_chaves) {
    C_arv->no_raiz = no_raiz;
    C_arv->prox_rrn = prox_rrn;
    C_arv->nro_niveis = nro_niveis;
    C_arv->nro_chaves = nro_chaves;
}

// Lê o campo 'nivel' de uma página de disco a partir de um arquivo.
void ler_nivel(FILE *arq_arv, pagina *pag) {
    int nivel;
    fread(&nivel, sizeof(int), 1, arq_arv);
    pag->nivel = nivel;
}

// Lê o campo 'qntd_chaves' de uma página de disco a partir de um arquivo.
void ler_qntd_chaves(FILE *arq_arv, pagina *pag) {
    int n;
    fread(&n, sizeof(int), 1, arq_arv);
    pag->qntd_chaves = n;
}

// Lê o campo 'ponteiro' de uma página de disco a partir de um arquivo.
void ler_ponteiro(FILE *arq_arv, pagina *pag, int posicao) {
    int ponteiro;
    fread(&ponteiro, sizeof(int), 1, arq_arv);
    pag->ponteiros[posicao] = ponteiro;
}

// Lê o par chave (chave de busca e byte offset do arquivo de dados) de uma página de disco a partir de um arquivo.
void ler_chave(FILE *arq_arv, pagina *pag, int posicao) {
    int chave_busca;
    long int byte_offset;
    fread(&chave_busca, sizeof(int), 1, arq_arv);
    fread(&byte_offset, sizeof(long int), 1, arq_arv);
    pag->chaves[posicao].chave_busca = chave_busca;
    pag->chaves[posicao].byte_offset = byte_offset;
}

// Lê, campo a campo, uma página de disco a partir de um arquivo.
void ler_pagina(FILE *arq_arv, pagina *pag) {
    ler_nivel(arq_arv, pag);
    ler_qntd_chaves(arq_arv, pag);
    for (int i = 0; i < (ORDEM - 1); i++) {
        ler_ponteiro(arq_arv, pag, i);
        ler_chave(arq_arv, pag, i);
    }
    ler_ponteiro(arq_arv, pag, ORDEM - 1);
}

// Busca, recursivamente, a chave buscada em uma página. 
int buscar_posicao_rec(chave* vetor_chaves, int ini, int fim, int chave_buscada) {
  // Caso base da recursão.
  if (ini >= fim) return fim; 

  // Calculando índice i da busca binária e encontrando chave a ser comparada.
  int i = (ini + fim) / 2;
  int chave = vetor_chaves[i].chave_busca;

  if (chave == chave_buscada) {
    return i;
  } else if (chave < chave_buscada) {
    return buscar_posicao_rec(vetor_chaves, i + 1, fim, chave_buscada);
  } else {
    return buscar_posicao_rec(vetor_chaves, ini, i - 1, chave_buscada);
  }
}

// Realizada a chamada da etapa recursiva da busca binária em um vetor de chaves.
int buscar_posicao_vetor_chaves(chave *vetor_chaves, int tam, int chave_buscada) {
    int pos = buscar_posicao_rec(vetor_chaves, 0, tam - 1, chave_buscada);

    // Adequando posição de inserção porque a condição de parada da busca binária não é suficiente para 
    // retornar a posição teoricamente correta de uma chave que não existe no vetor. 
    if (vetor_chaves[pos].chave_busca < chave_buscada || pos == -1) pos++; 

    return pos;
}

// Realiza a chamada da etapa recursiva da busca binária em uma página.
int buscar_posicao_pagina(pagina *pag, int chave_buscada){
    return buscar_posicao_rec(pag->chaves, 0, pag->qntd_chaves - 1, chave_buscada);
}

// Encontra, recursivamente, o byte offset de uma chave buscada em uma árvore-b*. Retorna -1, caso não encontre.
long int encontrar_byte_offset_rec(FILE *arq_arv, int rrn, pagina *pag, int chave_buscada) {
    // Verificando se o nó é folha, não tem filhos condizentes com a chave buscada ou se a árvore é vazia.
    if (rrn == -1) return -1;

    // Lendo página do passo atual da recursão.
    fseek(arq_arv, (rrn * TAM_PAGINA) + TAM_PAGINA, SEEK_SET);
    ler_pagina(arq_arv, pag);
    
    // Encontrando a posição da chave buscada por meio de uma busca binária na página analisada.
    // Aqui, será retornada a posição da chave analisada; 
    // OU a posição do elemento imediatamente menor;
    // OU a posição do elemento imediatamente maior do que a chave buscada.
    int pos_buscada = buscar_posicao_pagina(pag, chave_buscada);

    // Verificando se encontramos a chave buscada. Para isso, a posição calculada deve estar dentro da página analisada.
    if (pos_buscada < (ORDEM - 1) && pag->chaves[pos_buscada].chave_busca == chave_buscada) {
        return pag->chaves[pos_buscada].byte_offset;
    } 
    // Descendo para direita na árvore.
    else if (pag->chaves[pos_buscada].chave_busca < chave_buscada){ 
        return encontrar_byte_offset_rec(arq_arv, pag->ponteiros[pos_buscada + 1], pag, chave_buscada);
    }
    // Descendo para esquerda na árvore.
    else{
        return encontrar_byte_offset_rec(arq_arv, pag->ponteiros[pos_buscada], pag, chave_buscada);
    }   
}

// Retorna o byte offset de uma chave buscada em uma árvore-b*. Retorna -1, caso não encontre.
long int encontrar_byte_offset(FILE *arq_arv, int rrn_raiz, int chave_buscada) {
    // Alocando memória.
    pagina *pag = alocar_pagina();

    // Encontrando byte offset da chave buscada.
    int byte_offset = encontrar_byte_offset_rec(arq_arv, rrn_raiz, pag, chave_buscada);

    // Liberando memória.
    desalocar_pagina(&pag);

    return byte_offset;
}

// Escreve, campo a campo, uma página de disco num arquivo.
void escrever_pagina(FILE *arq_arv, pagina *pag, int rrn) {
    fseek(arq_arv, (rrn + 1) * TAM_PAGINA, SEEK_SET);

    fwrite(&pag->nivel, sizeof(int), 1, arq_arv);
    fwrite(&pag->qntd_chaves, sizeof(int), 1, arq_arv);
    for (int i = 0; i < ORDEM; i++) {
        fwrite(&pag->ponteiros[i], sizeof(int), 1, arq_arv);
        if (i < ORDEM - 1) {
            fwrite(&pag->chaves[i].chave_busca, sizeof(int), 1, arq_arv);
            fwrite(&pag->chaves[i].byte_offset, sizeof(long int), 1, arq_arv);
        }
    }
}

// Cria os vetores auxiliares de chaves e de ponteiros para realizar redistribuição e splits.
void criar_vetores_auxiliares(chave *vetor_chaves, int *vetor_ponteiros, grupo_paginas *grupo, int pos_pai) {
    
    // Armazenando a quantidade de chaves de cada página.
    // Em caso de split 1 para 2, não há página à direita, logo, a quantidade de chaves é 0.
    // Em casos de split 2 para 3 e redistribuição, a quantidade de chaves deve ser acessada via página.
    int n_esq = grupo->pag_esq->qntd_chaves;
    int n_dir = 0;
    if (grupo->pag_dir != NULL) {
        n_dir = grupo->pag_dir->qntd_chaves;
    }

    // Preenchendo vetores com as chaves e com os ponteiros da página à esquerda.
    int i;
    for (i = 0; i < n_esq; i++) {
        vetor_chaves[i] = grupo->pag_esq->chaves[i];
        vetor_ponteiros[i] = grupo->pag_esq->ponteiros[i];
    }
    vetor_ponteiros[i] = grupo->pag_esq->ponteiros[i];

    // Preenchendo vetores com as chaves e com os ponteiros do pai e da página à direita, caso existam.
    if (grupo->pag_dir != NULL) {
        vetor_chaves[i++] = grupo->pag_pai->chaves[pos_pai];

        int j;
        for (j = 0; j < n_dir; i++, j++) {
            vetor_chaves[i] = grupo->pag_dir->chaves[j];
            vetor_ponteiros[i] = grupo->pag_dir->ponteiros[j];
        }
        vetor_ponteiros[i] = grupo->pag_dir->ponteiros[j];
    }
}

// Realiza o shift nos vetores de chaves e de ponteiros para inserção de uma nova chave.
void shiftar_vetores_auxiliares(chave *vetor_chaves, int *vetor_ponteiros, int pos, int qntd_chaves) {
    for (int i = qntd_chaves - 1; i > pos; i--) {
        vetor_chaves[i] = vetor_chaves[i - 1];
        vetor_ponteiros[i + 1] = vetor_ponteiros[i];
    }
}

// Realiza o processo de redistribuição de chaves entre duas páginas irmãs e seu pai para inserção de uma chave nova.
// Retorna true se ocorrer redistribuição. Caso contrário, retorna false.
bool redistribuir(FILE *arq_arv, grupo_paginas *grupo, int pos_pai, int flag, chave *chave_nova, int rrn_propagado) {    
    // Armazenando as quantidades de chaves de cada página envolvida na redistribuição.
    int n_esq = grupo->pag_esq->qntd_chaves;
    int n_dir = grupo->pag_dir->qntd_chaves;    
    int n_total = n_esq + n_dir + 2; /* +2 = pai + chave nova*/
    
    // Verificando se é possível realizar redistribuição (para esquerda: flag == 0; para direita: flag == 1)
    if (flag == 0 && n_esq == ORDEM - 1) return false;
    else if (flag == 1 && n_dir == ORDEM - 1) return false;

    // Criando e preenchendo vetores auxiliares de chaves e ponteiros com as páginas envolvidas e com o pai.
    chave vetor_chaves[n_total];
    int vetor_ponteiros[n_total + 1];
    criar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, grupo, pos_pai);

    // Buscando posição de inserção da chave nova no vetor auxiliar de chaves.
    int pos = buscar_posicao_vetor_chaves(vetor_chaves, n_total - 1, chave_nova->chave_busca);

    // Shiftando vetores auxiliares para inserção da chave nova.
    shiftar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, pos, n_total);

    // Inserindo a nova chave nos vetores auxiliares.
    vetor_chaves[pos] = *chave_nova;
    vetor_ponteiros[pos + 1] = rrn_propagado;

    // Calculando as quantidades de chaves nas páginas filhas após a redistribuição.
    n_esq = n_total / 2;
    n_dir = n_total - n_esq - 1;

    // Atualizando e escrevendo as informações da página à esquerda.
    atualizar_pagina(grupo->pag_esq, grupo->pag_esq->nivel, n_esq, vetor_chaves, vetor_ponteiros, 0, n_esq);
    escrever_pagina(arq_arv, grupo->pag_esq, grupo->rrn_esq);

    // Atualizando e escrevendo as informações da página à direita.
    atualizar_pagina(grupo->pag_dir, grupo->pag_dir->nivel, n_dir, vetor_chaves, vetor_ponteiros, n_esq + 1, n_total);
    escrever_pagina(arq_arv, grupo->pag_dir, grupo->rrn_dir);

    // Atualizando o pai e escrevendo a página.
    grupo->pag_pai->chaves[pos_pai] = vetor_chaves[n_esq];
    escrever_pagina(arq_arv, grupo->pag_pai, grupo->rrn_pai);

    // Redistribuição realizada.
    return true;
}

// Realiza o processo de split 2 para 3 em duas páginas de um mesmo nível de uma árvore-b*.
bool split_2_3(FILE *arq_arv, arv_cabecalho *C_arv, grupo_paginas *grupo, int pos_pai, chave *chave_nova, int *rrn_propagado) {
    // Armazenando quantidade de chaves das páginas à esquerda e à direita do pai original. 
    // Para split 2 para 3, as duas páginas estão cheias.
    int n_esq = grupo->pag_esq->qntd_chaves;
    int n_dir = grupo->pag_dir->qntd_chaves;
    int n_total = n_esq + n_dir + 2; /* +2 = pai + chave nova*/
    
    // Criando vetores auxiliares de chaves e ponteiros.
    chave vetor_chaves[n_total];
    int vetor_ponteiros[n_total + 1];
    criar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, grupo, pos_pai);

    // Buscando posição de inserção da chave nova no vetor auxiliar de chaves.
    int pos = buscar_posicao_vetor_chaves(vetor_chaves, n_total - 1, chave_nova->chave_busca);

    // Shiftando vetores auxiliares para inserção da chave nova.
    shiftar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, pos, n_total);

    // Inserindo a nova chave nos vetores auxiliares na posição correta.
    vetor_chaves[pos] = *chave_nova;
    vetor_ponteiros[pos + 1] = *rrn_propagado;

    // Calculando as quantidades de chaves nas páginas filhas após a redistribuição. Uma nova página será criada.
    n_dir = (n_total - 2) / 3;
    n_esq = (n_total - n_dir - 2) / 2;
    int n_centro =  n_total - n_esq - n_dir - 2;

    // Particionando o vetor auxiliar de chaves entre as páginas filhas para auxiliar a atualização das páginas.
    int fim_esq = n_esq;
    int fim_centro = fim_esq + n_centro + 1;
    int fim_dir = n_total;

    // Atualizando informações da página à esquerda do pai original.
    atualizar_pagina(grupo->pag_esq, grupo->pag_esq->nivel, n_esq, vetor_chaves, vetor_ponteiros, 0, fim_esq);
    escrever_pagina(arq_arv, grupo->pag_esq, grupo->rrn_esq);

    // Atualizando informações da página do meio (à direita do pai original).
    pagina *pag_centro = alocar_pagina();
    atualizar_pagina(pag_centro, grupo->pag_esq->nivel, n_centro, vetor_chaves, vetor_ponteiros, fim_esq + 1, fim_centro);
    escrever_pagina(arq_arv, pag_centro, grupo->rrn_dir);
    desalocar_pagina(&pag_centro);

    // Atualizando informações da página à direita do pai promovido.
    atualizar_pagina(grupo->pag_dir, grupo->pag_dir->nivel, n_dir, vetor_chaves, vetor_ponteiros, fim_centro + 1, fim_dir);
    grupo->rrn_dir = C_arv->prox_rrn;
    escrever_pagina(arq_arv, grupo->pag_dir, grupo->rrn_dir);

    // Atualizando o pai original e armazenando a posição do pai promovido.
    grupo->pag_pai->chaves[pos_pai] = vetor_chaves[n_esq];
    int pos_pai_promovido = n_esq + n_centro + 1;

    // Atualizando cabeçalho da árvore-b* porque uma nova página foi criada.
    C_arv->prox_rrn++;
   
    // Verificando se é possível inserir o pai promovido.
    int n_pai = grupo->pag_pai->qntd_chaves; /* taxa de ocupação da página pai */
    if (n_pai != ORDEM - 1) { 
        // Shiftando vetores auxiliares para inserção do pai promovido.
        shiftar_vetores_auxiliares(grupo->pag_pai->chaves, grupo->pag_pai->ponteiros, pos_pai + 1, n_pai + 1);

        // Inserindo pai promovido e atualizando contador de chaves da página.
        grupo->pag_pai->chaves[pos_pai + 1] = vetor_chaves[pos_pai_promovido];
        grupo->pag_pai->ponteiros[pos_pai + 2] = grupo->rrn_dir;
        grupo->pag_pai->qntd_chaves++;

        // Não há propagação de split.
        return false; 
    } 
    
    // Caso não seja possível, reatribuindo chave a ser inserida devido à propagação de split.
    *chave_nova = vetor_chaves[pos_pai_promovido];
    *rrn_propagado = grupo->rrn_dir;

    // Há propagação de split.
    return true;     
}

// Verifica se deve ocorrer redistribuição ou split, nessa ordem, para a inserção de uma chave em uma página cheia. 
// Retorna a página mãe se houver propagação de split. Caso contrário, retorna NULL. 
pagina *tratar_pagina_cheia(FILE *arq_arv, arv_cabecalho *C_arv, pagina *pag_atual, int rrn_atual, pagina *pag_filha, 
                            int pos_pai, chave *chave_nova, int *rrn_propagado) {
    // Declarando variáveis auxiliares.
    pagina *pag_irma = alocar_pagina();
    bool existe_irma_esq = false, existe_irma_dir = false, redistribuiu = false;
    int rrn_irmao, rrn_filha = pag_atual->ponteiros[pos_pai];
    grupo_paginas *grupo;

    // Verificando se existem páginas irmãs da página cheia.
    if (pos_pai > 0) existe_irma_esq = true;
    if (pos_pai <= pag_atual->qntd_chaves - 1) existe_irma_dir = true;

    // Tentando redistribuir com a página irmã à esquerda da página cheia, caso exista.
    if (existe_irma_esq) {  
        // Encontrando RRN da página irmã e lendo-a.
        rrn_irmao = pag_atual->ponteiros[pos_pai - 1];
        fseek(arq_arv, (rrn_irmao + 1) * TAM_PAGINA, SEEK_SET);
        ler_pagina(arq_arv, pag_irma);

        // Tentando redistribuir à esquerda.
        grupo = criar_grupo_paginas(pag_irma, rrn_irmao, pag_filha, rrn_filha, pag_atual, rrn_atual);
        redistribuiu = redistribuir(arq_arv, grupo, pos_pai - 1, 0, chave_nova, *rrn_propagado);
        free(grupo);
    }

    // Tentando redistribuir com a irmã à direita da página cheia, caso exista e caso a redistribuição anterior falhe.
    if (!redistribuiu && existe_irma_dir) { 
        // Encontrando RRN da página irmã e lendo-a.
        rrn_irmao = pag_atual->ponteiros[pos_pai + 1];
        fseek(arq_arv, (rrn_irmao + 1) * TAM_PAGINA, SEEK_SET);
        ler_pagina(arq_arv, pag_irma);

        // Tentando redistribuir à direita.
        grupo = criar_grupo_paginas(pag_filha, rrn_filha, pag_irma, rrn_irmao, pag_atual, rrn_atual);
        redistribuiu = redistribuir(arq_arv, grupo, pos_pai, 1, chave_nova, *rrn_propagado);
        free(grupo);
    }

    // Verificando se a redistribuição ocorreu. Se sim, desalocando memória e retornando NULL.
    if (redistribuiu == true) {
        desalocar_varias_paginas(pag_atual, pag_filha, pag_irma);
        C_arv->nro_chaves++;
        return NULL;
    }

    // Caso contrário, realizando split 2 para 3. 
    // Verificando se existem páginas irmãs da página cheia e criando grupos de páginas.
    if (existe_irma_dir) {
        grupo = criar_grupo_paginas(pag_filha, rrn_filha, pag_irma, rrn_irmao, pag_atual, rrn_atual);
    } else if (existe_irma_esq) {
        grupo = criar_grupo_paginas(pag_irma, rrn_irmao, pag_filha, rrn_filha, pag_atual, rrn_atual);
        pos_pai--;
    }

    // Realizando split 2 para 3.
    bool propagou = split_2_3(arq_arv, C_arv, grupo, pos_pai, chave_nova, rrn_propagado);
    
    // Liberando memória alocada.
    free(grupo);

    // Verificando se há propagação de split. 
    // Se sim, desalocando memória, retornando página atual para subir um nível da árvore-b*.
    if (propagou) { 
        desalocar_varias_paginas(pag_filha, pag_irma, NULL);
        return pag_atual;
    } 
    
    // Se não, escrevendo página atual, desalocando memória e retornando NULL.
    escrever_pagina(arq_arv, pag_atual, rrn_atual);
    desalocar_varias_paginas(pag_atual, pag_filha, pag_irma);
    C_arv->nro_chaves++;
    return NULL;    
}

// Insere, recursivamente, uma chave em uma página da árvore-b*. Retorna a página caso esteja cheia antes da inserção.
pagina* inserir_chave(FILE *arq_arv, arv_cabecalho *C_arv, int rrn_atual, chave *chave_nova, int *rrn_propagado){
    // Alocando e lendo página auxiliar para o passo atual da recursão.
    pagina *pag_atual = alocar_pagina();
	fseek(arq_arv, (rrn_atual + 1) * TAM_PAGINA, SEEK_SET);
    ler_pagina(arq_arv, pag_atual);

	// Verificando se é um nó folha. A inserção só ocorre em nós folhas.
	if (pag_atual->nivel == 1){
        // Verificando se a folha está cheia. Se sim, retornando página atual para subir um nível da árvore-b*.
		if (pag_atual->qntd_chaves == ORDEM - 1) return pag_atual;

        // Calculando posição a ser inserida a nova chave na página.
        int pos = buscar_posicao_vetor_chaves(pag_atual->chaves, pag_atual->qntd_chaves, chave_nova->chave_busca);

        // Shiftando os vetores de chaves e de ponteiros da página na qual será inserida a nova chave.
        shiftar_vetores_auxiliares(pag_atual->chaves, pag_atual->ponteiros, pos, pag_atual->qntd_chaves + 1);

        // Inserindo a nova chave na página.
        pag_atual->chaves[pos] = *chave_nova;
        pag_atual->qntd_chaves++;

        // Escrevendo página atualizada no arquivo de árvore-b* e atualizando contador de chaves da árvore-b*.
        escrever_pagina(arq_arv, pag_atual, rrn_atual);
        C_arv->nro_chaves++;

		// Liberando memória alocada.
        desalocar_pagina(&pag_atual);

        // Página inserida, então retornando NULL.
		return NULL;
	}

    // Decidindo qual sub-árvore será acessada para inserção da nova chave.
    int pos = buscar_posicao_vetor_chaves(pag_atual->chaves, pag_atual->qntd_chaves, chave_nova->chave_busca);

    // Inserindo chave nova. Armazenando página filha se a inserção falhar (página cheia).
    pagina *pag_filha = inserir_chave(arq_arv, C_arv, pag_atual->ponteiros[pos], chave_nova, rrn_propagado);
    
    // Verificando se a inserção foi completa. Se sim, liberando memória e retornando NULL.
    if (pag_filha == NULL) {
        desalocar_pagina(&pag_atual);
        return NULL;
    }

    // Se não, a página filha está cheia. Então, tratando inserção em página cheia.
    return tratar_pagina_cheia(arq_arv, C_arv, pag_atual, rrn_atual, pag_filha, pos, chave_nova, rrn_propagado);
}

// Realiza o processo de split 1 para 2 na raiz de uma árvore-b*.
void split_1_2(FILE *arq_arv, arv_cabecalho *C_arv, chave *chave_nova, int *rrn_propagado, pagina *pag_raiz) {
    // Armazenando RRN da raiz antiga.
    int rrn_raiz_antiga = C_arv->no_raiz;

    // Armazenando quantidade de chaves das páginas à esquerda (raiz antiga) e à direita (página nova) da raiz nova.
    // Para split 1 para 2, há uma página cheia. 
    int n_esq = pag_raiz->qntd_chaves / 2;
    int n_dir = n_esq;
    int n_total = n_esq + n_dir + 1;

    // Criando e preenchendo vetores auxiliares.
    chave vetor_chaves[n_total];
    int vetor_ponteiros[n_total + 1];
    grupo_paginas *grupo;
    grupo = criar_grupo_paginas(pag_raiz, rrn_raiz_antiga, NULL, -1, NULL, -1);
    criar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, grupo, -1);
    free(grupo);

    // Calculando posição a ser inserida a nova chave no vetor auxiliar de chaves.
    int pos = buscar_posicao_vetor_chaves(vetor_chaves, n_total - 1, chave_nova->chave_busca);

    // Shiftando vetores auxiliares para inserção da chave nova.
    shiftar_vetores_auxiliares(vetor_chaves, vetor_ponteiros, pos, n_total);

    // Inserindo a nova chave e ponteiro para página nos vetores auxiliares.
    vetor_chaves[pos] = *chave_nova;
    vetor_ponteiros[pos + 1] = *rrn_propagado;

    // Atualizando página filha à esquerda da nova raiz.
    pagina *pag_esq = alocar_pagina();
    atualizar_pagina(pag_esq, pag_raiz->nivel, n_esq, vetor_chaves, vetor_ponteiros, 0, n_esq);
    escrever_pagina(arq_arv, pag_esq, rrn_raiz_antiga);

    // Atualizando página filha à direita da nova raiz.
    pagina *pag_dir = alocar_pagina();
    atualizar_pagina(pag_dir, pag_raiz->nivel, n_dir, vetor_chaves, vetor_ponteiros, n_esq + 1, n_total);
    int rrn_pag_dir_raiz = C_arv->prox_rrn;
    escrever_pagina(arq_arv, pag_dir, rrn_pag_dir_raiz);

    // Atualizando informações da nova raiz.
    atualizar_pagina(pag_raiz, pag_raiz->nivel + 1, 1, vetor_chaves, vetor_ponteiros, n_esq, n_esq + 1);
    pag_raiz->ponteiros[0] = rrn_raiz_antiga;
    pag_raiz->ponteiros[1] = rrn_pag_dir_raiz;

    // Atualizando cabeçalho da árvore-b* porque uma nova página foi criada.
    C_arv->prox_rrn++;

    // Liberando memória alocada.
    desalocar_varias_paginas(pag_esq, pag_dir, NULL);
}

// Verifica se é necessário realizar o split 1 para 2 na raiz. Retorna true caso ocorra. Caso contrário, false.
bool tentar_split_1_2(FILE *arq_arv, arv_cabecalho *C_arv, chave *chave_nova, int *rrn_propagado, pagina *pag_raiz) {
    // Verificando se houve propagação de inserção até a raiz. Se não, retorna false.
    if (pag_raiz == NULL) return false;

    // Se sim, realizando split 1 para 2.
    split_1_2(arq_arv, C_arv, chave_nova, rrn_propagado, pag_raiz);
    return true;
}

// Cria uma árvore-b*, inicializa o cabeçalho e insere uma chave. Retorna true caso seja criada. Caso contrário, false.
bool criar_arvore(FILE *arq_arv, arv_cabecalho *C_arv, reg_dados *D, long int byte_offset) {
    // Verificando se árvore-b* já existe, logo, não precisa ser criada.
    if (C_arv->no_raiz != -1) return false;

    // Caso ainda não exista, inicializando o cabeçalho da árvore-b*.
    atualizar_arv_cabecalho(C_arv, 0, 1, 1, 1);

    // Alocando e inicializando página raiz.
    pagina *pag_raiz = alocar_pagina();
    inicializar_vetores_pagina(pag_raiz);
    pag_raiz->nivel = 1;
    pag_raiz->qntd_chaves = 1;
    salvar_chave(&pag_raiz->chaves[0], retornar_id(D), byte_offset);

    // Escrevendo a raiz no arquivo de árvore-b*.
    escrever_pagina(arq_arv, pag_raiz, 0);

    // Liberando memória alocada.
    desalocar_pagina(&pag_raiz);

    // Árvore-b* criada.
    return true;
}

// Indexa um registro de dados, inserindo-o em um arquivo de árvore-b*.
void inserir_b_estrela(FILE *arq_arv, arv_cabecalho *C_arv, reg_dados *D, long int byte_offset) {
    // Criando árvore-b*, caso necessário.
    bool criou_arvore = criar_arvore(arq_arv, C_arv, D, byte_offset);

    // Se a árvore NÃO foi criada acima, inserindo a nova chave na árvore-b* já existente.
    if(!criou_arvore) {
        // Declarando variáveis auxiliares.
        pagina *pag_raiz;
        int rrn_propagado = -1;

        // Declarando nova chave.
        chave chave_nova;
        chave_nova.chave_busca = retornar_id(D);
        chave_nova.byte_offset = byte_offset;

        // Inserindo nova chave e armazenando se houve propagação de split até a raiz.
        pag_raiz = inserir_chave(arq_arv, C_arv, C_arv->no_raiz, &chave_nova, &rrn_propagado);

        // Tentando realizar split 1 para 2 na raiz, se necessário.
        bool splitou_raiz = tentar_split_1_2(arq_arv, C_arv, &chave_nova, &rrn_propagado, pag_raiz);

        if (splitou_raiz) {
            // Escrevendo nova raiz.
            int rrn_raiz_nova = C_arv->prox_rrn;
            escrever_pagina(arq_arv, pag_raiz, rrn_raiz_nova);

            // Liberando memória alocada.
            desalocar_pagina(&pag_raiz);

            // Atualizando informações do cabeçalho da árvore-b*.
            atualizar_arv_cabecalho(C_arv, rrn_raiz_nova, C_arv->prox_rrn + 1, C_arv->nro_niveis + 1, C_arv->nro_chaves + 1);
        }
    }
}