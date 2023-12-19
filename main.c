// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin - 100% [12544420] & Pedro Augusto Monteiro Delgado - 100% [13672766]

#include <stdio.h>

#include "funcionalidades.h"
#include "utils.h"

int main(void){
    int opcao, qntd_operacoes;
    char nome_csv[TAM_STRING];
    char nome_dados[TAM_STRING];
    char campo_indexado[TAM_STRING];
    char tipo_dado[TAM_STRING];
    char nome_indice[TAM_STRING];

    scanf(" %d", &opcao);

    if (opcao == 1) {
        scanf(" %s %s", nome_csv, nome_dados);

        criar_tabela(nome_csv, nome_dados);
    } else if (opcao == 2) {
        scanf(" %s", nome_dados);

        selecionar(nome_dados);
    } else if (opcao == 3) {
        scanf(" %s %s %s %s", nome_dados, campo_indexado, tipo_dado, nome_indice);

        criar_indice(nome_dados, campo_indexado, tipo_dado, nome_indice);
    } else if (opcao == 4) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);

        selecionar_com_condicoes(nome_dados, campo_indexado, tipo_dado, nome_indice, qntd_operacoes);
    } else if (opcao == 5) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);

        deletar(nome_dados, campo_indexado, tipo_dado, nome_indice, qntd_operacoes);
    } else if (opcao == 6) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);

        inserir(nome_dados, campo_indexado, tipo_dado, nome_indice, qntd_operacoes);
    } else if (opcao == 7) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);

        atualizar(nome_dados, campo_indexado, tipo_dado, nome_indice, qntd_operacoes);
    } else if (opcao == 8) {
        scanf(" %s %s %s %s", nome_dados, campo_indexado, tipo_dado, nome_indice);

        criar_indice_arv_b(nome_dados, nome_indice);
    } else if (opcao == 9) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);

        selecionar_arv_b(nome_dados, campo_indexado, tipo_dado, nome_indice, qntd_operacoes);
    } else if (opcao == 10) {
        scanf(" %s %s %s %s %d", nome_dados, campo_indexado, tipo_dado, nome_indice, &qntd_operacoes);
    
        inserir_arv_b(nome_dados, nome_indice, qntd_operacoes);
    }

    return 0;
}