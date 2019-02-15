#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#include "utilidades.h"
#define LEN 80 // define o tamanho aceito para cada string

// estrutura que define cada nodo da árvore de termos
struct NodoTermo{
    char info[LEN]; // string do termo
    int quantidade; // quantidade de vezes que o termo foi pesquisado
    struct NodoTermo *esq;
    struct NodoTermo *dir;
};
typedef struct NodoTermo nodoTermo;



// estrutura que define cada nodo da árvore de consultas
struct NodoConsulta{
    itemA *infoLSE; // lista de termos
    int quantidade; // quantidade de vezes que a consulta foi pesquisada
    int numTermos; // quantidade de termos que a consulta tem
    struct NodoConsulta *esq;
    struct NodoConsulta *dir;
};
typedef struct NodoConsulta nodoConsulta;



// estrutura que define cada nodo da árvore de localidades
struct TNodoA{
        char info[LEN]; // string buscada ou nome da cidade
        struct NodoConsulta *consultas; // árvore de consultas daquela localidade
        struct NodoTermo *termos; // árvore de termos daquela localidade
        struct TNodoA *esq;
        struct TNodoA *dir;
};
typedef struct TNodoA pNodoA;



pNodoA* leArquivo(FILE *arquivo); // a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo

// operações iniciais para construir a árvore
pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA **localidadeAtual, char info[LEN]); // funcao que insere uma localidade na arvore
nodoConsulta* insereConsulta(nodoConsulta *consultas, itemA *consulta, int quantidadeDeTermos, int flag); // insere uma consulta na árvore de consultas
nodoTermo* insereTermo(nodoTermo *termoNodo, char termo[LEN/2]);  // insere termo na árvore de termos

// operações para transformar a árvore numa abp ordenada por mais pesquisados
nodoConsulta* reestruturaConsultaQuantidade(nodoConsulta *arvore, nodoConsulta **novo); // reestrutura árvore inicialmente ordenada em ordem alfabética, para ordem de consultas mais pesquisadas, utilizando alfabética como critério de desempate
nodoConsulta* insereConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo); //insere uma consulta usando o critério da função acima
nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo **novo); // reestrutura árvore inicialmente ordenada em ordem alfabética, para ordem de termos mais pesquisados, utilizando alfabética como critério de desempate
nodoTermo* insereTermoQuantidade(nodoTermo *arvore, nodoTermo *novo); // //insere um termo usando o critério da função acima

// visualização
void imprimeArvore(pNodoA *a);
void imprimeConsultas(nodoConsulta *a);
void imprimeTermos(nodoTermo *a);
void imprimeCidades(pNodoA *a);

//trata operacoes
void leOperacoes(FILE *arquivoOp,FILE *arquivoSaida, pNodoA *ArvoreGeral);
void operacaoA(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80], int quantidade);
void operacaoB(FILE *arquivoSaida, nodoConsulta *arvoreGeral , int quantidade);
void operacaoC(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80], int quantidade);
void operacaoD(FILE *arquivoSaida, nodoTermo *arvoreGeral , int quantidade);
void operacaoE(FILE *arquivoSaida, pNodoA *arvoreGeral , char localidade[80]);
void operacaoF(FILE *arquivoSaida, pNodoA *arvoreGeral);

//imprime no arquivo (funcoes que sao chamadas pelas funcoes de operacao)
void imprimeConsultasArquivo(FILE *arquivoSaida, nodoConsulta *a, int *quantidade);
void imprimeConsultasArquivoTodo(FILE *arquivoSaida, nodoConsulta *a);
void imprimeTermosArquivo(FILE *arquivoSaida, nodoTermo *a, int *quantidade);
void imprimeTermosArquivoTodo(FILE *arquivoSaida, nodoTermo *a);
void calculaTamanhoMedio(nodoConsulta *a, int *quantidadeConsultas, int *somaTermos);





#endif // ARVORE_H_INCLUDED
