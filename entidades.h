#include <stdbool.h>

// descreve um nodo da fila
struct fila_nodo_t
{
    void *item;               // item associado ao nodo
    struct fila_nodo_t *prox; // próximo nodo
};

// descreve uma fila
struct fila_t
{
    struct fila_nodo_t *prim; // primeiro nodo da fila
    struct fila_nodo_t *fim;  // último nodo da fila
    int num;                  // número de itens na fila
};

struct cjto_t
{
    int cap;    // Capacidade do conjunto
    int num;    // Número de itens (cardinalidade)
    bool *flag; // Vetor de booleanos
};

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
    int lotação_maxima;
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
};
