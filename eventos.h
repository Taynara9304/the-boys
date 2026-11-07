#ifndef EVENTOS
#define EVENTOS

#include "entidades.h"

// 1
void chega(struct mundo *m, int t, struct evento *ev);

// 2
void espera(struct mundo *m, int t, struct evento *ev);

// 3
void desiste(struct mundo *m, int t, struct evento *ev);

// 4
void avisa(struct mundo *m, int t, struct evento *ev);

// 5
void entra(struct mundo *m, int t, struct evento *ev);

// 6
void sai(struct mundo *m, int t, struct evento *ev);

// 7
void viaja(struct mundo *m, int t, struct evento *ev);

// 8
void morre(struct mundo *m, int t, struct evento *ev);

// 9
void missao(struct mundo *m, int t, struct evento *ev);

// 10
void invade(struct mundo *m, int t, struct evento *ev);

// 11
void recupera_heroi(struct mundo *m, int t, struct evento *ev);

// 12
void reconstroi_base(struct mundo *m, int t, struct evento *ev);

// 13
void fim(struct mundo *m, int t, struct evento *ev);

#endif
