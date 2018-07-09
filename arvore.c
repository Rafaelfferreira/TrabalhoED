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
                flagCidade = 1; // seta a flag pra 1 novamente pois a proxima leitura sera de uma cidade


            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo




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
        }
    }

    //imprimeArvore(arvoreFinal);
    //getchar();

    //nodoConsulta *globalReestruturadaCons = NULL; // nova árvore de consultas global organizada por quantidade de pesquisas
    //nodoTermo *globalReestruturadaTerm = NULL; // nova árvore de termos global organizada por quantidade de pesquisas


    // chama as funções que reestruturam a árvore de acordo com a quantidade de pesquisas
    nodoConsulta *globalReestruturadaCons = (nodoConsulta*) malloc(sizeof(nodoConsulta));
    globalReestruturadaCons = reestruturaConsultaQuantidade(arvoreFinal->consultas, &globalReestruturadaCons);
    nodoTermo *globalReestruturadaTerm = (nodoTermo*) malloc(sizeof(nodoTermo));
    reestruturaTermoQuantidade(arvoreFinal->termos, &globalReestruturadaTerm);


    printf("Tempo:%f\n\n", (clock() - tempo) / (double)CLOCKS_PER_SEC);
   /* imprimeConsultas(arvoreFinal->consultas);
    getchar();
    imprimeConsultas(globalReestruturadaCons);
    getchar();

    imprimeTermos(arvoreFinal->termos);
    getchar();
    imprimeTermos(globalReestruturadaTerm);
    getchar(); */

    //imprimeArvore(arvoreFinal);
    return 0;
}


pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA **localidadeAtual, char info[LEN])
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

    //se info, pela ordem alfabetica, vem antes, vai pra direita
    if (strcmp(info, stringPrecedente) == 0)
        inicioArvore->dir = insereLocalidade(inicioArvore->dir, localidadeAtual, info);

    else // se não, vai pra esquerda
    {
        inicioArvore->esq = insereLocalidade(inicioArvore->esq, localidadeAtual, info);
    }

    return inicioArvore;
}

//Recebe o arquivo das operacoes e a arvore que contem as localidades
//Retorna void porem escreve no arquivo de saida
void leOperacoes(FILE *arquivo, FILE *arquivoSaida, pNodoA *ArvoreGeral)
{
    char termo[LEN], parametro1[LEN], parametro2[LEN]; // le termo do arquivo
    int i, quantidade; // indice da string
    char operacao; //Salva qual a operacao ira ser executada a seguir

    //Gera as arvores organizadas por ordem de que termos/pesquisas aparecem mais
    nodoConsulta *globalReestruturadaCons = NULL; // nova árvore de consultas global organizada por quantidade de pesquisas
    nodoTermo *globalReestruturadaTerm = NULL; // nova árvore de termos global organizada por quantidade de pesquisas
    globalReestruturadaCons = reestruturaConsultaQuantidade(ArvoreGeral->consultas, globalReestruturadaCons);
    globalReestruturadaTerm = reestruturaTermoQuantidade(ArvoreGeral->termos, globalReestruturadaTerm);


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
                //operacaoA(arquivoSaida, ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao A e escreve o output no txt de saida
                break;
            case 'b':
                //operacaoB(arquivoSaida, globalReestruturadaCons ,quantidade); //Funcao que realiza a operacao B e escreve o output no txt de saida
                break;
            case 'c':
                //operacaoC(arquivoSaida, ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao C e escreve o output no txt de saida
                break;
            case 'd':
                //operacaoD(arquivoSaida, globalReestruturadaTerm , quantidade); //Funcao que realiza a operacao D e escreve o output no txt de saida
                break;
            case 'e':
                //operacaoE(arquivoSaida, ArvoreGeral , parametro1); //Funcao que realiza a operacao E e escreve o output no txt de saida
                break;
            case 'f':
                //operacaoF(arquivoSaida, ArvoreGeral); //Funcao que realiza a operacao F e escreve o output no txt de saida
                break;
        }

    }
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
        // se a consulta nova vier antes na ordem alfabética, coloca ela à direita
        if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info é a precedente
            consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);

        // caso contrário, insere à esquerda
        else
            consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);
    }


    // se não tiver a mesma quantidade, é uma consulta diferente, logo faz os testes para ver se vai para a direita ou esquerda:
    else if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info é a precedente
            consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);

    // caso contrário, insere à esquerda
    else
        consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);

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
    if(strcmp(termo, stringPrecedente)) //se o termo vier depois na ordem alfabética, insere à esquerda
        termoNodo->esq = insereTermo(termoNodo->esq, termo);

    //caso contrário insere à direita
    else
        termoNodo->dir = insereTermo(termoNodo->dir, termo);


    return termoNodo;
}


nodoConsulta* reestruturaConsultaQuantidade(nodoConsulta *arvore, nodoConsulta **novaArvore)
// recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
// devolve a nova árvore
{

    if (arvore == NULL)
        return 0; // critério de parada da recursão

    // a inserção utiliza o central direita (pois assim está organizado pelo critério alfabético)
    reestruturaConsultaQuantidade(arvore->dir, novaArvore);
    insereConsultaQuantidade(*novaArvore, arvore); //arvore nesse ponto se entende como folha
    reestruturaConsultaQuantidade(arvore->esq, novaArvore);
    //novaArvore = insereConsultaQuantidade(novaArvore, arvore); //arvore nesse ponto se entende como folha

    //free(arvore); // libera o espaço ocupado por aquela folha

    //return novaArvore;
    return *novaArvore;

}

nodoConsulta* insereConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo)
// insere um nodo numa árvore de consultas de acordo com a quantidade destas (ou ordem alfabética)
{

    if(arvore == NULL) // se chegamos na extremidade da árvore, adicionamos um novo nodo
    {
        arvore = (nodoConsulta*) malloc(sizeof(nodoConsulta)); // cria nova folha

        arvore->infoLSE = novo->infoLSE;
        arvore->numTermos = novo->numTermos;
        arvore->quantidade = novo->quantidade;
        arvore->dir = NULL; // garante que não haja confusão de ficar linkado com a árvore anterior
        arvore->esq = NULL;

        return arvore;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereConsultaQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereConsultaQuantidade(arvore->esq, novo);

    else // se tiver a mesma quantia de consultas, utiliza critério alfabético
    // como a outra árvore já está ordenada de forma alfabética, é só garantir que a inserção ocorre de maneira central direita
        arvore->esq = insereConsultaQuantidade(arvore->esq, novo);

    return arvore;
}


//nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo *novaArvore)
nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo **novaArvore)
// recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
// devolve a nova árvore
{

    if (arvore == NULL)
        return NULL; // critério de parada da recursão

    // a inserção utiliza o pós-fixado esquerda (pois assim está organizado pelo critério alfabético)
    reestruturaTermoQuantidade(arvore->dir, novaArvore);
    //insereTermoQuantidade(novaArvore, arvore); //arvore agora se entende como folha
    insereTermoQuantidade(*novaArvore, arvore); //arvore agora se entende como folha
    reestruturaTermoQuantidade(arvore->esq, novaArvore);



    return *novaArvore;
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
        arvore = novoTermo;

        return arvore;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereTermoQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereTermoQuantidade(arvore->esq, novo);

    else // caso contrário utiliza o critério de ordenamento alfabético
    // como a outra árvore já está ordenada de forma alfabética, é só garantir que a inserção ocorre de maneira central doreita
        arvore->esq = insereTermoQuantidade(arvore->esq, novo); // entao pela recusao tem que inserir a esquerda ja que vem depois

    //free(novo); // libera espaco da arvore antiga
    return arvore;

}

//Funcao que realiza a operacao A (Listar consultas mais realizadas por localidade) porem nao retorna nada, apenas escreve no arquivo
void operacaoA(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade, int quantidade)
{
    nodoConsulta *arvoreQuantidade = (nodoConsulta*) malloc(sizeof(nodoConsulta)); //Arvore de consultas organizados por quantidade de vezes que aparecem

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe");
    }
    //Encontrou a localidade certa na arvore geral
    else if(strcmp(arvoreGeral->info, localidade) == 0)
    {

        // recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
        // devolve a nova árvore
        arvoreQuantidade = reestruturaConsultaQuantidade(arvoreGeral->consultas, arvoreQuantidade); //ArvoreQuantidade agora tem uma arvore reestruturada por quais consultas aparecem mais

        if(quantidade == 0)
        {
            //Imprime todas as consultas
        }
        else
        {
            while(quantidade != 0) //Imprime os mais pesquisados e vai decrementando a var quantidade
            {
                //Vai imprimindo
                quantidade--;   //Decrementa o indice quantidade
            }
        }
    }
    else if(strcmp(arvoreGeral->info, localidade) > 0) //Se arvoreGeral->info > localidade
    {
        operacaoA(arquivoSaida, arvoreGeral->dir, localidade, quantidade);
    }
    else if(strcmp(arvoreGeral->info, localidade) < 0) //Se arvoreGeral->info < localidade
    {
        operacaoA(arquivoSaida, arvoreGeral->esq, localidade, quantidade);
    }
}

//Funcao que realiza a operacao B (Listar as consultas mais realizadas no arquivo)
void operacaoB(FILE *arquivoSaida, nodoConsulta *arvoreGeral , int quantidade)
{
    if (quantidade == 0)
    {
        //Listar todas as consultas e escrever no arquivo
    }
    else
    {
        while(quantidade > 0)
        {
            //Vai procurando as consultas mais pesquisadas e imprimindo no arquivo
            quantidade--;
        }
    }
}

//Funcao que realiza a operacao C (Listar termos mais consultados por localidade) porem nao retorna nada, apenas escreve no arquivo
void operacaoC(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade, int quantidade)
{
    nodoTermo *arvoreQuantidade = (nodoTermo*) malloc(sizeof(nodoTermo)); //Arvore de consultas organizados por quantidade de vezes que aparecem

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe");
    }
    //Encontrou a localidade certa na arvore geral
    else if(strcmp(arvoreGeral->info, localidade) == 0)
    {

        // recebe uma árvore para reestruturar e uma nova árvore a qual será reestruturada
        // devolve a nova árvore
        arvoreQuantidade = reestruturaTermoQuantidade(arvoreGeral->termos, arvoreQuantidade); //ArvoreQuantidade agora tem uma arvore reestruturada por quais termos aparecem mais

        if(quantidade == 0)
        {
            //Imprime todos os termos
        }
        else
        {
            while(quantidade != 0) //Imprime os mais pesquisados e vai decrementando a var quantidade
            {
                //Vai imprimindo
                quantidade--;   //Decrementa o indice quantidade
            }
        }
    }
    else if(strcmp(arvoreGeral->info, localidade) > 0) //Se arvoreGeral->info > localidade
    {
        operacaoC(arquivoSaida, arvoreGeral->dir, localidade, quantidade);
    }
    else if(strcmp(arvoreGeral->info, localidade) < 0) //Se arvoreGeral->info < localidade
    {
        operacaoC(arquivoSaida, arvoreGeral->esq, localidade, quantidade);
    }
}

//Funcao que realiza a operacao D (Listar os termos mais consultados no arquivo)
void operacaoD(FILE *arquivoSaida, nodoTermo *arvoreGeral , int quantidade)
{
    if (quantidade == 0)
    {
        //Listar todas as consultas e escrever no arquivo
    }
    else
    {
        while(quantidade > 0)
        {
            //Vai procurando as consultas mais pesquisadas e imprimindo no arquivo
            quantidade--;
        }
    }
}

//Funcao que realiza a operacao E (Listar o tamanho medio das consultas por localidade)
void operacaoE(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade)
{
    int quantidadeNodo; //Variavel que salva quantos termos tem no nodo atual, essa var vai ser atualizada a cada nodo lido da arvore
    int quantidadeConsultas; //conta quantas consultas tem, incrementa em um a cada consulta
    int somaTermos; //vai adicionando o numero total de termos de cada consulta nessa variavel
    int MediaTermos; //Apos varrer todas as consultas da localidade, essa variavel recebe soma/quantidade

    nodoConsulta *arvoreConsultas;

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe");
    }
    else if(strcmp(arvoreGeral->info, localidade) == 0) //Encontrou a localidade certa na arvore
    {
        arvoreConsultas = arvoreGeral->consultas;
        //Criar um algoritmo para percorrer a arvore inteiramente
        //A cada nodo da arvore, percorre a lista ate o final somando os seus termos a variavel quantidadeNodo
        //PARA CADA NODO {
            somaTermos += quantidadeNodo;
            quantidadeConsultas++;
        //}
        MediaTermos = somaTermos/quantidadeConsultas; //Calculando como int para o truncamento ser feito automaticamente

        //Imprimir no arquivo o MediaTermos
    }
    else if(strcmp(arvoreGeral->info, localidade) > 0) //Se arvoreGeral->info > localidade
    {
        operacaoE(arquivoSaida, arvoreGeral->dir, localidade);
    }
    else if(strcmp(arvoreGeral->info, localidade) < 0) //Se arvoreGeral->info < localidade
    {
        operacaoE(arquivoSaida, arvoreGeral->esq, localidade);
    }
}

//Funcao que realiza a operacao F (Listar tamanho medio das consultas de todo o arquivo)
void operacaoF(FILE *arquivoSaida, pNodoA *arvoreGeral)
{
    int quantidadeNodo; //Variavel que salva quantos termos tem no nodo atual, essa var vai ser atualizada a cada nodo lido da arvore
    int quantidadeConsultas; //conta quantas consultas tem, incrementa em um a cada consulta
    int somaTermos; //vai adicionando o numero total de termos de cada consulta nessa variavel
    int MediaTermos; //Apos varrer todas as consultas da localidade, essa variavel recebe soma/quantidade

    nodoConsulta *arvoreConsulta;
    arvoreConsulta = arvoreGeral->consultas;    //Recebe a arvore de consultas globais

    //Criar um algoritmo para percorrer a arvore inteiramente
        //A cada nodo da arvore, percorre a lista ate o final somando os seus termos a variavel quantidadeNodo
        //PARA CADA NODO {
            somaTermos += quantidadeNodo;
            quantidadeConsultas++;
        //}
        MediaTermos = somaTermos/quantidadeConsultas; //Calculando como int para o truncamento ser feito automaticamente
}


void imprimeArvore(pNodoA *a)
{
    if (a == NULL)
        return 0;

    imprimeArvore(a->dir);

    printf("Localidade: %s \n\nTermos:\n", a->info);
        //imprimeTermos(a->termos);
    printf("\nConsultas:\n");
        //imprimeConsultas(a->consultas);
    printf("\n\n\n");

    imprimeArvore(a->esq);



    //getchar();

}

void imprimeConsultas(nodoConsulta *a)
{
    if (a == NULL)
        return 0;

    imprimeConsultas(a->dir);

    itemA *aux = a->infoLSE;

    printf("\nQuantidade: %d | ", a->quantidade);
    while (aux != NULL)
    {
        printf("%s - ", aux->info);
        aux = aux->prox;

    }

    imprimeConsultas(a->esq);

    //printf("\n");

}

void imprimeTermos(nodoTermo *a)
{
    if (a == NULL)
        return 0;

    imprimeTermos(a->dir);
    printf("Quantidade: %d | %s\n", a->quantidade, a->info);
    imprimeTermos(a->esq);


}

