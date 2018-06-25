#ifndef ARVORE_H_INCLUDED
#define ARVORE_H_INCLUDED

#define LEN 80 // define o tamanho aceito para cada string

// estrutura que define cada nodo construindo uma arvore binaria
struct TNodoA{
        char info[LEN]; // string buscada ou nome da cidade
        int pesquisas; // numero de vezes que foi pesquisado; -1 representa uma localidade
        struct TNodoA *esq;
        struct TNodoA *dir;
};

typedef struct TNodoA pNodoA;

pNodoA* insereLocalidade(pNodoA *inicioArvore, pNodoA *localidadeAtual, char info[LEN]); // funcao que insere uma localidade na arvore
pNodoA* leArquivo(FILE *arquivo); // a funcao le o arquivo e retorna (monta) uma arvore de acordo com as informacoes contidas neste arquivo
void imprimeCidades(pNodoA *a);



#endif // ARVORE_H_INCLUDED
