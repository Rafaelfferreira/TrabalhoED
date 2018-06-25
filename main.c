#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.c"
#define LEN 80

int main()
{
    FILE *arquivo; // cria vari�vel ponteiro para o arquivo
    char palavra[LEN]; // vari�vel do tipo string

    //abrindo o arquivo com tipo de abertura w
    arquivo = fopen("evaluation/input.txt", "r");

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


