#include <stdlib.h>
#include <math.h>
#include "fila.h"
#include "fprio.h"
#include "eventos.h"
#include "entidades.c"

void chega(int t, struct heroi h, struct base b)
{
    int espera = 0;

    h.id_base = b.id;

    if (b.id_herois < b.lotação_maxima && b.fila_de_espera.num == 0)
        espera = 1;
    else
        espera = h.paciencia > 10 * b.fila_de_espera.num;

    if (espera && b.estado)
        // insere espera() na LEF
        return;
    else
        // insere desiste() na LEF
        return;
}

void espera(int t, struct heroi *h, struct base b)
{
    b.fila_de_espera.fim->prox->item = h;
    // insere avisa(t, b) na LEF
}

void desiste(int t, struct heroi h, struct base b)
{
    int novaBase = rand();
    // como eu sei quantas bases vão ter na minha inicialização?
    // eu posso assumir que o id das bases vão de 0 a n?
    // insere na LEF o viaja()
}

void avisa(int t, struct base b)
{
    if (!b.estado)
        return;

    while ((fila_tamanho(b.fila_de_espera) < b.lotação_maxima) && (b.fila_de_espera->num != 0))
    {
        struct heroi *heroi_removido = fila_retira(b.fila_de_espera);
        fila_insere(b.presentes, heroi_removido);
    }
}

void entra(int t, struct heroi h, struct base b)
{
    int tpb = 15 + h.paciencia * (rand() % 21);
    // insere o evento sai()
}

void sai(int t, struct heroi h, struct base b)
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

void viaja(int t, struct heroi h, struct base b)
{
    // como eu acho as coordenadas da base atual?
    int distancia = sqrt(pow((h.id_base.x - b.local.x), 2) + pow((h.id_base.y - b.local.y), 2));
    int duracao = distancia / h.velocidade;

    // insere na LEF chega(t + duração, h, d)
}

void morre(int t, struct heroi h, struct base b)
{
    fila_retira(b.presentes);
    h.estado = 0;
    // remove da LEF todos os eventos futuros relacionados ao herói H
    // cria e insere na LEF o evento AVISA (agora, B)
}

void missao(int i, struct missao m)
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

void invade(int t, struct invasao i, struct base b)
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

void recupera_heroi(int i, struct heroi h)
{
    h.estado = 1;
}

void reconstroi_base(int t, struct base b)
{
    b.estado = 1;
}

void fim(int t)
{
    // como encerra a simulação?
    // onde eu guardo as estatísticas?
    // eu tenho que imprimir aqui ou crio outra função e chamo aqui?
}
