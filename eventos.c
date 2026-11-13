#include <stdlib.h>
#include <stdio.h>

#include "fila.h"
#include "conjunto.h"
#include "fprio.h"
#include "eventos.h"
#include "entidades.h"
#include "funcoes_auxiliares.h"

// o herói chega na base e decide se ele vai esperar ou desistir
void chega(struct mundo *m, int t, struct evento *ev)
{
    int esp = 0;

    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    h->id_base = b->id;

    //verifica se o herói vai esperar
    if ((cjto_card(b->presentes) < b->lotacao_maxima) && fila_tamanho(b->fila_de_espera) == 0)
        esp = 1;
    else
        esp = h->paciencia > 10 * fila_tamanho(b->fila_de_espera);

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) %s\n", t, h->id, b->id,
           b->presentes->num, b->lotacao_maxima,
           (esp && b->estado) ? "ESPERA" : "DESISTE");

    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    if (esp && b->estado)
        // insere espera() na LEF
        novo_ev->tipo = 2;
    else
        // insere desiste() na LEF
        novo_ev->tipo = 3;

    novo_ev->id_heroi = h->id;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

// adiciona o herói na fila de espera na base
void espera(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    // atualiza as estaticas de máximo de heróis na fila
    if (fila_tamanho(b->fila_de_espera) + 1 > m->estatisticas.maxFila_base[b->id])
        m->estatisticas.maxFila_base[b->id] = b->fila_de_espera->num + 1;

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, ev->id_heroi, b->id,
           fila_tamanho(b->fila_de_espera));

    fila_insere(b->fila_de_espera, &m->herois[ev->id_heroi]);

    // insere AVISA(t, b) na LEF
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 4;
    novo_ev->id_heroi = ev->id_heroi;
    novo_ev->id_base = ev->id_base;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

// procura uma nova base para o herói
void desiste(struct mundo *m, int t, struct evento *ev)
{
    printf("%6d: DESIST HEROI %2d BASE %d\n", t, ev->id_heroi, ev->id_base);

    // insere na LEF o viaja()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 7;
    novo_ev->id_heroi = ev->id_heroi;
    novo_ev->id_base = aleat(0, m->NBases);
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

// coloca os heróis da fila na base
void avisa(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id,
           b->presentes->num, b->lotacao_maxima);
    fila_imprime(b->fila_de_espera);
    printf("]\n");

    // enquanto houver vagas na base e heróis na fila, remove os heróis da fila e coloca na base
    while ((cjto_card(b->presentes) < b->lotacao_maxima) && (fila_tamanho(b->fila_de_espera) != 0))
    {
        struct heroi *heroi_removido = fila_retira(b->fila_de_espera);
        cjto_insere(b->presentes, heroi_removido->id);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", t, b->id, heroi_removido->id);

        // insere na LEF o ENTRA()
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 5;
        novo_ev->id_heroi = heroi_removido->id;
        novo_ev->id_base = heroi_removido->id_base;
        novo_ev->id_missao = -1;
        novo_ev->id_invasao = -1;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
            free(novo_ev);
    }
}

// agenda o evento sai
void entra(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    // tempo de permanência na base
    int tpb = 15 + h->paciencia * ((rand() % 20) + 1);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id,
           b->presentes->num, b->lotacao_maxima, t + tpb);

    // insere o evento SAI()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 6;
    novo_ev->id_heroi = h->id;
    novo_ev->id_base = h->id_base;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + tpb) < 0)
        free(novo_ev);
}

// retira o herói da base atual, procura uma nova base e viaja
void sai(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    // retira da base atual
    struct heroi *heroi_removido = &m->herois[ev->id_heroi];

    cjto_retira(b->presentes, heroi_removido->id);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", t, heroi_removido->id, b->id,
           b->presentes->num, b->lotacao_maxima);

    // procura uma nova base
    int novaBase = rand() % m->NBases;

    // insere na LEF o VIAJA()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 7;
    novo_ev->id_heroi = heroi_removido->id;
    novo_ev->id_base = novaBase;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);

    // insere na LEF o AVISA()
    struct evento *novo_ev_2 = malloc(sizeof(struct evento));
    if (!novo_ev_2)
        return;

    novo_ev_2->tipo = 4;
    novo_ev_2->id_heroi = heroi_removido->id;
    novo_ev_2->id_base = b->id;
    novo_ev_2->id_missao = -1;
    novo_ev_2->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev_2, novo_ev_2->tipo, t) < 0)
        free(novo_ev_2);
}

// calcula o tempo da viagem e insere o CHEGA
void viaja(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];

    struct base *b_origem = &m->bases[h->id_base];
    struct base *b_destino = &m->bases[ev->id_base];

    int distancia = calc_distancia_entre_bases(b_origem, b_destino);
    int duracao = distancia / h->velocidade;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id,
           b_origem->id, b_destino->id, distancia, h->velocidade, t + duracao);

    // insere na LEF CHEGA(t + duração, h, d)
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 1;
    novo_ev->id_heroi = h->id;
    novo_ev->id_base = b_destino->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + duracao) < 0)
        free(novo_ev);
}

// retira o herói da base, muda o estado dele e remove todos os eventos relacionado a ele
void morre(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];
    struct heroi *h = &m->herois[ev->id_heroi];
    m->estatisticas.qtdMortes++;

    cjto_retira(b->presentes, h->id);

    h->estado = 0;

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", t, h->id, ev->id_missao);

    // remove da LEF todos os eventos futuros relacionados ao herói H
    struct fpnodo_t **p = &m->lef->prim;

    while (*p)
    {
        struct evento *e = (*p)->item;

        if (e->id_heroi == h->id && e->tipo != 4)
        {
            struct fpnodo_t *removido = *p;
            *p = removido->prox;
            free(removido->item);
            free(removido);
            m->lef->num--;
        }
        else
            p = &(*p)->prox;
    }

    // cria e insere na LEF o evento AVISA (agora, B)
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 4;
    novo_ev->id_heroi = -1;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void missao(struct mundo *m, int t, struct evento *ev)
{
    /*  calcula a distância de cada base ao local da missão M
        encontra BMP = base mais próxima ativa da missão cujos heróis ativos
        possam cumpri-la */

    struct missao *missao = &m->missoes[ev->id_missao];

    int idBmp = -1;
    int menorDistancia = 0;

    for (int i = 0; i < m->NBases; i++)
    {
        struct base *b = &m->bases[i];

        if (b->estado != 1)
            continue;

        int dx = missao->local.x - b->local.x;
        int dy = missao->local.y - b->local.y;
        int distancia = dx * dx + dy * dy;

        if (idBmp != -1 && distancia >= menorDistancia)
            continue;

        // verifica se os heróis da base tem as habilidades da missão
        struct cjto_t *habilidadesBase = cjto_cria(m->NHabilidades);

        if (!habilidadesBase)
            continue;

        for (int j = 0; j < b->presentes->cap; j++)
        {
            if (!b->presentes->flag[j])
                continue;

            struct heroi *h = &m->herois[j];
            struct cjto_t *novo = cjto_uniao(habilidadesBase, h->habilidades);

            cjto_destroi(habilidadesBase);

            habilidadesBase = novo;
        }

        if (habilidadesBase && cjto_contem(habilidadesBase, missao->habilidades) == 1)
        {
            idBmp = i;
            menorDistancia = distancia;
        }
        cjto_destroi(habilidadesBase);
    }

    missao->qtdTentativas++;

    if (idBmp != -1)
    {
        struct base *b = &m->bases[idBmp];
        b->qtdMissoes++;
        m->qtdMissoesCumpridas++;

        for (int i = 0; i < b->presentes->cap; i++)
        {
            if (!b->presentes->flag[i])
                continue;

            struct heroi *h = &m->herois[i];

            if (!h->estado)
                continue;

            int risco = missao->perigo / (h->paciencia + h->experiencia + 1);

            if (risco > (rand() % 31))
            {
                // cria e insere na LEF o evento MORRE (agora, B)
                struct evento *novo_ev = malloc(sizeof(struct evento));
                if (!novo_ev)
                    return;

                novo_ev->tipo = 8;
                novo_ev->id_heroi = h->id;
                novo_ev->id_base = b->id;
                novo_ev->id_missao = missao->id;
                novo_ev->id_invasao = -1;

                if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
                    free(novo_ev);
            }
            else
                h->experiencia += 1;
        }
    }
    else
    {
        // cria e insere na LEF o evento MISSAO (t + 24*60, m)
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 9;
        novo_ev->id_heroi = -1;
        novo_ev->id_base = -1;
        novo_ev->id_missao = missao->id;
        novo_ev->id_invasao = -1;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + 24 * 60) < 0)
            free(novo_ev);
    }
}

void invade(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];
    struct invasao *invasao = &m->invasoes[ev->id_invasao];

    if (!b->estado)
        return;

    for (int i = 0; i < b->presentes->num; i++)
    {
        struct heroi *h = &m->herois[b->presentes->flag[i]];
        h->estado = 3;

        // cria e insere na LEF o evento RECUPERA_HEROI (t + i.tempo_penalidade_heroi, B)
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 11;
        novo_ev->id_heroi = h->id;
        novo_ev->id_base = b->id;
        novo_ev->id_missao = -1;
        novo_ev->id_invasao = invasao->id;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + invasao->tempo_penalidade_heroi) < 0)
            free(novo_ev);

        // remove da LEF o sai()
        struct fpnodo_t **p = &m->lef->prim;

        while (*p)
        {
            struct evento *e = (*p)->item;

            if (e->id_heroi == h->id && e->id_base == b->id && e->tipo == 6)
            {
                struct fpnodo_t *removido = *p;
                *p = removido->prox;
                free(removido->item);
                free(removido);
                m->lef->num--;
            }
            else
                p = &(*p)->prox;
        }

        // cria e insere na LEF o evento SAI ()
        struct evento *sai = malloc(sizeof(struct evento));
        if (!sai)
            return;

        sai->tipo = 6;
        sai->id_heroi = h->id;
        sai->id_base = b->id;
        sai->id_missao = -1;
        sai->id_invasao = invasao->id;

        if (fprio_insere(m->lef, sai, sai->tipo, t) < 0)
            free(sai);
    }

    b->estado = 0;

    // cria e insere na LEF o evento RECONSTROI_BASE (t + i.tempo_penalidade_base, B)
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 12;
    novo_ev->id_heroi = -1;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = invasao->id;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + invasao->tempo_penalidade_base) < 0)
        free(novo_ev);

    m->qtdInvasoes++;
}

void recupera_heroi(struct mundo *m, int t, struct evento *ev)
{
    (void)t;
    m->herois[ev->id_heroi].estado = 1;
}

void reconstroi_base(struct mundo *m, int t, struct evento *ev)
{
    (void)t;
    m->bases[ev->id_base].estado = 1;
}

void fim(struct mundo *m, int t, struct evento *ev)
{
    (void)ev;

    printf("%d: FIM\n", t);

    for (int i = 0; i < m->NHerois; i++)
    {
        struct heroi *h = &m->herois[i];

        printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS [ ", h->id,
               h->estado ? "VIVO" : "MORTO", h->paciencia, h->velocidade, h->experiencia);

        for (int j = 0; j < h->habilidades->cap; j++)
            if (h->habilidades->flag[j])
                printf("%d ", j);
        printf("]\n");
    }

    for (int i = 0; i < m->NBases; i++)
    {
        struct base *b = &m->bases[i];

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b->id,
               b->lotacao_maxima, b->qtdMaxHeroisFila, b->qtdMissoes);
    }

    int min = m->missoes[0].qtdTentativas, max = min, soma = 0;
    for (int i = 0; i < m->NMissoes; i++)
    {
        int q = m->missoes[i].qtdTentativas;
        if (q < min)
            min = q;
        if (q > max)
            max = q;
        soma += q;
    }

    printf("EVENTOS TRATADOS: %d\n", m->qtdEventos);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m->qtdMissoesCumpridas,
           m->NMissoes, 100.0 * m->qtdMissoesCumpridas / m->NMissoes);
    printf("INVASÕES REALIZADAS: %d/%d (%.1f%%)\n", m->qtdInvasoes,
           m->NInvasoes, 100.0 * m->qtdInvasoes / m->NInvasoes);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max,
           (float)soma / m->NMissoes);
    printf("TAXA MORTALIDADE: %.1f%%\n", 100.0 * m->qtdMortes / m->NHerois);
}