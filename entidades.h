#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fprio.h"
#include "fila.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 // 525600 = 60*24*365 = 365 dias
#define N_TAMANHO_MUNDO 20000 // não era um par de coordenadas?
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_INVASOES T_FIM_DO_MUNDO / 10000

struct ponto
{
    int x;
    int y;
};

struct heroi
{
    int id;
    struct cjto_t *habilidades;
    int estado; // 0 = vivo; 1 = morto; 3 = desabilitado.
    int paciencia;
    int velocidade;
    int experiencia;
    int id_base;
};

struct base
{
    int id;
    int lotacao_maxima;
    struct cjto_t *presentes;
    int id_herois[6];
    int estado;
    struct fila_t *fila_de_espera;
    struct ponto local;
};

struct missao
{
    int id;
    struct cjto_t *habilidades;
    int perigo;
    struct ponto local;
};

struct invasao
{
    int id;
    struct ponto local;
    int tempo_penalidade_base;
    int tempo_penalidade_heroi;
};

struct mundo
{
    int NHerois;
    struct heroi *herois;
    int NBases;
    struct base *bases;
    int NMissoes;
    struct missao *missoes;
    int NInvasoes;
    struct invasao *invasoes;
    int NHabilidades;
    int TamanhoMundo;
    int relogio;
    struct fprio_t *lef;
};

struct evento
{
    int tipo;
    int heroi;
    int base;
    int missao;
};

#endif
