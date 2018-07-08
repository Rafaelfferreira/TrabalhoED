#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvore.h"


#define LEN 80


pNodoA* leArquivo(FILE *arquivo)
// a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo
{
    clock_t tempo;
	tempo = clock();


    char pesquisa[LEN]; // le termo do arquivo
    int i; // indice da string
    int flagCidade = 1; // flag = 1 -> string representa localidade
    int quantidadeDeTermos = 0; // quantidade de termos para consultas compostas

    pNodoA *localidade; // endereco da localidade
    itemA *consultaLSE = NULL; // lista simplesmente encadeada dos termos
    nodoConsulta *consultas = NULL; // árvore de consultas
    nodoTermo *termos = NULL; // árvore de termos


    pNodoA *arvoreFinal = (pNodoA*) malloc(sizeof(pNodoA)); // árvore com as localidades

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
                i++;




        if (flagCidade) // se leu uma cidade
        {
            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            flagCidade = 0; // seta a flag de cidade para zero
            //chama a funcao que insere cidade
            arvoreFinal = insereLocalidade(arvoreFinal, &localidade, pesquisa);
        }



        else // se não for uma cidade
        {

            if(pesquisa[i] == '\n') // se chegou no fim da linha
            {
                flagCidade = 1; // seta a flag pra 1 novamente pois a proxima leitura sera de uma cidade
                pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            }




            //inserção das consultas

            if(verificaComposta(pesquisa)) //checa se é uma pesquisa composta
                consultaLSE = separaTermos(pesquisa, &quantidadeDeTermos); // separa os termos


            //se nao, cria consulta simples e seta a quantidade de termos para 1
            else
            {
                consultaLSE = criaConsultaSimples(pesquisa); // cria consulta simples (lista de um elemento)
                quantidadeDeTermos = 1;
            }


            arvoreFinal->consultas = insereConsulta(arvoreFinal->consultas, consultaLSE, quantidadeDeTermos); //insere a consulta na árvore global
            localidade->consultas = insereConsulta(localidade->consultas, consultaLSE, quantidadeDeTermos); //insere a consulta na localidade atual



            quantidadeDeTermos = 0; // depois de inserida a consulta, zera a quantidade de termos da consulta (reutilização da var)



            // inserção de termos
            while (consultaLSE != NULL)
            {
                // insere termo atual na árvore de termos da localidade atual
                localidade->termos = insereTermo(localidade->termos, consultaLSE->info);
                // insere termo atual na árvore de termos globais
                arvoreFinal->termos = insereTermo(arvoreFinal->termos, consultaLSE->info);

                //avança para próximo termo
                consultaLSE = consultaLSE->prox;
            }
            // aqui consultaLSE já valerá NULL e poderá ser reutilizada

            // teste de impressão
           // imprimeArvore(arvoreFinal);
        }
    }

    nodoConsulta *globalReestruturadaCons = NULL; // nova árvore de consultas global organizada por quantidade de pesquisas
    nodoTermo *globalReestruturadaTerm = NULL; // nova árvore de termos global organizada por quantidade de pesquisas


    // chama as funções que reestruturam a árvore de acordo com a quantidade de pesquisas
    globalReestruturadaCons = reestruturaConsultaQuantidade(arvoreFinal->consultas, globalReestruturadaCons);
    globalReestruturadaTerm = reestruturaTermoQuantidade(arvoreFinal->termos, globalReestruturadaTerm);


    printf("Tempo:%f",(clock() - tempo) / (double)CLOCKS_PER_SEC);
    getchar();
    imprimeConsultas(globalReestruturadaCons);
    getchar();
    //imprimeArvore(arvoreFinal);
    return 0;
}


pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA **localidadeAtual, char info[LEN])
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


        *localidadeAtual = novaLoc;
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



nodoConsulta* insereConsulta(nodoConsulta *consultas, itemA *consulta, int quantidadeDeTermos)
//  funcao que insere consulta na arvore de consultas
{

    if (consultas == NULL) //adiciona consulta
    {
        itemA *consultaAux = consulta; // pois consulta será alterada na função que insere termos e a lista, perdida
        nodoConsulta *novaCons = (nodoConsulta*) malloc(sizeof(nodoConsulta));

        novaCons->infoLSE = consultaAux;
        novaCons->dir = NULL;
        novaCons->esq = NULL;
        novaCons->numTermos = quantidadeDeTermos;
        novaCons->quantidade = 1;


        return novaCons;
    }

    char stringPrecedente[LEN]; //variável auxiliar pra testar pra que lado vai a consulta na árvore
    ordemAlfabetica(consulta->info, consultas->infoLSE->info, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    // testa se tem a mesma quantidade de termos
    if (consultas->numTermos == quantidadeDeTermos) // se a string da pesquisa for igual a info do nodo atual (aquela localidade já estpa na árvore)
    {
        if (testaMesmaConsulta(consulta, consultas->infoLSE)) //testa se é a mesma consulta
        {
            consultas->quantidade++; // aumenta a quantidade de vezes que aquela consulta foi feita
            return consultas; // e retorna a recursão
        }

        // caso contrário, verifica pra onde ir (direita ou esquerda da consulta atual)
        // se a consulta nova vier antes na ordem alfabética, coloca ela à esquerda
        if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info é a precedente
            consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);

        // caso contrário, insere à direita
        else
            consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);
    }


    // se não tiver a mesma quantidade, é uma consulta diferente, logo faz os testes para ver se vai para a direita ou esquerda:
    else if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info é a precedente
            consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);

    // caso contrário, insere à direita
    else
        consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);

    return consultas;
}



nodoTermo* insereTermo(nodoTermo *termoNodo, char termo[LEN/2])
//  funcao que insere termo na árvore de termos
{
    if (termoNodo == NULL) //adiciona termo
    {

        nodoTermo *novoTermo = (nodoTermo*) malloc(sizeof(nodoTermo));

        strcpy(novoTermo->info, termo);
        novoTermo->dir = NULL;
        novoTermo->esq = NULL;
        novoTermo->quantidade = 1;


        return novoTermo;
    }

    if(!strcmp(termo, termoNodo->info)) //se o termo for igual ao termo atual (notar que strcmp devolve zero para strs iguais)
    {
        termoNodo->quantidade++; // só aumenta a quantidade de vezes que aquele termo foi pesquisado
        return termoNodo;
    }

    char stringPrecedente[LEN]; //variável auxiliar pra testar pra que lado vai a consulta na árvore
    ordemAlfabetica(termoNodo->info, termo, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    // testa ordem alfabética para saber se insere à direita ou esquerda
    if(strcmp(termo, stringPrecedente)) //se o termo vier depois na ordem alfabética, insere à direita
        termoNodo->dir = insereTermo(termoNodo->dir, termo);

    //caso contrário insere à esquerda
    else
        termoNodo->esq = insereTermo(termoNodo->esq, termo);


    return termoNodo;
}


nodoConsulta* reestruturaConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novaArvore)
// recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
// devolve a nova árvore
{

    if (arvore == NULL)
        return NULL; // critério de parada da recursão

    // a inserção utiliza o pós-fixado esquerda (pois assim está organizado pelo critério alfabético)
    reestruturaConsultaQuantidade(arvore->esq, novaArvore);
    novaArvore = insereConsultaQuantidade(novaArvore, arvore); //arvore nesse ponto se entende como folha
    reestruturaConsultaQuantidade(arvore->dir, novaArvore);
    //novaArvore = insereConsultaQuantidade(novaArvore, arvore); //arvore nesse ponto se entende como folha

    free(arvore); // libera o espaço ocupado por aquela folha

    return novaArvore;

}

nodoConsulta* insereConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo)
// insere um nodo numa árvore de consultas de acordo com a quantidade destas (ou ordem alfabética)
{

    if(arvore == NULL) // se chegamos na extremidade da árvore, adicionamos um novo nodo
    {
        nodoConsulta *arvoreNova = (nodoConsulta*) malloc(sizeof(nodoConsulta)); // cria nova folha

        arvoreNova->infoLSE = novo->infoLSE;
        arvoreNova->numTermos = novo->numTermos;
        arvoreNova->quantidade = novo->quantidade;
        arvoreNova->dir = NULL; // garante que não haja confusão de ficar linkado com a árvore anterior
        arvoreNova->esq = NULL;

        return arvoreNova;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereConsultaQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereConsultaQuantidade(arvore->esq, novo);

    else // se tiver a mesma quantia de consultas, utiliza critério alfabético
    // como a outra árvore já está ordenada de forma alfabética, é só garantir que a inserção ocorre de maneira pós fixada à esquerda
    // porém note que esse modo constroi arvore para ser printada como pós fixada direita
        arvore->dir = insereConsultaQuantidade(arvore->dir, novo);

    return arvore;
}


nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo *novaArvore)
// recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
// devolve a nova árvore
{

    if (arvore == NULL)
        return NULL; // critério de parada da recursão

    // a inserção utiliza o pós-fixado esquerda (pois assim está organizado pelo critério alfabético)
    reestruturaTermoQuantidade(arvore->esq, novaArvore);
    novaArvore = insereTermoQuantidade(novaArvore, arvore); //arvore agora se entende como folha
    reestruturaTermoQuantidade(arvore->dir, novaArvore);
    //novaArvore = insereTermoQuantidade(novaArvore, arvore); //arvore nesse ponto se entende como folha
    free(arvore); // libera o espaço ocupado por aquela folha

    return novaArvore;
}


nodoTermo* insereTermoQuantidade(nodoTermo *arvore, nodoTermo *novo)
// insere o nodo novo em uma árvore de termos
{
    if(arvore == NULL) // se chegamos na extremidade da árvore, adicionamos um novo nodo
    {
        nodoTermo *novoTermo = (nodoTermo*) malloc(sizeof(nodoTermo));

        strcpy(novoTermo->info, novo->info);
        novoTermo->quantidade = novo->quantidade;
        novoTermo->dir = NULL; // garante que não haja confusão de ficar linkado com a árvore anterior
        novoTermo->esq = NULL;

        return novoTermo;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereTermoQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereTermoQuantidade(arvore->esq, novo);

    else // caso contrário utiliza o critério de ordenamento alfabético
    // como a outra árvore já está ordenada de forma alfabética, é só garantir que a inserção ocorre de maneira pós fixada à esquerda
    // porém note que esse modo constroi arvore para ser printada como pós fixada direita
        arvore->dir = insereTermoQuantidade(arvore->dir, novo);

    return arvore;

}





void imprimeArvore(pNodoA *a)
{
    if (a == NULL)
        return 0;

    imprimeArvore(a->esq);

        printf("Localidade: %s \n\nTermos:\n", a->info);
        imprimeTermos(a->termos);
        printf("\nConsultas:\n");
        imprimeConsultas(a->consultas);
        printf("\n\n\n");

    imprimeArvore(a->dir);

    //getchar();

}

void imprimeConsultas(nodoConsulta *a)
{
    if (a == NULL)
        return 0;

    imprimeConsultas(a->esq);

    itemA *aux = a->infoLSE;

    printf("Quantidade: %d | ", a->quantidade);
    while (aux != NULL)
    {
        printf("%s - ", aux->info);
        aux = aux->prox;

    }
    printf("\n");

    imprimeConsultas(a->dir);
}

void imprimeTermos(nodoTermo *a)
{
    if (a == NULL)
        return 0;

    imprimeTermos(a->esq);

    printf("Quantidade: %d | %s\n", a->quantidade, a->info);

    imprimeTermos(a->dir);
}

