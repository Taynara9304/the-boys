#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

void chega(struct mundo *m, int t, struct evento *ev);

void espera(int t, struct heroi *h, struct base b);

void desiste(int t, struct heroi h, struct base b);

void avisa(int t, struct base b);

void entra(int t, struct heroi h, struct base b);

void sai(int t, struct heroi h, struct base b);

void viaja(int t, struct heroi h, struct base b);

void morre(int t, struct heroi h, struct base b);

void missao(int t, struct missao m);

void invade(int t, struct invasao i, struct base b);

void recupera_heroi(int t, struct heroi h);

void reconstroi_base(int t, struct base b);

void fim(int t);

#endif
