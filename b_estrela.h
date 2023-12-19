// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das structs e das funções relacionadas à árvore-b*, como alocação, desalocação,
  busca, leitura, escrita e inserção.
*/

#define ORDEM 5
#define TAM_PAGINA 76

#include "dados.h"

// Define um registro de cabeçalho de árvore-b*.
typedef struct _arv_cabecalho arv_cabecalho;
struct _arv_cabecalho {
    char status;
    int no_raiz;
    int prox_rrn;
    int nro_niveis;
    int nro_chaves;
    char lixo[59];
} ;

// Define uma chave (chave de busca + byte offset no registro de dados).
typedef struct _chave chave;
struct _chave {
    int chave_busca;
    long int byte_offset;
} ;

// Define uma página de disco.
typedef struct _pagina pagina;
struct _pagina {
    int nivel;
    int qntd_chaves;
    int ponteiros[ORDEM];
    chave chaves[ORDEM - 1];
} ;

// Aloca registro de cabeçalho da árvore-b*.
arv_cabecalho *alocar_arv_cabecalho();

// Desaloca registro de cabeçalho da árvore-b*.
void desalocar_arv_cabecalho(arv_cabecalho **C_arv);

// Lê, campo a campo, o registro de cabeçalho da árvore-b* a partir de um arquivo.
void ler_arv_cabecalho(FILE *arq_arv, arv_cabecalho *C_arv);

// Retorna o byte offset de uma chave buscada em uma árvore-b*. Retorna -1, caso não encontre.
long int encontrar_byte_offset(FILE *arq_arv, int rrn_raiz, int chave_buscada);

// Escreve, campo a campo, o registro de cabeçalho da árvore-b* num arquivo.
void escrever_arv_cabecalho(FILE *arq_arv, arv_cabecalho *C_arv);

// Inicializa os campos do registro de cabeçalho da árvore-b*.
void inicializar_arv_cabecalho(arv_cabecalho *C_arv);

// Indexa um registro de dados, inserindo-o em um arquivo de árvore-b*.
void inserir_b_estrela(FILE *arq_arv, arv_cabecalho *C_arv, reg_dados *D, long int byte_offset);