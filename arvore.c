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

    strcpy(arvoreFinal->info, "Global"); // primeiro nodo corresponde às pesquisas e termos globais (de todas as localidades)
    arvoreFinal->termos = NULL;
    arvoreFinal->consultas = NULL;
    arvoreFinal->dir = NULL;
    arvoreFinal->esq = NULL;


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
            arvoreFinal = insereLocalidade(arvoreFinal, localidade, pesquisa);
            //puts(pesquisa);
            //imprimeCidades(arvoreFinal);

            //printf("%s --- ", arvoreFinal->info);
            //getchar();
        }

        else if(pesquisa[i] == '\n') // se chegou no fim da linha
        {
            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            flagCidade = 1; // seta a flag pra 1 novamente pois a proxima leitura sera de uma cidade
        }


        //getchar();
        //checa se eh uma pesquisa composta:::::::::::::::::
        //se sim, insere composta::::::::::::::::
        //se nao, insere simples::::::::::::::::

    }

    return 0;
}


pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA *localidadeAtual, char info[LEN])
//  funcao que insere localidade na arvore
// localidade atual sera atualizada para otimizar a insercao das pesquisas por cidade
{
    if (inicioArvore == NULL) //adiciona cidade
    {

        pNodoA *novaLoc = (pNodoA*) malloc(sizeof(pNodoA));

        strcpy(novaLoc->info, info);
        novaLoc->termos = NULL;
        novaLoc->consultas = NULL;
        novaLoc->dir = NULL;
        novaLoc->esq = NULL;

        novaLoc;

        return novaLoc;
    }

    else if (strcmp(info, inicioArvore->info) == 0) // se a string da pesquisa for igual a info do nodo atual (aquela localidade já estpa na árvore)
    {

        localidadeAtual = inicioArvore; // atualiza endereço de localidade atual
        return inicioArvore; // retorna árvore idêntica
    }

    char stringPrecedente[LEN];
    ordemAlfabetica(info, inicioArvore->info, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    //se info, pela ordem alfabetica, vem antes, vai pra esquerda
    if (strcmp(info, stringPrecedente) == 0)
        inicioArvore->esq = insereLocalidade(inicioArvore->esq, localidadeAtual, info);

    else // se não, vai pra direita
    {
        inicioArvore->dir = insereLocalidade(inicioArvore->dir, localidadeAtual, info);
    }

    return inicioArvore;
}


void imprimeCidades(pNodoA *a)
{
    if (a == NULL)
        return 0;

    imprimeCidades(a->esq);
    printf("%s --- ", a->info);
    imprimeCidades(a->dir);

}

