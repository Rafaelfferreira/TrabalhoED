#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

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
    char info[LEN]; // string do termo
    int quantidade; // quantidade de vezes que a consulta foi pesquisada foi pesquisada
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



pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA *localidadeAtual, char info[LEN]); // funcao que insere uma localidade na arvore
pNodoA* leArquivo(FILE *arquivo); // a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo
void imprimeCidades(pNodoA *a);




#endif // ARVORE_H_INCLUDED
