// Tipo abstrato de dado "conjunto de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa um conjunto de números inteiros com capacidade para
// até CAP valores entre 0 e CAP-1, definida no momento de sua criação.
//
// Implementação com vetor de booleanos.

#include <stdlib.h>
#include <stdio.h>
#include "conjunto.h"

// Cria um conjunto vazio para armazenar até CAP valores entre 0 e CAP-1
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_cria(int cap)
{
    if (cap < 0)
        return NULL;

    struct cjto_t *conjunto = malloc(sizeof(struct cjto_t));

    if (!conjunto)
        return NULL;

    conjunto->flag = calloc(cap, sizeof(bool));

    if (!conjunto->flag)
    {
        free(conjunto);
        return NULL;
    }

    conjunto->cap = cap;
    conjunto->num = 0;

    return conjunto;
}

// Destrói o conjunto, liberando a memória ocupada por ele.
// Retorno: NULL
struct cjto_t *cjto_destroi(struct cjto_t *c)
{
    if (!c)
        return NULL;

    free(c->flag);
    free(c);

    return NULL;
}

// Cria e retorna uma cópia do conjunto c.
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_copia(struct cjto_t *c)
{
    if (!c)
        return NULL;

    struct cjto_t *conjunto = malloc(sizeof(struct cjto_t));

    if (!conjunto)
        return NULL;

    conjunto->flag = malloc(sizeof(bool) * c->cap);

    if (!conjunto->flag)
    {
        free(conjunto);
        return NULL;
    }

    for (int i = 0; i < c->cap; i++)
        conjunto->flag[i] = c->flag[i];

    conjunto->cap = c->cap;
    conjunto->num = c->num;

    return conjunto;
}

// Cria um conjunto para até CAP valores preenchido
// com N itens aleatórios na faixa [0...CAP-1].
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_aleat(int n, int cap)
{
    if (cap <= 0 || n < 0 || n > cap)
        return NULL;

    struct cjto_t *conjunto = malloc(sizeof(struct cjto_t));

    if (!conjunto)
        return NULL;

    conjunto->flag = calloc(cap, sizeof(bool));

    if (!conjunto->flag)
    {
        free(conjunto);
        return NULL;
    }

    int i = 0;

    while (i < n)
    {
        int posicao = rand() % cap;

        if (!conjunto->flag[posicao])
        {
            conjunto->flag[posicao] = true;
            i++;
        }
    }

    conjunto->cap = cap;
    conjunto->num = n;

    return conjunto;
}

// Insere um item no conjunto. Ignora se item não estiver entre 0 e CAP-1.
// Retorno: número de itens no conjunto após a operação ou -1 em erro.
int cjto_insere(struct cjto_t *c, int item)
{
    if (!c)
        return -1;

    if (item >= c->cap || item < 0)
        return c->num;

    if (!c->flag[item])
    {
        c->flag[item] = true;
        c->num += 1;
    }

    return c->num;
}

// Retira um item do conjunto. Ignora se item não estiver entre 0 e CAP-1.
// Retorno: número de itens no conjunto após a operação ou -1 em erro.
int cjto_retira(struct cjto_t *c, int item)
{
    if (!c)
        return -1;

    if (item >= c->cap || item < 0)
        return c->num;

    if (c->flag[item])
    {
        c->flag[item] = false;
        c->num -= 1;
    }

    return c->num;
}

// Informa a cardinalidade (número de itens) do conjunto.
// Retorno: número de itens no conjunto ou -1 em erro.
int cjto_card(struct cjto_t *c)
{
    if (!c)
        return -1;

    return c->num;
}

// Testa se o item está no conjunto.
// Retorno: 1 se está no conjunto, 0 se não está ou -1 em erro.
int cjto_pertence(struct cjto_t *c, int item)
{
    if (!c)
        return -1;

    if (item >= c->cap || item < 0)
        return 0;

    if (c->flag[item])
        return 1;
    else
        return 0;
}

// Testa se os conjuntos c1 e c2 têm os mesmos itens.
// Retorno: 1 se c1 == c2, 0 se não ou -1 em erro.
int cjto_iguais(struct cjto_t *c1, struct cjto_t *c2)
{
    if (!c1 || !c2)
        return -1;

    if ((c1->cap != c2->cap) || (c1->num != c2->num))
        return 0;

    for (int i = 0; i < c1->cap; i++)
        if (c1->flag[i] != c2->flag[i])
            return 0;

    return 1;
}

// Testa se o conjunto c1 contém o conjunto c2.
// Retorno: 1 se c1 contém c2, 0 se não ou -1 em erro.
int cjto_contem(struct cjto_t *c1, struct cjto_t *c2)
{
    if (!c1 || !c2)
        return -1;

    if (c1->num < c2->num)
        return 0;

    for (int i = 0; i < c2->cap; i++)
        if (c2->flag[i] && (i >= c1->cap || !c1->flag[i]))
            return 0;

    return 1;
}

// Cria e retorna o conjunto uniao entre os conjunto c1 e c2.
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_uniao(struct cjto_t *c1, struct cjto_t *c2)
{
    if (!c1 || !c2)
        return NULL;

    struct cjto_t *conjunto = cjto_cria((c1->cap > c2->cap) ? c1->cap : c2->cap);

    if (!conjunto)
        return NULL;

    for (int i = 0; i < conjunto->cap; i++)
        if ((i < c1->cap && c1->flag[i]) || (i < c2->cap && c2->flag[i]))
            cjto_insere(conjunto, i);

    return conjunto;
}

// Cria e retorna o conjunto interseção entre os conjunto c1 e c2.
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_inter(struct cjto_t *c1, struct cjto_t *c2)
{
    if (!c1 || !c2)
        return NULL;

    struct cjto_t *conjunto = cjto_cria(c1->cap);

    if (!conjunto)
        return NULL;

    for (int i = 0; i < conjunto->cap; i++)
        if (c1->flag[i] && !(i < c2->cap && c2->flag[i]))
            cjto_insere(conjunto, i);

    return conjunto;
}

// Cria e retorna o conjunto diferença c1 - c2.
// Retorno: ponteiro para o novo conjunto ou NULL em erro.
struct cjto_t *cjto_dif(struct cjto_t *c1, struct cjto_t *c2)
{
    if (!c1 || !c2)
        return NULL;

    struct cjto_t *conjunto = cjto_cria((c1->num > c2->num) ? c1->num : c2->num);

    if (!conjunto)
        return NULL;

    for (int i = 0; i < conjunto->cap; i++)
        if ((i < c1->cap && c1->flag[i]) || (i < c2->cap && c2->flag[i]))
            cjto_insere(conjunto, i);

    return conjunto;
}

// Imprime o conteúdo do conjunto no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois e sem \n no fim
void cjto_imprime(struct cjto_t *c)
{
    if (!c)
        return;

    for (int i = 0; i < c->num - 1; i++)
    {
        if (c->flag[i])
            printf("%d ", i);
    }

    if (c->flag[c->num])
        printf("%d", c->num);
}
