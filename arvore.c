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
    nodoConsulta *consultas = NULL; // �rvore de consultas
    nodoTermo *termos = NULL; // �rvore de termos


    pNodoA *arvoreFinal = (pNodoA*) malloc(sizeof(pNodoA)); // �rvore com as localidades

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
                i++;




        if (flagCidade) // se leu uma cidade
        {
            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo
            flagCidade = 0; // seta a flag de cidade para zero
            //chama a funcao que insere cidade
            arvoreFinal = insereLocalidade(arvoreFinal, &localidade, pesquisa);
        }



        else // se n�o for uma cidade
        {

            if(pesquisa[i] == '\n') // se chegou no fim da linha
                flagCidade = 1; // seta a flag pra 1 novamente pois a proxima leitura sera de uma cidade


            pesquisa[i] = '\0'; //Coloca o sinal de final de string no final da pesquisa atual, isso eh feito aqui dentro pois precisamos testar o ultimo caracter antes de altera-lo




            //inser��o das consultas

            if(verificaComposta(pesquisa)) //checa se � uma pesquisa composta
                consultaLSE = separaTermos(pesquisa, &quantidadeDeTermos); // separa os termos


            //se nao, cria consulta simples e seta a quantidade de termos para 1
            else
            {
                consultaLSE = criaConsultaSimples(pesquisa); // cria consulta simples (lista de um elemento)
                quantidadeDeTermos = 1;
            }


            arvoreFinal->consultas = insereConsulta(arvoreFinal->consultas, consultaLSE, quantidadeDeTermos); //insere a consulta na �rvore global
            localidade->consultas = insereConsulta(localidade->consultas, consultaLSE, quantidadeDeTermos); //insere a consulta na localidade atual



            quantidadeDeTermos = 0; // depois de inserida a consulta, zera a quantidade de termos da consulta (reutiliza��o da var)



            // inser��o de termos
            while (consultaLSE != NULL)
            {
                // insere termo atual na �rvore de termos da localidade atual
                localidade->termos = insereTermo(localidade->termos, consultaLSE->info);
                // insere termo atual na �rvore de termos globais
                arvoreFinal->termos = insereTermo(arvoreFinal->termos, consultaLSE->info);

                //avan�a para pr�ximo termo
                consultaLSE = consultaLSE->prox;
            }
            // aqui consultaLSE j� valer� NULL e poder� ser reutilizada
        }
    }


    nodoConsulta *globalReestruturadaCons = (nodoConsulta*) malloc(sizeof(nodoConsulta)); // nova �rvore de consultas global organizada por quantidade de pesquisas
    nodoTermo *globalReestruturadaTerm = (nodoTermo*) malloc(sizeof(nodoTermo)); // nova �rvore de termos global organizada por quantidade de pesquisas


    // chama as fun��es que reestruturam a �rvore de acordo com a quantidade de pesquisas

    globalReestruturadaCons = NULL;
    globalReestruturadaTerm = NULL;

    reestruturaConsultaQuantidade(arvoreFinal->consultas, &globalReestruturadaCons);
    reestruturaTermoQuantidade(arvoreFinal->termos, &globalReestruturadaTerm);


    printf("Tempo:%f\n\n", (clock() - tempo) / (double)CLOCKS_PER_SEC);

    /*imprimeConsultas(arvoreFinal->consultas);
    getchar();
    imprimeConsultas(globalReestruturadaCons);
    getchar();*/



    imprimeTermos(arvoreFinal->termos);
    getchar();
    imprimeTermos(globalReestruturadaTerm);
    getchar();


    return arvoreFinal;
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

    else if (strcmp(info, inicioArvore->info) == 0) // se a string da pesquisa for igual a info do nodo atual (aquela localidade j� estpa na �rvore)
    {

        localidadeAtual = inicioArvore; // atualiza endere�o de localidade atual
        return inicioArvore; // retorna �rvore id�ntica
    }

    char stringPrecedente[LEN];
    ordemAlfabetica(info, inicioArvore->info, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    //se info, pela ordem alfabetica, vem antes, vai pra direita
    if (strcmp(info, stringPrecedente) == 0)
        inicioArvore->dir = insereLocalidade(inicioArvore->dir, localidadeAtual, info);

    else // se n�o, vai pra esquerda
    {
        inicioArvore->esq = insereLocalidade(inicioArvore->esq, localidadeAtual, info);
    }

    return inicioArvore;
}

//Recebe o arquivo das operacoes e a arvore que contem as localidades
//Retorna void porem escreve no arquivo de saida
void leOperacoes(FILE *arquivoOp, FILE *arquivoSaida, pNodoA *ArvoreGeral)
{
    char termo[LEN], parametro1[LEN], parametro2[LEN]; // le termo do arquivo
    int i, quantidade; // indice da string
    char operacao; //Salva qual a operacao ira ser executada a seguir

    //Gera as arvores organizadas por ordem de que termos/pesquisas aparecem mais
    nodoConsulta *globalReestruturadaCons = NULL;//(nodoConsulta*) malloc(sizeof(nodoConsulta)); // nova �rvore de consultas global organizada por quantidade de pesquisas
    nodoTermo *globalReestruturadaTerm = NULL;//(nodoTermo*) malloc(sizeof(nodoTermo)); // nova �rvore de termos global organizada por quantidade de pesquisas
    reestruturaConsultaQuantidade(ArvoreGeral->consultas, &globalReestruturadaCons);
    reestruturaTermoQuantidade(ArvoreGeral->termos, &globalReestruturadaTerm);


    while((termo[0] = getc(arquivoOp)) != EOF) //Loop ate o arquivo acabar
    {
        operacao = termo[0]; //O primeiro char de cada linha a ser lido eh a operacao

        i = 0; //resetando o indice que le os parametros

        //Leitura do segundo termo da linha (Ou a localidade ou a quantidade, dependendo da operacao)
        if(operacao != 'f') //Se a operacao for f nao precisa de mais nenhum termo
        {
            getc(arquivoOp);
            while ( (parametro1[i] = getc(arquivoOp)) != ';' &&     //Percorre o arquivo ate um nova linha ou um ;
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

            while ( (parametro2[i] = getc(arquivoOp)) != ';' &&     //Percorre o arquivo ate um nova linha ou um ;
                    parametro2[i] != '\n')
            {
                i++;
            }

            quantidade = atoi(parametro2); //Converte a quantidade para uma int
        }

        //Decide qual funcao de operacao deve ser chamada
        switch(operacao)
        {
            case 'a':
                operacaoA(arquivoSaida, ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao A e escreve o output no txt de saida
                break;
            case 'b':
                operacaoB(arquivoSaida, globalReestruturadaCons ,quantidade); //Funcao que realiza a operacao B e escreve o output no txt de saida
                break;
            case 'c':
                operacaoC(arquivoSaida, ArvoreGeral , parametro1, quantidade); //Funcao que realiza a operacao C e escreve o output no txt de saida
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
        itemA *consultaAux = consulta; // pois consulta ser� alterada na fun��o que insere termos e a lista, perdida
        nodoConsulta *novaCons = (nodoConsulta*) malloc(sizeof(nodoConsulta));

        novaCons->infoLSE = consultaAux;
        novaCons->dir = NULL;
        novaCons->esq = NULL;
        novaCons->numTermos = quantidadeDeTermos;
        novaCons->quantidade = 1;


        return novaCons;
    }

    char stringPrecedente[LEN]; //vari�vel auxiliar pra testar pra que lado vai a consulta na �rvore
    ordemAlfabetica(consulta->info, consultas->infoLSE->info, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    // testa se tem a mesma quantidade de termos
    if (consultas->numTermos == quantidadeDeTermos) // se a string da pesquisa for igual a info do nodo atual (aquela localidade j� estpa na �rvore)
    {
        if (testaMesmaConsulta(consulta, consultas->infoLSE)) //testa se � a mesma consulta
        {
            consultas->quantidade++; // aumenta a quantidade de vezes que aquela consulta foi feita
            return consultas; // e retorna a recurs�o
        }

        // caso contr�rio, verifica pra onde ir (direita ou esquerda da consulta atual)
        // se a consulta nova vier antes na ordem alfab�tica, coloca ela � direita
        if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info � a precedente
            consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);

        // caso contr�rio, insere � esquerda
        else
            consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);
    }


    // se n�o tiver a mesma quantidade, � uma consulta diferente, logo faz os testes para ver se vai para a direita ou esquerda:
    else if(!strcmp(stringPrecedente, consulta->info)) //se for igual, strcmp retorna 0, logo !0 para saber se consulta->info � a precedente
            consultas->dir = insereConsulta(consultas->dir, consulta, quantidadeDeTermos);

    // caso contr�rio, insere � esquerda
    else
        consultas->esq = insereConsulta(consultas->esq, consulta, quantidadeDeTermos);

    return consultas;
}



nodoTermo* insereTermo(nodoTermo *termoNodo, char termo[LEN/2])
//  funcao que insere termo na �rvore de termos
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
        termoNodo->quantidade++; // s� aumenta a quantidade de vezes que aquele termo foi pesquisado
        return termoNodo;
    }

    char stringPrecedente[LEN]; //vari�vel auxiliar pra testar pra que lado vai a consulta na �rvore
    ordemAlfabetica(termoNodo->info, termo, stringPrecedente); //assinala em stringPrecedente a precedente em ordem alfabetica

    // testa ordem alfab�tica para saber se insere � direita ou esquerda
    if(strcmp(termo, stringPrecedente)) //se o termo vier depois na ordem alfab�tica, insere � esquerda
        termoNodo->esq = insereTermo(termoNodo->esq, termo);

    //caso contr�rio insere � direita
    else
        termoNodo->dir = insereTermo(termoNodo->dir, termo);


    return termoNodo;
}


nodoConsulta* reestruturaConsultaQuantidade(nodoConsulta *arvore, nodoConsulta **novaArvore)
// recebe uma �rvore para reestruturar e uma nova �rvore a qual ser� reestruturada
// devolve a nova �rvore
{

    if (arvore == NULL)
        return 0; // crit�rio de parada da recurs�o

    // a inser��o utiliza o central direita (pois assim est� organizado pelo crit�rio alfab�tico)
    reestruturaConsultaQuantidade(arvore->dir, novaArvore);
    *novaArvore = insereConsultaQuantidade(*novaArvore, arvore); //arvore nesse ponto se entende como folha
    reestruturaConsultaQuantidade(arvore->esq, novaArvore);


    return *novaArvore;

}

nodoConsulta* insereConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo)
// insere um nodo numa �rvore de consultas de acordo com a quantidade destas (ou ordem alfab�tica)
{

    if(arvore == NULL) // se chegamos na extremidade da �rvore, adicionamos um novo nodo
    {

        arvore = malloc(sizeof(nodoConsulta)); // cria nova folha

        arvore->infoLSE = novo->infoLSE;
        arvore->numTermos = novo->numTermos;
        arvore->quantidade = novo->quantidade;
        arvore->dir = NULL; // garante que n�o haja confus�o de ficar linkado com a �rvore anterior
        arvore->esq = NULL;

        return arvore;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereConsultaQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereConsultaQuantidade(arvore->esq, novo);

    else // se tiver a mesma quantia de consultas, utiliza crit�rio alfab�tico
    // como a outra �rvore j� est� ordenada de forma alfab�tica, � s� garantir que a inser��o ocorre de maneira central direita
        arvore->esq = insereConsultaQuantidade(arvore->esq, novo);

    return arvore;
}


nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo **novaArvore)
// recebe uma �rvore para reestruturar e uma nova �rvore a qual ser� reestruturada
// devolve a nova �rvore
{

    if (arvore == NULL)
        return 0; // crit�rio de parada da recurs�o

    // a inser��o utiliza o p�s-fixado esquerda (pois assim est� organizado pelo crit�rio alfab�tico)
    reestruturaTermoQuantidade(arvore->dir, novaArvore);
    *novaArvore = insereTermoQuantidade(*novaArvore, arvore); //arvore agora se entende como folha
    reestruturaTermoQuantidade(arvore->esq, novaArvore);



    return *novaArvore;
}


nodoTermo* insereTermoQuantidade(nodoTermo *arvore, nodoTermo *novo)
// insere o nodo novo em uma �rvore de termos
{

    if(arvore == NULL) // se chegamos na extremidade da �rvore, adicionamos um novo nodo
    {
        arvore = (nodoTermo*) malloc(sizeof(nodoTermo));

        strcpy(arvore->info, novo->info);
        arvore->quantidade = novo->quantidade;
        arvore->dir = NULL; // garante que n�o haja confus�o de ficar linkado com a �rvore anterior
        arvore->esq = NULL;

        return arvore;
    }

    else if (novo->quantidade > arvore->quantidade) // se o nodo atual tem mais consultas, vai para a direita
        arvore->dir = insereTermoQuantidade(arvore->dir, novo);

    else if (novo->quantidade < arvore->quantidade) // se o nodo atual tem menos consultas, vai para a esquerda
        arvore->esq = insereTermoQuantidade(arvore->esq, novo);

    else // caso contr�rio utiliza o crit�rio de ordenamento alfab�tico
    // como a outra �rvore j� est� ordenada de forma alfab�tica, � s� garantir que a inser��o ocorre de maneira central doreita
        arvore->esq = insereTermoQuantidade(arvore->esq, novo); // entao pela recusao tem que inserir a esquerda ja que vem depois


    return arvore;

}

//Funcao que realiza a operacao A (Listar consultas mais realizadas por localidade) porem nao retorna nada, apenas escreve no arquivo
void operacaoA(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80], int quantidade)
{
    nodoConsulta *arvoreQuantidade = NULL;//(nodoConsulta*) malloc(sizeof(nodoConsulta)); //Arvore de consultas organizados por quantidade de vezes que aparecem

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe");
    }
    //Encontrou a localidade certa na arvore geral
    else if(strcmp(arvoreGeral->info, localidade) == 0)
    {
        // recebe uma �rvore para reestruturar e uma nova �rvore a qual ser� reestruturada
        // devolve a nova �rvore
        nodoConsulta *globalReestruturadaCons = NULL;
        arvoreQuantidade = reestruturaConsultaQuantidade(arvoreGeral->consultas, &globalReestruturadaCons); //ArvoreQuantidade agora tem uma arvore reestruturada por quais consultas aparecem mais

        if(quantidade == 0)
        {
            imprimeConsultasArquivoTodo(arquivoSaida, arvoreQuantidade);
        }
        else
        {
            imprimeConsultasArquivo(arquivoSaida, arvoreQuantidade, &quantidade);
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
        imprimeConsultasArquivoTodo(arquivoSaida, arvoreGeral);
    }
    else
    {
        while(quantidade > 0)
        {
            imprimeConsultasArquivo(arquivoSaida, arvoreGeral, &quantidade);
        }
    }
}

//Funcao que realiza a operacao C (Listar termos mais consultados por localidade) porem nao retorna nada, apenas escreve no arquivo
void operacaoC(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80], int quantidade)
{
    nodoTermo *arvoreQuantidade = NULL; //Arvore de consultas organizados por quantidade de vezes que aparecem

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe\n");
    }
    //Encontrou a localidade certa na arvore geral
    else if(strcmp(arvoreGeral->info, localidade) == 0)
    {
        // recebe uma �rvore para reestruturar e uma nova �rvore a qual ser� reestruturada
        // devolve a nova �rvore
        nodoConsulta *globalReestruturadaTerm = NULL;
        arvoreQuantidade = reestruturaTermoQuantidade(arvoreGeral->termos, &globalReestruturadaTerm); //ArvoreQuantidade agora tem uma arvore reestruturada por quais consultas aparecem mais

        if(quantidade == 0)
        {
            imprimeTermosArquivoTodo(arquivoSaida, arvoreQuantidade);
        }
        else
        {
            imprimeTermosArquivo(arquivoSaida, arvoreQuantidade, &quantidade);
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
        imprimeTermosArquivoTodo(arquivoSaida, arvoreGeral);
    }
    else
    {
            imprimeTermosArquivo(arquivoSaida, arvoreGeral, &quantidade);
    }
}

//Funcao que realiza a operacao E (Listar o tamanho medio das consultas por localidade)
void operacaoE(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80])
{
    int MediaTermos; //Apos varrer todas as consultas da localidade, essa variavel recebe soma/quantidade
    //int quantidadeNodo; //Variavel que salva quantos termos tem no nodo atual, essa var vai ser atualizada a cada nodo lido da arvore
    int quantidadeConsultas = 0; //conta quantas consultas tem, incrementa em um a cada consulta
    int somaTermos = 0; //vai adicionando o numero total de termos de cada consulta nessa variavel

    nodoConsulta *arvoreConsultas;

    if(arvoreGeral == NULL) //Se achou algum NULL quer dizer que nao existe a localidade desejada
    {
        //Imprimir no arquivo que a localidade inserida nao existe
        fprintf(arquivoSaida, "A localidade inserida nao existe\n");
    }
    else if(strcmp(arvoreGeral->info, localidade) == 0) //Encontrou a localidade certa na arvore
    {
        arvoreConsultas = arvoreGeral->consultas;
        calculaTamanhoMedio(arvoreConsultas, quantidadeConsultas, somaTermos);
        MediaTermos = somaTermos/quantidadeConsultas; //Calculando como int para o truncamento ser feito automaticamente

        //Imprime no arquivo a media dos termos
        fprintf(arquivoSaida, "Media de termos/pesquisa: %d", MediaTermos);
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

    calculaTamanhoMedio(arvoreConsulta, quantidadeConsultas, somaTermos);
    MediaTermos = somaTermos/quantidadeConsultas; //Calculando como int para o truncamento ser feito automaticamente

    fprintf(arquivoSaida, "Media de termos/pesquisa: %d", MediaTermos);
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

//Imprime o numero especifico de consultas no arquivo
void imprimeConsultasArquivo(FILE *arquivoSaida, nodoConsulta *a, int *quantidade)
{
    if (a == NULL)
        return 0;

    imprimeConsultasArquivo(arquivoSaida, a->dir, quantidade);

    itemA *aux = a->infoLSE;

    if(*quantidade > 0) //Imprime apenas a quantidade desejada (talvez precise adicionar um * aqui)
    {
        fprintf(arquivoSaida, "Quantidade: %d | ", a->quantidade);
        while (aux != NULL)
        {
            fprintf(arquivoSaida, "%s - ", aux->info);
            aux = aux->prox;

        }
        *quantidade = *quantidade - 1;

        fprintf(arquivoSaida, "\n");
    }

    imprimeConsultasArquivo(arquivoSaida, a->esq, quantidade);
}

//Imprime todas as consultas do arquivo
void imprimeConsultasArquivoTodo(FILE *arquivoSaida, nodoConsulta *a)
{
    if (a == NULL)
        return 0;

    imprimeConsultasArquivoTodo(arquivoSaida, a->esq);

    itemA *aux = a->infoLSE;

    fprintf(arquivoSaida, "Quantidade: %d | ", a->quantidade);
    while (aux != NULL)
    {
        fprintf(arquivoSaida, "%s - ", aux->info);
        aux = aux->prox;

    }

   fprintf(arquivoSaida, "\n");

    imprimeConsultasArquivoTodo(arquivoSaida, a->dir);
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

//Imprime todos os termos no arquivo
void imprimeTermosArquivoTodo(FILE *arquivoSaida, nodoTermo *a)
{
    if (a == NULL)
        return 0;

    imprimeTermosArquivoTodo(arquivoSaida, a->esq);

    fprintf(arquivoSaida, "Quantidade: %d | %s\n", a->quantidade, a->info);

    imprimeTermosArquivoTodo(arquivoSaida, a->dir);
}

//Imprime o numero especifico de termos no arquivo
void imprimeTermosArquivo(FILE *arquivoSaida, nodoTermo *a, int *quantidade)
{
    if (a == NULL)
        return 0;

    imprimeTermosArquivo(arquivoSaida, a->dir, quantidade);

    if (*quantidade > 0)
    {
        fprintf(arquivoSaida, "Quantidade: %d | %s\n", a->quantidade, a->info);
        *quantidade = *quantidade - 1;
    }

    imprimeTermosArquivo(arquivoSaida, a->esq, quantidade);
}

void imprimeTermos(nodoTermo *a)
{
    if (a == NULL)
        return 0;

    imprimeTermos(a->dir);
    printf("Quantidade: %d | %s\n", a->quantidade, a->info);
    imprimeTermos(a->esq);


}

//Calcula o tamanho medio das consultas de uma arvore
void calculaTamanhoMedio(nodoConsulta *a, int *quantidadeConsultas, int *somaTermos)
{
    if (a == NULL)
        return 0;

    calculaTamanhoMedio(a->esq, &quantidadeConsultas, &somaTermos);

    itemA *aux = a->infoLSE;

    quantidadeConsultas++;
    somaTermos++; //SOMA UM A QUANTIDADE DE CONSULTAS E TERMOS
    while (aux != NULL)
    {
        somaTermos++; //SOMA UM A QUANTIDADE DE TERMOS
        aux = aux->prox;

    }

    calculaTamanhoMedio(a->dir, &quantidadeConsultas, &somaTermos);
}
