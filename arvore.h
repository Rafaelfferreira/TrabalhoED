#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#include "utilidades.h"
#define LEN 80 // define o tamanho aceito para cada string

// estrutura que define cada nodo da �rvore de termos
struct NodoTermo{
    char info[LEN]; // string do termo
    int quantidade; // quantidade de vezes que o termo foi pesquisado
    struct NodoTermo *esq;
    struct NodoTermo *dir;
};
typedef struct NodoTermo nodoTermo;



// estrutura que define cada nodo da �rvore de consultas
struct NodoConsulta{
    itemA *infoLSE; // lista de termos
    int quantidade; // quantidade de vezes que a consulta foi pesquisada foi pesquisada
    int numTermos; // quantidade de termos que a consulta tem
    struct NodoConsulta *esq;
    struct NodoConsulta *dir;
};
typedef struct NodoConsulta nodoConsulta;



// estrutura que define cada nodo da �rvore de localidades
struct TNodoA{
        char info[LEN]; // string buscada ou nome da cidade
        struct NodoConsulta *consultas; // �rvore de consultas daquela localidade
        struct NodoTermo *termos; // �rvore de termos daquela localidade
        struct TNodoA *esq;
        struct TNodoA *dir;
};
typedef struct TNodoA pNodoA;


// opera��es iniciais para construir a �rvore
pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA **localidadeAtual, char info[LEN]); // funcao que insere uma localidade na arvore
pNodoA* leArquivo(FILE *arquivo); // a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo

void imprimeCidades(pNodoA *a);
nodoConsulta* insereConsulta(nodoConsulta *consultas, itemA *consulta, int quantidadeDeTermos); // insere uma consulta na �rvore de consultas
nodoTermo* insereTermo(nodoTermo *termoNodo, char termo[LEN/2]);  // insere termo na �rvore de termos

// opera��es para transformar a �rvore numa abp ordenada por mais pesquisados
nodoConsulta* reestruturaConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo); // reestrutura �rvore inicialmente ordenada em ordem alfab�tica, para ordem de consultas mais pesquisadas, utilizando alfab�tica como crit�rio de desempate
nodoConsulta* insereConsultaQuantidade(nodoConsulta *arvore, nodoConsulta *novo); //insere uma consulta usando o crit�rio da fun��o acima
nodoTermo* reestruturaTermoQuantidade(nodoTermo *arvore, nodoTermo *novo); // reestrutura �rvore inicialmente ordenada em ordem alfab�tica, para ordem de termos mais pesquisados, utilizando alfab�tica como crit�rio de desempate
nodoTermo* insereTermoQuantidade(nodoTermo *arvore, nodoTermo *novo); // //insere um termo usando o crit�rio da fun��o acima

// visualiza��o
void imprimeArvore(pNodoA *a);
void imprimeConsultas(nodoConsulta *a);
void imprimeTermos(nodoTermo *a);

void leOperacoes(FILE *arquivo, pNodoA *ArvoreGeral);
operacaoA(pNodoA *ArvoreGeral , char Localidade, int quantidade);
operacaoB(pNodoA *ArvoreGeral , int quantidade);
operacaoC(pNodoA *ArvoreGeral , char Localidade, int quantidade);
operacaoD(pNodoA *ArvoreGeral , int quantidade);
operacaoE(pNodoA *ArvoreGeral , char Localidade);
operacaoF(pNodoA *ArvoreGeral);







#endif // ARVORE_H_INCLUDED
