#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "conjunto.h"
#include "fprio.h"
#include "eventos.h"
#include "entidades.h"

void chega(struct mundo *m, int t, struct evento *ev)
{
    int esp = 0;

    struct heroi *h = &m->herois[ev->heroi];
    struct base *b = &m->bases[ev->base];

    if ((b->presentes->num < b->lotacao_maxima) && fila_tamanho(b->fila_de_espera) == 0)
        esp = 1;
    else
        esp = h->paciencia > 10 * fila_tamanho(b->fila_de_espera);

    struct evento *novo_ev = malloc(sizeof(struct evento));

    if (esp && b->estado)
        // insere espera() na LEF
        novo_ev->funcao = espera;
    else
        // insere desiste() na LEF
        novo_ev->funcao = desiste;

    novo_ev->heroi = h->id;
    novo_ev->base = b->id;
    novo_ev->missao = -1;

    fprio_insere(m->lef, novo_ev, 0, t + 1);
}

void espera(struct mundo *m, int t, struct evento *ev)
{
    fila_insere(&m->bases[ev->base].fila_de_espera, &m->herois[ev->heroi]);

    // insere avisa(t, b) na LEF
    struct evento *novo_ev = malloc(sizeof(struct evento));

    novo_ev->funcao = avisa;
    novo_ev->heroi = ev->heroi;
    novo_ev->base = ev->base;
    novo_ev->missao = -1;

    fprio_insere(m->lef, novo_ev, 0, t + 1);
}

void desiste(struct mundo *m, int t, struct evento *ev)
{
    m->herois[ev->heroi].id_base = rand() % m->NBases;

    // insere na LEF o viaja()
    struct evento *novo_ev = malloc(sizeof(struct evento));

    novo_ev->funcao = viaja;
    novo_ev->heroi = ev->heroi;
    novo_ev->base = ev->base;
    novo_ev->missao = -1;

    fprio_insere(m->lef, novo_ev, 0, t + 1);
}

void avisa(struct mundo *m, int t, struct evento *ev)
{
    if (!b.estado)
        return;

    while ((fila_tamanho(b.fila_de_espera) < b.lotação_maxima) && (b.fila_de_espera->num != 0))
    {
        struct heroi *heroi_removido = fila_retira(b.fila_de_espera);
        fila_insere(b.presentes, heroi_removido);
    }
}

void entra(struct mundo *m, int t, struct evento *ev)
{
    int tpb = 15 + h.paciencia * (rand() % 21);
    // insere o evento sai()
}

void sai(struct mundo *m, int t, struct evento *ev)
{
    // retira da base atual
    fila_retira(b.presentes);

    // procura uma nova base
    int novaBase = rand();

    // como eu sei quantas bases vão ter na minha inicialização?
    // eu posso assumir que o id das bases vão de 0 a n?
    // insere na LEF o viaja()
    // insere na LEF o avisa()
}

void viaja(struct mundo *m, int t, struct evento *ev)
{
    // como eu acho as coordenadas da base atual?
    int distancia = sqrt(pow((h.id_base.x - b.local.x), 2) + pow((h.id_base.y - b.local.y), 2));
    int duracao = distancia / h.velocidade;

    // insere na LEF chega(t + duração, h, d)
}

void morre(struct mundo *m, int t, struct evento *ev)
{
    fila_retira(b.presentes);
    h.estado = 0;
    // remove da LEF todos os eventos futuros relacionados ao herói H
    // cria e insere na LEF o evento AVISA (agora, B)
}

void missao(struct mundo *m, int t, struct evento *ev)
{
    // acha a base mais próxima da missão
    struct base bmp = primeira_base;

    for (int i = 1; i < qtdBases; i++)
    {
        // como eu pego as bases?
        int distancia = sqrt(pow((h.id_base.x - b.local.x), 2) + pow((h.id_base.y - b.local.y), 2));
        if (distancia < bmp)
            bmp = base;
    }

    if (bmp)
    {
        // como marca uma missão como concluida?
        for (int i = 0; i < qtdHeroisNaBae; i++)
        {
            if (heroi_atual.estado = 0)
            {
                continue;
            }

            int risco = m.perigo / (h.paciencia + h.experiencia + 1);

            if (risco > (rand() % 31))
                // insere morre() na LEF;
                else h.experiencia += 1;
        }
    }
    else
    {
        // insere na LEF missao(t + 24*60, m)
    }
}

void invade(struct mundo *m, int t, struct evento *ev)
{
    if (b.estado == 0)
    {
        return;
    }

    for (int i = 0; i < qtdHerois; i++)
    {
        // como deixa um herói inativo?
        // insere na LEF recupera_heroi(t + i.tempo_penalidade_heroi, h);
        // remove da LEF o sai()
        // insere na LEF sai()
    }

    b.estado = 0;
    // insere na LEF reconstroi_base(t + i.tempo_penalidade_base, b);
}

void recupera_heroi(struct mundo *m, int t, struct evento *ev)
{
    h.estado = 1;
}

void reconstroi_base(struct mundo *m, int t, struct evento *ev)
{
    b.estado = 1;
}

void fim(struct mundo *m, int t, struct evento *ev)
{
    // como encerra a simulação?
    // onde eu guardo as estatísticas?
    // eu tenho que imprimir aqui ou crio outra função e chamo aqui?
}
