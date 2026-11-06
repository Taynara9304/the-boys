#include "entidades.h"
#include "universo.h"
#include <stdlib.h>

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

        struct ponto p = {
        x : rand() % (N_TAMANHO_MUNDO),
        y : rand() % (N_TAMANHO_MUNDO),
        };

        b[i].local = p;
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

        struct ponto p = {
        x : rand() % (N_TAMANHO_MUNDO),
        y : rand() % (N_TAMANHO_MUNDO),
        };

        ms[i].local = p;
        ms[i].habilidades = cjto_aleat(((rand() % 5) + 6), N_HABILIDADES);
        ms[i].perigo = rand() % 101;
    }

    return ms;
}

struct invasao *inicializar_invasoes()
{
    struct invasao *in = calloc(N_INVASOES, sizeof(struct invasao));

    if (!in)
        return;

    for (int i = 0; i < N_INVASOES; i++)
    {
        in[i].id = i;

        struct ponto p = {
        x : rand() % (N_TAMANHO_MUNDO),
        y : rand() % (N_TAMANHO_MUNDO),
        };

        in[i].local = p;
        in[i].tempo_penalidade_heroi = rand() % 4320;
        in[i].tempo_penalidade_base = rand() % 7200;
    }

    return in;
}


void eventos_iniciais(struct mundo *m) {
  for (int i = 0; i < N_HEROIS; i++)
  {
    m->herois[i].id_base = rand() % N_BASES;

    struct evento *ev = malloc(sizeof(struct evento));
    ev->tipo = 1;
    ev->base = m->bases[i].id;
    ev->heroi = m->herois[i].id;
    ev->missao = -1;

    fprio_insere(m->lef, ev, 0, (rand() % 4321));
  }

  /*
    Cada missão deve ser agendada para ocorrer em algum momento da simu-
    lação:
    para cada missão M:
    tempo = número aleatório [0...T_FIM_DO_MUNDO]
    criar e inserir na LEF o evento MISSÃO (tempo, M)

  */
  for (int i = 0; i < N_MISSOES; i++)
  {
    struct evento *ev = malloc(sizeof(struct evento));
    ev->funcao = missao;
    ev->heroi = -1;
    ev->base = -1;
    ev->missao = i;

    fprio_insere(m.lef, ev, 0, (rand() % T_FIM_DO_MUNDO));
  }

    /*
    Cada invasão deve ser agendada para ocorrer a partir dos três primeiros dias
    de simulação:
    para cada invasão I:
    tempo = número aleatório [(60*24*3)+1...T_FIM_DO_MUNDO]
    criar e inserir na LEF o evento INVADE (tempo, I)
  */

  /*
    O evento FIM deve ser agendado para o instante final da simulação:
    tempo = T_FIM_DO_MUNDO
    criar e inserir na LEF o evento FIM (tempo)
  */
}

// fuções de destroi
struct mundo *destroi_mundo(struct mundo *w) {

}

