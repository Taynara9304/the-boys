// programa principal do projeto "The Boys - 2024/2"
// Autor: Taynara da Silva Dias, GRR 20261372

// seus #includes vão aqui
#include "entidades.c"

// seus #defines vão aqui
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 // 525600 = 60*24*365 = 365 dias
#define N_TAMANHO_MUNDO 20000 // não era um par de coordenadas?
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_INVASOES T_FIM_DO_MUNDO / 10000

void incializar_herois()
{
  /*
    id = número sequencial [0...N_HEROIS-1]
    experiência = 0
    paciência = número aleatório [0...100]
    velocidade = número aleatório [50...5000] // em metros/minuto = 3 Km/h a 300 Km/h
    habilidades = conjunto com tamanho aleatório [1...3] de habilidades aleatórias
  */
}

void incializar_bases()
{
  /*
    id = número sequencial [0...N_BASES-1]
    local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    lotação = número aleatório [3...10]
    presentes = conjunto vazio (com capacidade para armazenar IDs
    de heróis até a lotação da base)
    espera = fila vazia
  */
}

void incializar_missoes()
{
  /*
    id = número sequencial [0...N_MISSOES-1]
    local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
    perigo = número aleatório [0...100]
  */
}

void incializar_invasoes()
{
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
  struct heroi h;
  incializar_herois(h);
  m->herois = h;

  m->NBases = N_BASES;
  struct base b;
  incializar_bases(b);
  m->bases = b;

  m->NMissoes = N_MISSOES;
  struct missao ms;
  incializar_missoes(ms);
  m->missoes = ms;

  m->NInvasoes = N_INVASOES;
  struct invasao i;
  incializar_invasoes(i);
  m->invasoes = i;

  m->NHabilidades = N_HABILIDADES;
  // m->TamanhoMundo = N_TAMANHO_MUNDO;
  m->relogio = T_INICIO;
}

// minimize o uso de variáveis globais

// programa principal
int main()
{
  // iniciar o mundo
  struct mundo *m;
  inicializar_mundo(m);

  /*
    Cada herói chegará em uma base aleatória em algum momento dos três
    primeiros dias de simulação:
    para cada herói H:
    base = número aleatório [0...N_BASES-1]
    tempo = número aleatório [0...4320] // 4320 = 60*24*3 = 3 dias
    criar e inserir na LEF o evento CHEGA (tempo, H, base)
    Cada missão deve ser agendada para ocorrer em algum momento da simu-
    lação:
    para cada missão M:
    tempo = número aleatório [0...T_FIM_DO_MUNDO]
    criar e inserir na LEF o evento MISSÃO (tempo, M)
    Cada invasão deve ser agendada para ocorrer a partir dos três primeiros dias
    de simulação:
    para cada invasão I:
    tempo = número aleatório [(60*24*3)+1...T_FIM_DO_MUNDO]
    criar e inserir na LEF o evento INVADE (tempo, I)
    O evento FIM deve ser agendado para o instante final da simulação:
    tempo = T_FIM_DO_MUNDO
    criar e inserir na LEF o evento FIM (tempo)
  */

  // executar o laço de simulação

  // destruir o mundo

  return (0);
}
