#include "entidades.h"
#include "universo.h"
#include <stdlib.h>

void eventos_iniciais(struct mundo *m)
{
    // cria evento CHEGA
    for (int i = 0; i < N_HEROIS; i++)
    {
        struct evento *ev = malloc(sizeof(struct evento));

        if (!ev)
            return;

        ev->tipo = 1;
        ev->id_heroi = i;
        ev->id_base = rand() % N_BASES;
        ev->id_missao = -1;
        ev->id_invasao = -1;

        if (fprio_insere(m->lef, ev, 1, (rand() % 4321)) < 0)
        {
            free(ev);
            return;
        }
    }

    // cria evento MISSAO
    for (int i = 0; i < N_MISSOES; i++)
    {
        struct evento *ev = malloc(sizeof(struct evento));
        if (!ev)
            return;

        ev->tipo = 9;
        ev->id_heroi = -1;
        ev->id_base = -1;
        ev->id_missao = i;
        ev->id_invasao = -1;

        if (fprio_insere(m->lef, ev, 9, (rand() % (T_FIM_DO_MUNDO + 1))) < 0)
        {
            free(ev);
            return;
        }
    }

    // cria evento INVASAO
    for (int i = 0; i < N_INVASOES; i++)
    {
        struct evento *ev = malloc(sizeof(struct evento));
        if (!ev)
            return;

        ev->tipo = 10;
        ev->id_heroi = -1;
        ev->id_base = m->invasoes[i].local;
        ev->id_missao = -1;
        ev->id_invasao = i;

        if (fprio_insere(m->lef, ev, 10, 4321 + (rand() % (T_FIM_DO_MUNDO - 4321 + 1))) < 0)
        {
            free(ev);
            return;
        }
    }

    // cria o evento FIM
    struct evento *ev = malloc(sizeof(struct evento));
    if (!ev)
        return;

    ev->tipo = 13;
    ev->id_heroi = -1;
    ev->id_base = -1;
    ev->id_missao = -1;
    ev->id_invasao = -1;

    if (fprio_insere(m->lef, ev, 13, T_FIM_DO_MUNDO) < 0)
        free(ev);
}

void inicializar_mundo(struct mundo *m)
{
    m->NHerois = N_HEROIS;
    m->herois = inicializar_herois();

    m->NBases = N_BASES;
    m->bases = inicializar_bases();

    m->NMissoes = N_MISSOES;
    m->missoes = inicializar_missoes();

    m->NInvasoes = N_INVASOES;
    m->invasoes = inicializar_invasoes();

    m->NHabilidades = N_HABILIDADES;

    m->TamanhoMundo = N_TAMANHO_MUNDO;

    m->lef = fprio_cria();

    m->relogio = T_INICIO;

    if (!m->herois || !m->bases || !m->missoes || !m->invasoes || !m->lef)
        return;

    eventos_iniciais(m);
}

struct heroi *inicializar_herois()
{
    struct heroi *h = calloc(N_HEROIS, sizeof(struct heroi));

    if (!h)
        return NULL;

    for (int i = 0; i < N_HEROIS; i++)
    {
        h[i].id = i;
        h[i].experiencia = 0;
        h[i].estado = 1;
        h[i].paciencia = rand() % 101;
        h[i].velocidade = (rand() % 4951) + 50;
        h[i].habilidades = cjto_aleat((rand() % 3) + 1, N_HABILIDADES);
    }

    return h;
}

struct base *inicializar_bases()
{
    struct base *b = calloc(N_BASES, sizeof(struct base));

    if (!b)
        return NULL;

    for (int i = 0; i < N_BASES; i++)
    {
        b[i].id = i;
        b[i].estado = 1;
        b[i].local.x = rand() % N_TAMANHO_MUNDO;
        b[i].local.y = rand() % N_TAMANHO_MUNDO;
        b[i].lotacao_maxima = (rand() % 8) + 3;
        b[i].presentes = cjto_cria(N_HEROIS);
        b[i].fila_de_espera = fila_cria();
    }

    return b;
}

struct missao *inicializar_missoes()
{
    struct missao *ms = calloc(N_MISSOES, sizeof(struct missao));

    if (!ms)
        return NULL;

    for (int i = 0; i < N_MISSOES; i++)
    {
        ms[i].id = i;

        ms[i].local.x = rand() % N_TAMANHO_MUNDO;
        ms[i].local.y = rand() % N_TAMANHO_MUNDO;
        ms[i].habilidades = cjto_aleat(((rand() % 5) + 6), N_HABILIDADES);
        ms[i].perigo = rand() % 101;
    }

    return ms;
}

struct invasao *inicializar_invasoes()
{
    struct invasao *in = calloc(N_INVASOES, sizeof(struct invasao));

    if (!in)
        return NULL;

    for (int i = 0; i < N_INVASOES; i++)
    {
        in[i].id = i;
        in[i].local = rand() % N_BASES;
        in[i].tempo_penalidade_heroi = rand() % 4321;
        in[i].tempo_penalidade_base = rand() % 7201;
    }

    return in;
}

// fuções de destroi
struct mundo *destroi_mundo(struct mundo *w)
{
    if (!w)
        return NULL;

    if (w->herois)
    {
        for (int i = 0; i < w->NHerois; i++)
            cjto_destroi(w->herois[i].habilidades);
        free(w->herois);
    }

    if (w->bases)
    {
        for (int i = 0; i < w->NBases; i++)
        {
            cjto_destroi(w->bases[i].presentes);
            fila_destroi(&w->bases[i].fila_de_espera);
        }
        free(w->bases);
    }

    if (w->missoes)
    {
        for (int i = 0; i < w->NMissoes; i++)
            cjto_destroi(w->missoes[i].habilidades);
        free(w->missoes);
    }

    free(w->invasoes);

    fprio_destroi(&w->lef);

    free(w);
    return NULL;
}
