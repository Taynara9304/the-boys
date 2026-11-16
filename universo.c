#include "entidades.h"
#include "universo.h"
#include "funcoes_auxiliares.h"
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
        ev->id_base = aleat(0, m->NBases - 1);
        ev->id_missao = -1;
        ev->id_invasao = -1;

        if (fprio_insere(m->lef, ev, 1, aleat(0, 4320)) < 0)
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

        if (fprio_insere(m->lef, ev, 9, aleat(0, T_FIM_DO_MUNDO)) < 0)
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

        if (fprio_insere(m->lef, ev, 10, aleat(4321, (T_FIM_DO_MUNDO))) < 0)
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
        h[i].paciencia = aleat(0, 100);
        h[i].velocidade = aleat(50, 5000);
        h[i].habilidades = cjto_aleat(aleat(1, 3), N_HABILIDADES);
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
        b[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        b[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        b[i].lotacao_maxima = aleat(3, 10);
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

        ms[i].local.x = aleat(0, N_TAMANHO_MUNDO - 1);
        ms[i].local.y = aleat(0, N_TAMANHO_MUNDO - 1);
        ms[i].habilidades = cjto_aleat((aleat(6, 10)), N_HABILIDADES);
        ms[i].perigo = aleat(0, 100);
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
        in[i].local = aleat(0, N_BASES - 1);
        in[i].tempo_penalidade_heroi = aleat(0, 4320);
        in[i].tempo_penalidade_base = aleat(0, 7200);
    }

    return in;
}

struct mundo *destroi_mundo(struct mundo *w)
{
    if (!w)
        return NULL;

    // libera os heróis
    if (w->herois)
    {
        for (int i = 0; i < w->NHerois; i++)
            cjto_destroi(w->herois[i].habilidades);
        free(w->herois);
    }

    // libera as bases
    if (w->bases)
    {
        for (int i = 0; i < w->NBases; i++)
        {
            while (fila_tamanho(w->bases[i].fila_de_espera) > 0)
                fila_retira(w->bases[i].fila_de_espera);

            w->bases[i].presentes = cjto_destroi(w->bases[i].presentes);
            w->bases[i].fila_de_espera = fila_destroi(w->bases[i].fila_de_espera);
        }
        free(w->bases);
    }

    // libera as missões
    if (w->missoes)
    {
        for (int i = 0; i < w->NMissoes; i++)
            cjto_destroi(w->missoes[i].habilidades);
        free(w->missoes);
    }

    // libera as invasões
    free(w->invasoes);

    // libera a LEF
    w->lef = fprio_destroi(w->lef);

    // libera o mundo
    free(w);
    return NULL;
}
