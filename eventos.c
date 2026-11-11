#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "fila.h"
#include "conjunto.h"
#include "fprio.h"
#include "eventos.h"
#include "entidades.h"

void chega(struct mundo *m, int t, struct evento *ev)
{
    int esp = 0;

    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    h->id_base = b->id;

    if ((b->presentes->num < b->lotacao_maxima) && fila_tamanho(b->fila_de_espera) == 0)
        esp = 1;
    else
        esp = h->paciencia > 10 * fila_tamanho(b->fila_de_espera);

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) %s\n", t, h->id, b->id,
           b->presentes->num, b->lotacao_maxima,
           (esp && b->estado) ? "ESPERA" : "DESISTE");

    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    if (esp && b->estado)
        // insere espera() na LEF
        novo_ev->tipo = 2;
    else
        // insere desiste() na LEF
        novo_ev->tipo = 3;

    novo_ev->id_heroi = h->id;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void espera(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, ev->id_heroi, b->id,
           fila_tamanho(b->fila_de_espera));

    fila_insere(b->fila_de_espera, &m->herois[ev->id_heroi]);

    // insere avisa(t, b) na LEF
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 4;
    novo_ev->id_heroi = ev->id_heroi;
    novo_ev->id_base = ev->id_base;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void desiste(struct mundo *m, int t, struct evento *ev)
{
    printf("%6d: DESIST HEROI %2d BASE %d\n", t, ev->id_heroi, ev->id_base);

    // insere na LEF o viaja()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 7;
    novo_ev->id_heroi = ev->id_heroi;
    novo_ev->id_base = rand() % N_BASES;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void avisa(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id,
           b->presentes->num, b->lotacao_maxima);
    for (struct fila_nodo_t *n = b->fila_de_espera->prim; n; n = n->prox)
        printf("%2d ", ((struct heroi *)n->item)->id);
    printf("]\n");

    while ((b->presentes->num < b->lotacao_maxima) && (b->fila_de_espera->num != 0))
    {
        struct heroi *heroi_removido = fila_retira(b->fila_de_espera);
        cjto_insere(b->presentes, heroi_removido->id);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", t, b->id, heroi_removido->id);

        // insere na LEF o entra()
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 5;
        novo_ev->id_heroi = heroi_removido->id;
        novo_ev->id_base = heroi_removido->id_base;
        novo_ev->id_missao = -1;
        novo_ev->id_invasao = -1;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
            free(novo_ev);
    }
}

void entra(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    int tpb = 15 + h->paciencia * ((rand() % 20) + 1);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id,
           b->presentes->num, b->lotacao_maxima, t + tpb);

    // insere o evento sai()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 6;
    novo_ev->id_heroi = h->id;
    novo_ev->id_base = h->id_base;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + tpb) < 0)
        free(novo_ev);
}

void sai(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    // retira da base atual
    struct heroi *heroi_removido = &m->herois[ev->id_heroi];

    cjto_retira(b->presentes, heroi_removido->id);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, heroi_removido->id, b->id,
           b->presentes->num, b->lotacao_maxima);

    // procura uma nova base
    int novaBase = rand() % N_BASES;

    // insere na LEF o viaja()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 7;
    novo_ev->id_heroi = heroi_removido->id;
    novo_ev->id_base = novaBase;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);

    // insere na LEF o avisa()
    struct evento *novo_ev_2 = malloc(sizeof(struct evento));
    if (!novo_ev_2)
        return;

    novo_ev_2->tipo = 4;
    novo_ev_2->id_heroi = heroi_removido->id;
    novo_ev_2->id_base = b->id;
    novo_ev_2->id_missao = -1;
    novo_ev_2->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev_2, novo_ev_2->tipo, t) < 0)
        free(novo_ev_2);
}

void viaja(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];

    struct base *b_origem = &m->bases[h->id_base];
    struct base *b_destino = &m->bases[ev->id_base];

    int distancia = sqrt(pow((b_origem->local.x - b_destino->local.x), 2) + pow((b_origem->local.y - b_destino->local.y), 2));
    int duracao = distancia / h->velocidade;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id,
           b_origem->id, b_destino->id, distancia, h->velocidade, t + duracao);

    // insere na LEF chega(t + duração, h, d)
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 1;
    novo_ev->id_heroi = h->id;
    novo_ev->id_base = b_destino->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + duracao) < 0)
        free(novo_ev);
}

void morre(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    // retira da base atual
    struct heroi *h = &m->herois[ev->id_heroi];

    cjto_retira(b->presentes, h->id);

    h->estado = 3;

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", t, h->id, ev->id_missao);

    // remove da LEF todos os eventos futuros relacionados ao herói H
    struct fpnodo_t **p = &m->lef->prim;

    while (*p)
    {
        struct evento *e = (*p)->item;

        if (e->id_heroi == h->id && e->tipo != 4)
        {
            struct fpnodo_t *removido = *p;
            *p = removido->prox;
            free(removido->item);
            free(removido);
            m->lef->num--;
        }
        else
            p = &(*p)->prox;
    }

    // cria e insere na LEF o evento AVISA (agora, B)
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 4;
    novo_ev->id_heroi = -1;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void missao(struct mundo *m, int t, struct evento *ev)
{
    /*  calcula a distância de cada base ao local da missão M
        encontra BMP = base mais próxima ativa da missão cujos heróis ativos
        possam cumpri-la */

    struct missao *missao = &m->missoes[ev->id_missao];

    int idBmp = -1;
    int menorDistancia = 0;

    for (int i = 0; i < m->NBases; i++)
    {
        struct base *b = &m->bases[i];

        if (!b->estado)
            continue;

        int dx = missao->local.x - b->local.x;
        int dy = missao->local.y - b->local.y;
        int distancia = dx * dx + dy * dy;

        if (idBmp != -1 && distancia >= menorDistancia)
            continue;

        // verifica se os heróis da base tem as habilidades da missão
        struct cjto_t *habilidadesBase = cjto_cria(m->NHabilidades);

        if (!habilidadesBase)
            continue;

        for (int j = 0; j < b->presentes->num; j++)
        {
            struct heroi *h = &m->herois[b->presentes->flag[j]];
            struct cjto_t *novo = cjto_uniao(habilidadesBase, h->habilidades);
            cjto_destroi(habilidadesBase);
            habilidadesBase = novo;
        }

        if (cjto_contem(habilidadesBase, missao->habilidades))
        {
            idBmp = i;
            menorDistancia = distancia;
        }
        cjto_destroi(habilidadesBase);
    }

    if (idBmp != -1)
    {
        struct base *b = &m->bases[idBmp];
        missao->concluida = 1;

        for (int i = 0; i < b->presentes->num; i++)
        {
            struct heroi *h = &m->herois[b->presentes->flag[i]];
            if (!h->estado)
                continue;

            int risco = missao->perigo / (h->paciencia + h->experiencia + 1);

            if (risco > (rand() % 31))
            {
                // cria e insere na LEF o evento MORRE (agora, B)
                struct evento *novo_ev = malloc(sizeof(struct evento));
                if (!novo_ev)
                    return;

                novo_ev->tipo = 8;
                novo_ev->id_heroi = h->id;
                novo_ev->id_base = b->id;
                novo_ev->id_missao = missao->id;
                novo_ev->id_invasao = -1;

                if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
                    free(novo_ev);
            }
            else
                h->experiencia += 1;
        }
    }
    else
    {
        // cria e insere na LEF o evento MISSAO (t + 24*60, m)
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 9;
        novo_ev->id_heroi = -1;
        novo_ev->id_base = -1;
        novo_ev->id_missao = missao->id;
        novo_ev->id_invasao = -1;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + 24 * 60) < 0)
            free(novo_ev);
    }
}

void invade(struct mundo *m, int t, struct evento *ev)
{
    if (b.estado == 0)
    {
        return;
    }

    for (int i = 0; i < qtdHerois; i++)
    {
        // como deixa um herói inativo?
        // insere na LEF recupera_heroi(t + i.tempo_penalidade_heroi, h);
        // remove da LEF o sai()
        // insere na LEF sai()
    }

    b.estado = 0;
    // insere na LEF reconstroi_base(t + i.tempo_penalidade_base, b);
}

void recupera_heroi(struct mundo *m, int t, struct evento *ev)
{
    h.estado = 1;
}

void reconstroi_base(struct mundo *m, int t, struct evento *ev)
{
    b.estado = 1;
}

void fim(struct mundo *m, int t, struct evento *ev)
{
    // como encerra a simulação?
    // onde eu guardo as estatísticas?
    // eu tenho que imprimir aqui ou crio outra função e chamo aqui?
}
