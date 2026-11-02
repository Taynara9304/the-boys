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

struct ponto
{
    int x;
    int y;
};

struct heroi
{
    int id;
    int habilidades[6];
    int estado;
    int paciencia;
    int velocidade;
    int experiência;
    int id_base;
};

struct base
{
    int id;
    int lotação_maxima;
    struct fila_t *presentes;
    int id_herois[6];
    int estado;
    struct fila_t *fila_de_espera;
    struct ponto local
};

struct missao
{
    int id;
    int habilidades;
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
    struct heroi herois;
    int NBases;
    struct base bases;
    int NMissoes;
    struct missao missoes;
    int NInvasoes;
    struct invasao invasoes;
    int NHabilidades;
    struct ponto TamanhoMundo;
    int relogio;
};
