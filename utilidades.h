#ifndef UTILIDADES_H_INCLUDED
#define UTILIDADES_H_INCLUDED

#define LEN 80 // define o tamanho aceito para cada string

// estrutura que define cada item da lista simplesmente encadeada de uma consulta composta por mais de um termo
struct ItemLista{
        char info[LEN]; // string com o termo
        struct ItemLista *prox; // pr�ximo da lista
};

typedef struct ItemLista itemA;

int verificaComposta(char stringAvaliada[LEN]);
itemA* criaConsultaSimples(char consulta[LEN]);
itemA* separaTermos(char consulta[LEN], int *quantidadeDeTermos);
itemA* insereLista(itemA *lista, char termo[LEN/2]);
void printaLista(itemA *lista);
void ordemAlfabetica(char string1[LEN], char string2[LEN], char *stringAlterada);
int testaMesmaConsulta(itemA *consulta1, itemA *consulta2);
int testaMesmaConsulta2(char termo[LEN/2], itemA *consulta);

#endif // UTILIDADES_H_INCLUDED
