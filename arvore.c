#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"


#define LEN 80


pNodoA* leArquivo(FILE *arquivo)
// a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo
{
    char pesquisa[LEN]; // le termo do arquivo
    pNodoA *localidade; // endereco da localidade
    int i; // indice da string
    int flagCidade = 1; // flag = 1 -> string representa localidade

    pNodoA *arvoreFinal = (pNodoA*) malloc(sizeof(pNodoA));

    while((pesquisa[0] = getc(arquivo)) != EOF)
    {
        i = 1; // pois o primeiro caractere ja foi lido no while mais externo do EOF
        while ( (pesquisa[i] = getc(arquivo)) != ';' &&
                pesquisa[i] != '\n')
        {
            //printf("%c", pesquisa[i-1]);
            //getchar();
            i++;
        }

        if (flagCidade == 1) // se leu uma cidade
        {
            flagCidade = 0; // seta a flag de cidade para zero
            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            //chama a funcao que insere cidade
            //arvoreFinal = insereLocalidade(arvoreFinal, localidade, pesquisa);
            //puts(pesquisa);
            //imprimeCidades(arvoreFinal);
            puts(pesquisa);
            printf("\n");

        }

        else if(pesquisa[i] == '\n') // se chegou no fim da linha
        {
            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            flagCidade = 1; // seta a flag pra 1 novamente pois a proxima leitura sera de uma cidade
        }
        //checa se eh uma pesquisa composta:::::::::::::::::
        //se sim, insere composta::::::::::::::::
        //se nao, insere simples::::::::::::::::

    }

    return 0;
}


pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA *localidadeAtual, char info[LEN]) // funcao que insere localidade na arvore
//localidade atual sera atualizada para otimizar a insercao das pesquisas por cidade
{
    if (inicioArvore == NULL) //adiciona cidade
    {
        strcpy(info, inicioArvore->info);
        inicioArvore->dir = NULL;
        inicioArvore->esq = NULL;
        inicioArvore->pesquisas = -1;
    }
    else if (strcpy(info, inicioArvore->info) == 0) // se a string da pesquisa for igual a info do nodo atual
    {
        localidadeAtual = inicioArvore; //testar aqui se nao eh um endereco......
        return inicioArvore;
    }

    else //se existe algum nodo a direita do
        return insereLocalidade(inicioArvore->dir, localidadeAtual, info[LEN]);
}

void imprimeCidades(pNodoA *a)
{
    if (a == NULL)
        return 0;

    printf("%s --- ", a->info);
    imprimeCidades(a->dir);
}

void pint2()
{
    printff();
}
/*
if (a == NULL) // se a arvore for inexistente cria arvore e nem checa se e cidade
    {
        a =  (pNodoA*) malloc(sizeof(pNodoA));
        strcpy(a->info, info); // copia a string passada como parametro pra info
        a->pesquisas = -1; // indica que e uma cidade
        a->esq = NULL;
        a->dir = NULL;
        return a;
    }
    */
