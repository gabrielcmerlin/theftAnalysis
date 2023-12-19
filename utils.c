// Organização de Arquivos (SCC0215) - Trabalho 2
// Gabriel da Costa Merlin [12544420] & Pedro Augusto Monteiro Delgado [13672766]

/*
  Este módulo contém a implementação das funções auxiliares ao programa principal, que não pertencem ao escopo dos
  demais módulos, como funções fornecidas, impressão de erros e manipulação de strings em geral.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "utils.h" 
#include "cabecalho.h"

// Função padrão do trabalho.
void binarioNaTela(char *nomeArquivoBinario) {
    /*
     * Você não precisa entender o código dessa função.
     *
     * Use essa função para comparação no run.codes.
     * Lembre-se de ter fechado (fclose) o arquivo anteriormente.
     * Ela vai abrir de novo para leitura e depois fechar
     * (você não vai perder pontos por isso se usar ela).
     */

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
        fprintf(stderr,
                "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): "
                "não foi possível abrir o arquivo que me passou para leitura. "
                "Ele existe e você tá passando o nome certo? Você lembrou de "
                "fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++) {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);

}

// Função padrão do trabalho.
void scan_quote_string(char *str) {
    /*
     * Use essa função para ler um campo string delimitado entre aspas (").
     * Chame ela na hora que for ler tal campo. Por exemplo:
     *
     * A entrada está da seguinte forma:
     *    nomeDoCampo "MARIA DA SILVA"
     *
     * Para ler isso para as strings já alocadas str1 e str2 do seu programa,
     * você faz:
     *    scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *    scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2
     *                             // (sem as aspas)
     */

    char R;

    while ((R = getchar()) != EOF && isspace(R)) {
        ;  // ignorar espaços, \r, \n...
    }

    if (R == 'N' || R == 'n') {  // campo NULO
        getchar();
        getchar();
        getchar();  // ignorar o "ULO" de NULO.
        strcpy(str, "");  // copia string vazia
    } else if (R == '\"') {
        if (scanf("%[^\"]", str) != 1) {  // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar();  // ignorar aspas fechando
    } else if (R != EOF) {  // vc tá tentando ler uma string
                            // que não tá entre aspas!
                            // Fazer leitura normal %s então,
                            // pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else {  // EOF
        strcpy(str, "");
    }
}

// Retorna a quantidade de dígitos, em decimal, de um número n.
int qntd_digitos(int n) {
  int qntd_dig = 0;

  while(n != 0) {
    n /= 10;

    qntd_dig++;
  }

  return qntd_dig;
}

// Inverte a posição dos caracteres de uma string.
void inverter(char *string) {
  char c;
 
  for (int i = 0, j = (strlen(string) - 1); i < j; i++, j--) {
      c = string[i];
      string[i] = string[j];
      string[j] = c;
  }
}

// Transforma um valor inteiro em uma string.
void int_para_string(int n, char **string) {
  int qntd_dig = qntd_digitos(n);

  for(int i = 0; i < qntd_dig; i++){
    (*string)[i] = n % 10 + '0';
    n /= 10;
  }
  (*string)[qntd_dig] = '\0';
  inverter((*string));
}

// Trunca uma string para 12 caracteres. Caso a string seja menor, preenche o restante com '$'.
char *truncar_string_doze(char *str){
    char *string_aux = (char *) malloc (13 * sizeof(char));
    string_aux[12] = '\0';
    
    if (strlen(str) < 12) {
        strcpy(string_aux, str);
        colocar_cifrao(string_aux, 12);
    } else {
        strncpy(string_aux, str, 12);
        string_aux[12] = '\0';
    }

    return string_aux;
}

// Preenche uma string com '$'.
void colocar_cifrao(char *string, int tam_max) {
  int tam_string = strlen(string);
  for(int i = tam_string; i < tam_max; i++) {
    string[i] = '$';
  }
}

// Tira o primeiro caractere '$' de uma string.
void tirar_cifrao(char *string) {
  int tam_string = strlen(string);
  for(int i = 0; i < tam_string; i++) {
    if(string[i] == '$') {
      string[i] = '\0';
      break;
    }
  }
}

// Imprime mensagem de erro específica para cada falha.
void printar_erro(int flag){
    switch(flag){
        case 0: // Arquivo inconsistente.
            printf("Falha no processamento do arquivo. \n");
        break;
        case 1: // Falha na abertura de arquivos.
            printf("Falha no processamento do arquivo. \n");
        break;
        case 2: // Falha na escrita em arquivos.
            printf("Falha no processamento do arquivo. \n");
        break;
        case 3: // Falha na leitura de arquivos.
            printf("Falha no processamento do arquivo. \n");
        break;
        case 4: // Falha na alocação de memória.
            printf("Falha na alocação de memória. \n");
        break;
        case 5: // Não há registros no arquivo.
            printf("Registro inexistente.\n");
        break;
    }
}

// Verifica se um arquivo está vazio. Se sim, imprime erro e retorna true. Caso contrário, retorna false.
bool verificar_arq_vazio(int total_bytes, int tam_cabecalho){
  if(total_bytes == tam_cabecalho){ // Não há registros de dados no arquivo.
    printar_erro(5);
    return true;
  }
  return false;
}

// Verifica se um arquivo está inconsistente. Se sim, imprime erro e retorna true. Caso contrário, retorna false.
bool verificar_arq_inconsistente(char status){
  if(status == '0') { // Arquivo está inconsistente
    printar_erro(0);
    return true;
  }
  return false;
}

// Conta a quantidade de '$' entre o delimitador do último campo variável ('|') e o delimitador de registro ('#').
int contar_lixo(FILE *arq_dados, reg_dados *D){
  int qntd_cifrao = 0;
  char *char_auxiliar = (char *) malloc(1 * sizeof(char));

  if (retornar_delimitador(D) == '$') { 
    qntd_cifrao += fread(char_auxiliar, sizeof(char), 1, arq_dados);
    while (*char_auxiliar != '#') {
      qntd_cifrao += fread(char_auxiliar, sizeof(char), 1, arq_dados);
    }
  }

  free(char_auxiliar);
  return qntd_cifrao;
}