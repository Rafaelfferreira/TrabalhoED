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

    strcpy(arvoreFinal->info, "Global"); // primeiro nodo corresponde �s pesquisas e termos globais (de todas as localidades)
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
//  Retorna sempre a arvore de localidades
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

    else if (strcmp(info, inicioArvore->info) == 0) // se a string da pesquisa for igual a info do nodo atual (aquela localidade j� estpa na �rvore)
    {

        localidadeAtual = inicioArvore; // atualiza endere�o de localidade atual
        return inicioArvore; // retorna �rvore id�ntica
    }

    char stringPrecedente[LEN];
    ordemAlfabetica(info, inicioArvore->info, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    //se info, pela ordem alfabetica, vem antes, vai pra esquerda
    if (strcmp(info, stringPrecedente) == 0)
        inicioArvore->esq = insereLocalidade(inicioArvore->esq, localidadeAtual, info);

    else // se n�o, vai pra direita
    {
        inicioArvore->dir = insereLocalidade(inicioArvore->dir, localidadeAtual, info);
    }

    return inicioArvore;
}

//Recebe o arquivo das operacoes e a arvore que contem as localidades
//Retorna void porem escreve no arquivo de saida
void leOperacoes(FILE *arquivo, pNodoA *ArvoreGeral)
{
    char termo[LEN], parametro1[LEN], parametro2[LEN]; // le termo do arquivo
    int i, quantidade; // indice da string
    char operacao; //Salva qual a operacao ira ser executada a seguir


    while((termo[0] = getc(arquivo)) != EOF) //Loop ate o arquivo acabar
    {
        operacao = termo[0]; //O primeiro char de cada linha a ser lido eh a operacao

        i = 0; //resetando o indice que le os parametros

        //Leitura do segundo termo da linha (Ou a localidade ou a quantidade, dependendo da operacao)
        if(operacao != 'f') //Se a operacao for f nao precisa de mais nenhum termo
        {
            while ( (parametro1[i] = getc(arquivo)) != ';' &&     //Percorre o arquivo ate um nova linha ou um ;
                    parametro1[i] != '\n')
            {
                i++;
            }

            //Cai aqui quando acabou de ler o primeiro termo da operacao
            if(operacao == 'a' || operacao == 'c' || operacao == 'e') //Testa se eh uma operacao que recebe a localidade como o segundo parametro
                parametro1[i] = '\0';     //Coloca o sinal de final da string

            if(operacao == 'b' || operacao == 'd') //Testa se eh uma operacao que recebe uma quantidade como segundo parametro
                quantidade = atoi(parametro1); //Transforma o parametro 1 em uma int e salva em quantidade
        }

        //Leitura do terceiro termo da linha (a quantidade de termos a ser impresso)
        if(operacao =='a' || operacao == 'c')   //A e C sao as duas unicas operacoes que precisam receber 2 parametros
        {
            i = 0; //resetando o indice que le os parametros

            while ( (parametro1[i] = getc(arquivo)) != ';' &&     //Percorre o arquivo ate um nova linha ou um ;
                    parametro1[i] != '\n')
            {
                i++;
            }

            quantidade = atoi(parametro2); //Converte a quantidade para uma int
        }

        //Decide qual funcao de operacao deve ser chamada
        switch(operacao)
        {
            case 'a':
                //operacaoA(ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao A e escreve o output no txt de saida
                break;
            case 'b':
                //operacaoB(ArvoreGeral ,quantidade); //Funcao que realiza a operacao B e escreve o output no txt de saida
                break;
            case 'c':
                //operacaoC(ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao C e escreve o output no txt de saida
                break;
            case 'd':
                //operacaoD(ArvoreGeral , quantidade); //Funcao que realiza a operacao D e escreve o output no txt de saida
                break;
            case 'e':
                //operacaoE(ArvoreGeral , parametro1); //Funcao que realiza a operacao E e escreve o output no txt de saida
                break;
            case 'f':
                //operacaoF(ArvoreGeral); //Funcao que realiza a operacao F e escreve o output no txt de saida
                break;
        }

    }
}


void imprimeCidades(pNodoA *a)
{
    if (a == NULL)
        return 0;

    imprimeCidades(a->esq);
    printf("%s --- ", a->info);
    imprimeCidades(a->dir);

}

