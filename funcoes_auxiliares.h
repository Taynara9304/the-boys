#ifndef FUNCOES_AUXILIARES
#define FUNCOES_AUXILIARES

#include "entidades.h"

// gera um número aleatório de min a max inclusos
int aleat(int min, int max);

// cacula a distância cartesiana entre duas bases
int calc_distancia_entre_bases(struct base *b_origem, struct base *b_destino);

// remove os elementos futuros de um herói na LEF
void remover_eventos_do_heroi(struct fprio_t *f, struct heroi *h);

// retorna um vetor de bases ordenadas no parâmetro da função
void ordenar_bases(struct mundo *m, struct missao *missao, struct base **basesOrdenadas);

#endif
