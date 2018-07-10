#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvore.c"
#include "utilidades.c"
#include <locale.h>

#define LEN 80

// argumentos[0] corresponde ao nome do programa chamado, a partir daí, cada cada argumento n está armazenado em argumentos[n]
int main(int numArgumentos, char *argumentos[])
{
    setlocale(LC_ALL, "Portuguese");//habilita a acentuação para o português

    pNodoA *ArvoreGeral;

    FILE *arquivo; // cria variável ponteiro para o arquivo de leitura
    FILE *arquivoOp, *arquivoWr; // cria variavel ponteiro para o arquivo com as operacoes e para o arquivo de output
    char palavra[LEN]; // variável do tipo string

    //abrindo o arquivo com tipo de abertura w
    arquivo = fopen(argumentos[1], "r");

    //testando se o arquivo foi realmente criado
    if(arquivo == NULL)
    {
         printf("Erro na abertura do arquivo!");
         return 1;
    }

    ArvoreGeral = leArquivo(arquivo);

    //fechamos o arquivo de input
    fclose(arquivo);

    arquivoOp = fopen(argumentos[2], "r"); //abrindo arquivo de operacoes
    arquivoWr = fopen(argumentos[3], "a"); // abrindo o arquivo de output

    leOperacoes(arquivoOp, arquivoWr, ArvoreGeral);

    fclose(arquivoOp);
    fclose(arquivoWr);

    return 0;
}


