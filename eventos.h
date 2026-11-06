#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

// 1
void chega(struct mundo *m, int t, struct evento *ev);

// 2
void espera(int t, struct heroi *h, struct base b);

// 3
void desiste(int t, struct heroi h, struct base b);

// 4
void avisa(int t, struct base b);

// 5
void entra(int t, struct heroi h, struct base b);

// 6
void sai(int t, struct heroi h, struct base b);

// 7
void viaja(int t, struct heroi h, struct base b);

// 8
void morre(int t, struct heroi h, struct base b);

// 9
void missao(int t, struct missao m);

// 10
void invade(int t, struct invasao i, struct base b);

// 11
void recupera_heroi(int t, struct heroi h);

// 12
void reconstroi_base(int t, struct base b);

// 13
void fim(int t);

#endif
