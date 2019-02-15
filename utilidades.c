#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilidades.h"

#define LEN 80
#define SPACE ' ' // caractere espa�o (crit�rio de parada do loop que significa que a string � quebrada)


int verificaComposta(char stringAvaliada[LEN])
// a fun��o verifica se a string passada � uma pesquisa composta, isto �, separada por espa�os
// retorna 1 se for e 0 se n�o for
{
    int i = 0; // contador do �ndice para percorrer todos os caracteres da string

    while (stringAvaliada[i] != '\0' && stringAvaliada[i] != SPACE) // enquanto n�o for fim da string nem achar espa�o
        i++; // vai analisando pr�ximos caracteres dessa string

    if (stringAvaliada[i] == '\0') //se chegou no final e n�o encontrou espa�os retorna 0
        return 0;

    else // se n�o � final, � espa�o, s�o os �nicos crit�rios de parada
        return 1;
}


itemA* criaConsultaSimples(char consulta[LEN])
// a fun��o recebe uma string referente a uma consulta de apenas um termo
// devolve uma lista simplesmente encadeada de apenas um termo (para que possa ser usado na �rvore das consultas)
{
    itemA *consultaLSE = (itemA*) malloc(sizeof(itemA));

    consultaLSE->prox = NULL; // lista �nica
    strcpy(consultaLSE->info, consulta); // copia a informa��o pra lista

    return consultaLSE;
}

itemA* separaTermos(char consulta[LEN], int *quantidadeDeTermos)
// a fun��o recebe uma string (consulta) composta de termos e quebra essa string
// devolve uma lista simplesmente encadeada onde cada nodo da lista representa um termo da consulta
// al�m disso, altera a quantidade de termos (passada por par�metro) dessa consulta
{
    itemA *listaDeTermos = NULL; //(itemA*) malloc(sizeof(itemA));
    char temporario[LEN/2]; // vari�vel auxiliar que ir� guardar o termo enquanto o while for varrendo
    int i = 0; // contador do �ndice para percorrer todos os caracteres da string consulta
    int j = 0; // contador do �ndice para percorrer todos os caracteres da string temporario

    while(consulta[i] != '\0') // enquanto n�o acabar de percorrer a string
    {
        while(consulta[i] != SPACE && consulta[i] != '\0')
        {
            // vai copiando os caracteres do termo at� achar o espa�o
            temporario[j] = consulta[i];
            i++;
            j++;
        }

        temporario[j] = '\0'; // encerra string temporario

        if (consulta[i] != '\0')
            i++; // incrementa i para pular o espa�o (e n�o incrementa caso seja o final da string)

        j = 0; // zera j para reaproveitar a vari�vel temporario

        *quantidadeDeTermos = *quantidadeDeTermos + 1; //incrementa conte�do de quantidadeDeTermos
        listaDeTermos = insereLista(listaDeTermos, temporario); // insere o termo novo na lista
    }

    return listaDeTermos;
}

itemA* insereLista(itemA *lista, char termo[LEN/2]) // tamb�m reduz a quantidade de caracteres j� que � um �nico termo
// a fun��o recebe uma lista encadeada simples e insere um nodo novo no in�cio dessa lista
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
// a fun��o vai receber duas listas correspondentes a consultas e retornar� qual vem primeiro na ordem alfab�tica
{
    itemA *aux1 = lista1;
    itemA *aux2 = lista2;

    while (aux1 != NULL || aux2 != NULL)
    {
        if (!strcmp(aux1->info, aux2->info)) // se o primeiro termo for igual verifica se alguma acabou (retornando-a), se n�o compara os pr�ximos dois
        {
            if (aux1->prox == NULL)
                return lista1;

            else if (aux2->prox == NULL)
                return lista2;

            else  // caso contr�rio, compara os outros termos
            {
                aux1 = aux1->prox;
                aux2 = aux2->prox;
            }
        }

        else if (strcmp(aux1->info, aux2->info) > 0)   // se a string de aux1 vier depois - na ordem alfab�tica - da string de aux2, retorna aux2 que vem antes
            return aux2;

        else
            return aux1;
    }

    printf("ERRROOOOO NA LOGICA");
}


void ordemAlfabetica(char string1[LEN], char string2[LEN], char *stringAlterada)
// fun��o que compara duas strings e atualiza a string (stringAlterada) com o conteudo da que deve vir antes no caso de uma ordena��o alfab�tica
// notar que essa fun��o s� deve ser utilizada caso a as duas strings n�o sejam iguais (deve ser feito um teste pr�vio)
{
    int i = 0, j = 0; // contadores/indices das strings

    while(string1[i] != '\0' || string2[j] != '\0')
    {
        if (string1[i] < string2[j] || string1[i] == '\0') // se chegou em um char que na string1 vem antes do char da string2, ent�o string1 � a da ordem alfab�tica
            {
                strcpy(stringAlterada, string1);
                return 0;
            }


        //notar que null n�o pode ser convertido pra int nem com cast (int)
        if (string2[j] < string1[i] || string2[j] == '\0') // an�logo
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
// fun��o testa se as consultas (LSE) s�o iguais (n�o importando o n�mero de termos)
// retorna 1 caso sejam, e 0 caso n�o
{
    itemA *aux = consulta1;
    while(aux != NULL)
    {
        if (!testaMesmaConsulta2(aux->info, consulta2)) //se o termo n�o estiver na consulta2, n�o s�o iguais
            return 0;

        else // caso esteja, verifica o pr�ximo
            aux = aux->prox;
    }

    // caso nenhum teste de termo deu errado, retorna 1 avisando que as consultas s�o iguais;
    return 1;
}

int testaMesmaConsulta2(char termo[LEN/2], itemA *consulta)
// fun��o testa se aquele termo � igual a algum outro dentro de uma lista de termos
// retorna 1 se sim e 0 se n�o
{
    itemA *aux = consulta;
    while(aux != NULL) // enquanto n�o chegar no final da lista ou achar uma igual, compara os termos
    {
        if(!strcmp(aux->info, termo)) //se forem iguais (note que 0 � falso, por isso not strcmp)
            return 1;
        else
            aux = aux->prox;
    }

    return 0; // caso tenha percorrido toda a lista e n�o achou nenhuma igual, retorna 0
}
