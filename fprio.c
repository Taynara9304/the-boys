// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Marcos Castilho, inclui as structs no arquivo.c, Out 2025
// Implementação com lista encadeada simples

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"

// A COMPLETAR

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria()
{
    struct fprio_t *fprio = malloc(sizeof(struct fprio_t));

    if (!fprio)
        return NULL;

    memset(fprio, 0, sizeof(struct fprio_t));

    return fprio;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi(struct fprio_t *f)
{
    if (!f)
        return NULL;

    struct fpnodo_t *aux;

    for (int i = 0; i < f->num; i++)
    {
        aux = f->prim;
        free(aux->item);
        f->prim = aux->prox;
        free(aux);
    }

    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere(struct fprio_t *f, void *item, int tipo, int prio)
{
    if (!f || !item)
        return -1;

    // verifica se o item já está na fila
    struct fpnodo_t *aux = f->prim;
    while (aux)
    {
        if (aux->item == item)
            return -1;
        aux = aux->prox;
    }

    struct fpnodo_t *elemento = malloc(sizeof(struct fpnodo_t));

    if (!elemento)
        return -1;

    memset(elemento, 0, sizeof(struct fpnodo_t));

    elemento->item = item;
    elemento->tipo = tipo;
    elemento->prio = prio;
    elemento->prox = NULL;

    // caso seja o primeiro elemento
    if (!f->num)
    {
        f->prim = elemento;
        elemento->prox = NULL;

        f->num += 1;
        return f->num;
    }

    // compara com o primeiro
    if (f->prim->prio > elemento->prio)
    {
        elemento->prox = f->prim;
        f->prim = elemento;

        f->num += 1;
        return f->num;
    }

    // caso não seja o primeiro
    int i = 0;
    aux = f->prim;

    while (i < f->num - 1)
    {

        if (aux->prox->prio > elemento->prio)
        {
            elemento->prox = aux->prox;
            aux->prox = elemento;

            f->num += 1;
            return f->num;
        }

        i++;
        aux = aux->prox;
    }

    // caso ele seja o ultimo elemento
    aux->prox = elemento;

    f->num += 1;
    return f->num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira(struct fprio_t *f, int *tipo, int *prio)
{
    if ((!f) || (!f->prim) || (!tipo) || (!prio))
        return NULL;

    struct fpnodo_t *aux = f->prim;
    void *item = aux->item;

    *tipo = aux->tipo;
    *prio = aux->prio;

    f->prim = aux->prox;
    free(aux);
    f->num -= 1;

    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho(struct fprio_t *f)
{
    if (!f)
        return -1;

    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime(struct fprio_t *f)
{
    if (!f)
        return;

    struct fpnodo_t *aux = f->prim;

    if (!f->num)
        return;

    for (int i = 0; i < f->num - 1; i++)
    {
        printf("(%d ", aux->tipo);
        printf("%d) ", aux->prio);
        aux = aux->prox;
    }

    printf("(%d ", aux->tipo);
    printf("%d)", aux->prio);
}