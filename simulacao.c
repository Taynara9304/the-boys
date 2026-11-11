#include <stdlib.h>
#include "simulacao.h"
#include "eventos.h"

void simula_mundo(struct mundo *m)
{
    struct evento *ev_atual;
    int tipo;
    int tempo;

    int acabou = 0;

    while (!acabou)
    {
        ev_atual = fprio_retira(m->lef, &tipo, &tempo);

        switch (tipo)
        {
        case 1:
            chega(m, tempo, ev_atual);
            break;
        case 2:
            espera(m, tempo, ev_atual);
            break;
        case 3:
            desiste(m, tempo, ev_atual);
            break;
        case 4:
            avisa(m, tempo, ev_atual);
            break;
        case 5:
            entra(m, tempo, ev_atual);
            break;
        case 6:
            sai(m, tempo, ev_atual);
            break;
        case 7:
            viaja(m, tempo, ev_atual);
            break;
        case 8:
            morre(m, tempo, ev_atual);
            break;
        case 9:
            missao(m, tempo, ev_atual);
            break;
        case 10:
            invade(m, tempo, ev_atual);
            break;
        case 11:
            recupera_heroi(m, tempo, ev_atual);
            break;
        case 12:
            reconstroi_base(m, tempo, ev_atual);
            break;
        case 13:
            fim(m, tempo, ev_atual);
            acabou = 1;
            break;
        default:
            break;
        }

        free(ev_atual);
        m->qtdEventos++;
    }
}