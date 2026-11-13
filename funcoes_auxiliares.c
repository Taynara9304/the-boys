#include "funcoes_auxiliares.h"
#include "entidades.h"
#include <math.h>


int calc_distancia_entre_bases(struct base *b_origem, struct base *b_destino) {
    return sqrt(pow((b_origem->local.x - b_destino->local.x), 2) + pow((b_origem->local.y - b_destino->local.y), 2));
}

int aleat(int min, int max) {
    return (rand() % (max + min)) + min;
}