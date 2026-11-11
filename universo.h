#include "entidades.h"

void inicializar_mundo(struct mundo *m);

struct heroi *inicializar_herois();

struct base *inicializar_bases();

struct missao *inicializar_missoes();

struct invasao *inicializar_invasoes();

void eventos_iniciais();

struct mundo *destroi_mundo(struct mundo *w);
