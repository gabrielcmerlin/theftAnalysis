// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a declaração das funcionalidades especificadas nos trabalhos introdutório e 1,
  correspondentes aos comandos da linguagem SQL, sendo eles:
    1. CREATE TABLE;
    2. SELECT;
    3. CREATE INDEX;
    4. SELECT (WHERE);
    5. DELETE;
    6. INSERT INTO;
    7. UPDATE.
*/

// FUNCIONALIDADE 1: Lê registros de dados de um arquivo .csv e escreve-os em um arquivo binário.
void criar_tabela(char *nome_csv, char *nome_dados);

// FUNCIONALIDADE 2: Imprime de forma organizada os dados de um arquivo binário.
void selecionar(char *nome_dados);

// FUNCIONALIDADE 3: Cria um arquivo de índice ordenado com registros de dados de índice.
void criar_indice(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice);

// FUNCIONALIDADE 4: Imprime os registros de um arquivo de dados seguindo um ou mais critérios de busca.
void selecionar_com_condicoes(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas);

// FUNCIONALIDADE 5: Deleta os registros de um arquivo de dados seguindo um ou mais critérios de busca.
void deletar(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas);

// FUNCIONALIDADE 6: Insere registros no fim do arquivo de dados e, se necessário, no arquivo de índices.
void inserir(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_insercoes);

// FUNCIONALIDADE 7: Atualiza os campos dos registros de um arquivo seguindo um ou mais critério de busca.
void atualizar(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas);

// FUNCIONALIDADE 8: Cria um arquivo de índice para um arquivos de dados, utilizando árvore-b*.
void criar_indice_arv_b(char *nome_dados, char *nome_arv_b);

// FUNCIONALIDADE 9: Imprime os registros de um arquivo de dados seguindo um ou mais critérios de busca, 
//                   utilizando árvore-b* para buscas cujo critério é um campo indexado.
void selecionar_arv_b(char *nome_dados, char *campo_indexado, char *tipo_dado, char *nome_indice, int qntd_buscas);

// FUNCIONALIDADE 10: Insere um ou mais registros de dados em um arquivo de índice de árvore-b*, lidos do usuário.
void inserir_arv_b(char *nome_dados, char *nome_arv_b, int qntd_insercoes);