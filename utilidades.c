#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"

#define LEN 80
#define SPACE ' ' // caractere espaço (critério de parada do loop que significa que a string é quebrada)


int verificaComposta(char stringAvaliada[LEN])
// a função verifica se a string passada é uma pesquisa composta, isto é, separada por espaços
// retorna 1 se for e 0 se não for
{
    int i = 0; // contador do índice para percorrer todos os caracteres da string

    while (stringAvaliada[i] != '\0' && stringAvaliada[i] != SPACE) // enquanto não for fim da string nem achar espaço
        i++; // vai analisando próximos caracteres dessa string

    if (stringAvaliada[i] == '\0') //se chegou no final e não encontrou espaços retorna 0
        return 0;

    else // se não é final, é espaço, são os únicos critérios de parada
        return 1;
}


itemA* criaConsultaSimples(char consulta[LEN])
// a função recebe uma string referente a uma consulta de apenas um termo
// devolve uma lista simplesmente encadeada de apenas um termo (para que possa ser usado na árvore das consultas)
{
    itemA *consultaLSE = (itemA*) malloc(sizeof(itemA));

    consultaLSE->prox = NULL; // lista única
    strcpy(consultaLSE->info, consulta); // copia a informação pra lista

    return consultaLSE;
}

itemA* separaTermos(char consulta[LEN], int *quantidadeDeTermos)
// a função recebe uma string (consulta) composta de termos e quebra essa string
// devolve uma lista simplesmente encadeada onde cada nodo da lista representa um termo da consulta
// além disso, altera a quantidade de termos (passada por parâmetro) dessa consulta
{
    itemA *listaDeTermos = NULL; //(itemA*) malloc(sizeof(itemA));
    char temporario[LEN/2]; // variável auxiliar que irá guardar o termo enquanto o while for varrendo
    int i = 0; // contador do índice para percorrer todos os caracteres da string consulta
    int j = 0; // contador do índice para percorrer todos os caracteres da string temporario

    while(consulta[i] != '\0') // enquanto não acabar de percorrer a string
    {
        while(consulta[i] != SPACE && consulta[i] != '\0')
        {
            // vai copiando os caracteres do termo até achar o espaço
            temporario[j] = consulta[i];
            i++;
            j++;
        }

        temporario[j] = '\0'; // encerra string temporario

        if (consulta[i] != '\0')
            i++; // incrementa i para pular o espaço (e não incrementa caso seja o final da string)

        j = 0; // zera j para reaproveitar a variável temporario

        *quantidadeDeTermos = *quantidadeDeTermos + 1; //incrementa conteúdo de quantidadeDeTermos
        listaDeTermos = insereLista(listaDeTermos, temporario); // insere o termo novo na lista
    }

    return listaDeTermos;
}

itemA* insereLista(itemA *lista, char termo[LEN/2]) // também reduz a quantidade de caracteres já que é um único termo
// a função recebe uma lista encadeada simples e insere um nodo novo no início dessa lista
// devolve a nova lista com o nodo inserido
{
    itemA *novoTermo = (itemA*) malloc(sizeof(itemA));
    strcpy(novoTermo->info, termo);
    novoTermo->prox = NULL;

    if (lista == NULL) // se a lista for nula, cria um primeiro elemento com esse termo
        lista = novoTermo; // lista retornada eh o proprio novo item


    else // se nao, insere o termo no proximo item da lista
    {
        itemA *aux = lista; // para andar ate o fim da lista antes de adicionar a consulta
        while (aux->prox != NULL) // enquanto nao for ate o ultimo item da lista
            aux = aux->prox;

        aux->prox = novoTermo; // adiciona o ultimo item no fim da lista
    }

    return lista;
}


void printaLista(itemA *lista)
{
    itemA *aux = lista;
    while(aux != NULL)
    {
        printf("%s ", aux->info);
        aux = aux->prox;
    }
}


itemA* ordemAlfabeticaLSE(itemA *lista1, itemA *lista2)
// a função vai receber duas listas correspondentes a consultas e retornará qual vem primeiro na ordem alfabética
{
    itemA *aux1 = lista1;
    itemA *aux2 = lista2;

    while (aux1 != NULL || aux2 != NULL)
    {
        if (!strcmp(aux1->info, aux2->info)) // se o primeiro termo for igual verifica se alguma acabou (retornando-a), se não compara os próximos dois
        {
            if (aux1->prox == NULL)
                return lista1;

            else if (aux2->prox == NULL)
                return lista2;

            else  // caso contrário, compara os outros termos
            {
                aux1 = aux1->prox;
                aux2 = aux2->prox;
            }
        }

        else if (strcmp(aux1->info, aux2->info) > 0)   // se a string de aux1 vier depois - na ordem alfabética - da string de aux2, retorna aux2 que vem antes
            return aux2;

        else
            return aux1;
    }

    printf("ERRROOOOO NA LOGICA");
}


void ordemAlfabetica(char string1[LEN], char string2[LEN], char *stringAlterada)
// função que compara duas strings e atualiza a string (stringAlterada) com o conteudo da que deve vir antes no caso de uma ordenação alfabética
// notar que essa função só deve ser utilizada caso a as duas strings não sejam iguais (deve ser feito um teste prévio)
{
    int i = 0, j = 0; // contadores/indices das strings

    while(string1[i] != '\0' || string2[j] != '\0')
    {
        if (string1[i] < string2[j] || string1[i] == '\0') // se chegou em um char que na string1 vem antes do char da string2, então string1 é a da ordem alfabética
            {
                strcpy(stringAlterada, string1);
                return 0;
            }


        //notar que null não pode ser convertido pra int nem com cast (int)
        if (string2[j] < string1[i] || string2[j] == '\0') // análogo
            {
                strcpy(stringAlterada, string2);
                return 0;
            }


        else
        {
            i++;
            j++;
        }
    }
}


int testaMesmaConsulta(itemA *consulta1, itemA *consulta2)
// função testa se as consultas (LSE) são iguais (não importando o número de termos)
// retorna 1 caso sejam, e 0 caso não
{
    itemA *aux = consulta1;
    while(aux != NULL)
    {
        if (!testaMesmaConsulta2(aux->info, consulta2)) //se o termo não estiver na consulta2, não são iguais
            return 0;

        else // caso esteja, verifica o próximo
            aux = aux->prox;
    }

    // caso nenhum teste de termo deu errado, retorna 1 avisando que as consultas são iguais;
    return 1;
}

int testaMesmaConsulta2(char termo[LEN/2], itemA *consulta)
// função testa se aquele termo é igual a algum outro dentro de uma lista de termos
// retorna 1 se sim e 0 se não
{
    itemA *aux = consulta;
    while(aux != NULL) // enquanto não chegar no final da lista ou achar uma igual, compara os termos
    {
        if(!strcmp(aux->info, termo)) //se forem iguais (note que 0 é falso, por isso not strcmp)
            return 1;
        else
            aux = aux->prox;
    }

    return 0; // caso tenha percorrido toda a lista e não achou nenhuma igual, retorna 0
}
