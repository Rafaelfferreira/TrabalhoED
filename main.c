#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvore.c"
#include "utilidades.c"
#include <locale.h>

#define LEN 80

int main()
{
    setlocale(LC_ALL, "Portuguese");//habilita a acentua��o para o portugu�s

    pNodoA *ArvoreGeral;

    FILE *arquivo; // cria vari�vel ponteiro para o arquivo de leitura
    FILE *arquivoOp, *arquivoWr; // cria variavel ponteiro para o arquivo com as operacoes e para o arquivo de output
    char palavra[LEN]; // vari�vel do tipo string

    //abrindo o arquivo com tipo de abertura w
    arquivo = fopen("evaluation/input_ANSI cheio.txt", "r");

    //testando se o arquivo foi realmente criado
    if(arquivo == NULL)
    {
         printf("Erro na abertura do arquivo!");
         return 1;
    }

    ArvoreGeral = leArquivo(arquivo);

    //fechamos o arquivo de input
    fclose(arquivo);

    arquivoOp = fopen("evaluation/operations_ANSI.txt", "r"); //abrindo arquivo de operacoes
    arquivoWr = fopen("evaluation/output_ANSI.txt", "a"); // abrindo o arquivo de output

    leOperacoes(arquivoOp, arquivoWr, ArvoreGeral);



    return 0;
}


