// programa principal do projeto "The Boys - 2024/2"
// Autor: Taynara da Silva Dias, GRR 20261372

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entidades.h"
#include "eventos.h"
#include "conjunto.h"
#include "fila.h"
#include "fprio.h"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 // 525600 = 60*24*365 = 365 dias
#define N_TAMANHO_MUNDO 20000 // não era um par de coordenadas?
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_INVASOES T_FIM_DO_MUNDO / 10000

enum tipo_evento
{
  CHEGA,
  ESPERA,
  DESISTE,
  AVISA,
  ENTRA,
  SAI,
  VIAJA,
  MORRE,
  MISSAO,
  FIM
};

void inicializar_herois(struct heroi *h)
{
  for (int i = 0; i < N_HEROIS; i++)
  {

    h[i].id = i;
    h[i].experiencia = 0;
    h[i].paciencia = rand() % 101;
    h[i].velocidade = (rand() % 4951) + 50;
    h[i].habilidades = cjto_aleat((rand() % 3) + 1, N_HABILIDADES);
  }
}

void inicializar_bases(struct base *b)
{
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
  /*
    id = número sequencial [0...N_BASES-1]
    local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    lotação = número aleatório [3...10]
    presentes = conjunto vazio (com capacidade para armazenar IDs
    de heróis até a lotação da base)
    espera = fila vazia
  */
}

void inicializar_missoes(struct missao *ms)
{
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

  /*
    id = número sequencial [0...N_MISSOES-1]
    local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
    perigo = número aleatório [0...100]
  */
}

void inicializar_invasoes(struct invasao *in)
{
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
  /*
    id = número sequencial [0...N_INVASOES-1]
    local = número aleatório [0...N_BASES-1]
    tempo_penalidade_heroi = número aleatório [0...4320] // 4320 = 60*24*3 = 3 dias
    tempo_penalidade_base = número aleatório [0...7200] // 7200 = 60*24*5 = 5 dias
  */
}

void inicializar_mundo(struct mundo *m)
{
  m->NHerois = N_HEROIS;
  struct heroi *h = calloc(N_HEROIS, sizeof(struct heroi));

  if (!h)
    return;

  inicializar_herois(h);
  m->herois = h;

  m->NBases = N_BASES;
  struct base *b = calloc(N_BASES, sizeof(struct base));

  if (!b)
    return;

  inicializar_bases(b);
  m->bases = b;

  m->NMissoes = N_MISSOES;
  struct missao *ms = calloc(N_MISSOES, sizeof(struct missao));

  if (!ms)
    return;

  inicializar_missoes(ms);
  m->missoes = ms;

  m->NInvasoes = N_INVASOES;
  struct invasao *in = calloc(N_INVASOES, sizeof(struct invasao));

  if (!in)
    return;

  inicializar_invasoes(in);
  m->invasoes = in;

  m->NHabilidades = N_HABILIDADES;

  m->TamanhoMundo = N_TAMANHO_MUNDO;

  m->lef = fprio_cria();

  m->relogio = T_INICIO;
}

// minimize o uso de variáveis globais

// programa principal
int main()
{
  srand(time(NULL));

  // iniciar o mundo
  struct mundo m;
  inicializar_mundo(&m);

  /*
    Cada herói chegará em uma base aleatória em algum momento dos três
    primeiros dias de simulação:
    para cada herói H:
    base = número aleatório [0...N_BASES-1]
    tempo = número aleatório [0...4320] // 4320 = 60*24*3 = 3 dias
    criar e inserir na LEF o evento CHEGA (tempo, H, base)
  */

  for (int i = 0; i < N_HEROIS; i++)
  {
    m.herois[i].id_base = rand() % N_BASES;

    struct evento *ev = malloc(sizeof(struct evento));
    ev->funcao = chega;
    ev->base = m.bases[i].id;
    ev->heroi = m.herois[i].id;
    ev->missao = -1;

    fprio_insere(m.lef, ev, 0, (rand() % 4321));
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

  // executar o laço de simulação

  // destruir o mundo

  return (0);
}
