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

    FILE *arquivo; // cria vari�vel ponteiro para o arquivo
    char palavra[LEN]; // vari�vel do tipo string

    //abrindo o arquivo com tipo de abertura w
    arquivo = fopen("evaluation/input_ANSI.txt", "r");

    //testando se o arquivo foi realmente criado
    if(arquivo == NULL)
    {
         printf("Erro na abertura do arquivo!");
         return 1;
    }

    leArquivo(arquivo);

    //fechamos o arquivo
    fclose(arquivo);



    return 0;
}


