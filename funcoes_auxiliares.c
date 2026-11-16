#include "funcoes_auxiliares.h"
#include "entidades.h"
#include <math.h>
#include <stdlib.h>

int calc_distancia_entre_bases(struct base *b_origem, struct base *b_destino)
{
    return sqrt(pow((b_origem->local.x - b_destino->local.x), 2) + pow((b_origem->local.y - b_destino->local.y), 2));
}

int aleat(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void remover_eventos_do_heroi(struct fprio_t *f, struct heroi *h)
{
    struct fpnodo_t *n = f->prim;

    while (n)
    {
        // guarda o próximo para garantir
        // que não dê erro se n for liberado
        struct fpnodo_t *prox = n->prox;
        struct evento *e = n->item;

        // verifica se o evento relacionado não é o AVISA()
        if (e->id_heroi == h->id && e->tipo != 4)
        {
            // remove o nodo da lista
            fprio_remove_item(f, e);
            free(e);
        }

        n = prox;
    }
}

struct elemento
{
    int id;
    int distancia;
};

void ordenar_bases(struct mundo *m, struct missao *missao, struct base **basesOrdenadas)
{
    struct elemento distancias[m->NBases];

    // calcula as distâncias
    for (int i = 0; i < m->NBases; i++)
    {
        struct base *b = &m->bases[i];

        int dx = missao->local.x - b->local.x;
        int dy = missao->local.y - b->local.y;
        int dist = dx * dx + dy * dy;

        distancias[i].distancia = dist;
        distancias[i].id = b->id;
    }

    // ordena as distância (inserção)
    for (int i = 1; i < m->NBases; i++)
    {
        struct elemento elem = distancias[i];
        int j = i - 1;

        while (j >= 0 && distancias[j].distancia > elem.distancia)
        {
            distancias[j + 1] = distancias[j];
            j--;
        }

        distancias[j + 1] = elem;
    }

    // reorganiza as bases
    for (int i = 0; i < m->NBases; i++)
        basesOrdenadas[i] = &m->bases[distancias[i].id];
}
