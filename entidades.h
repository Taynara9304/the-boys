#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fprio.h"
#include "fila.h"

struct ponto
{
    int x;
    int y;
};

struct heroi
{
    int id;
    struct cjto_t *habilidades;
    int estado;
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

struct evento;

typedef void (*funcao_t)(struct mundo *mundo, int t, struct evento *evento);

struct evento
{
    funcao_t funcao;
    int heroi;
    int base;
    int missao;
};

#endif
