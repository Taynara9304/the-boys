#include "fila.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria()
{
    struct fila_t *fila = malloc(sizeof(struct fila_t));

    if (!fila)
        return NULL;

    fila->prim = NULL;
    fila->fim = NULL;
    fila->num = 0;

    return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi(struct fila_t *f)
{
    if (!f)
        return NULL;

    for (int i = 0; i < f->num; i++)
    {
        struct fila_nodo_t *aux = f->prim;
        free(f->prim->item);
        f->prim = aux->prox;
        free(aux);
    }

    free(f);

    return NULL;
}

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere(struct fila_t *f, void *item)
{
    if (!f || !item)
        return -1;

    struct fila_nodo_t *aux = f->prim;

    for (int i = 0; i < f->num; i++)
    {
        if (aux->item == item)
            return -1;

        aux = aux->prox;
    }

    aux = malloc(sizeof(struct fila_nodo_t));
    if (!aux)
        return -1;

    aux->item = item;
    aux->prox = NULL;

    if (f->fim)
        f->fim->prox = aux;
    else
        f->prim = aux;

    f->fim = aux;
    f->num += 1;

    return f->num;
}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira(struct fila_t *f)
{
    if (!f || f->num == 0)
        return NULL;

    struct fila_nodo_t *aux = f->prim;
    void *item = aux->item;

    f->prim = aux->prox;

    if (!f->prim)
        f->fim = NULL;

    free(aux);
    f->num -= 1;

    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho(struct fila_t *f)
{
    if (!f)
        return -1;

    return f->num;
}

// Imprime o conteúdo da fila
void fila_imprime(struct fila_t *f)
{
    if (!f || f->num <= 0)
        return;

    struct fila_nodo_t *aux = f->prim;

    for (int i = 0; i < f->num - 2; i++)
    {
        printf("%p ", aux->item);
        aux = aux->prox;
    }

    printf("%p", aux->item);
}
