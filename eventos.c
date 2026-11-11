#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "fila.h"
#include "conjunto.h"
#include "fprio.h"
#include "eventos.h"
#include "entidades.h"

void chega(struct mundo *m, int t, struct evento *ev)
{
    int esp = 0;

    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    h->id_base = b->id;

    if ((b->presentes->num < b->lotacao_maxima) && fila_tamanho(b->fila_de_espera) == 0)
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

void espera(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    if (b->fila_de_espera->num + 1 > b->qtdMaxHeroisFila)
        b->qtdMaxHeroisFila = b->fila_de_espera->num + 1;

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", t, ev->id_heroi, b->id,
           fila_tamanho(b->fila_de_espera));

    fila_insere(b->fila_de_espera, &m->herois[ev->id_heroi]);

    // insere avisa(t, b) na LEF
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

void desiste(struct mundo *m, int t, struct evento *ev)
{
    printf("%6d: DESIST HEROI %2d BASE %d\n", t, ev->id_heroi, ev->id_base);

    // insere na LEF o viaja()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 7;
    novo_ev->id_heroi = ev->id_heroi;
    novo_ev->id_base = rand() % m->NBases;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
        free(novo_ev);
}

void avisa(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", t, b->id,
           b->presentes->num, b->lotacao_maxima);
    for (struct fila_nodo_t *n = b->fila_de_espera->prim; n; n = n->prox)
        printf("%2d ", ((struct heroi *)n->item)->id);
    printf("]\n");

    while ((b->presentes->num < b->lotacao_maxima) && (b->fila_de_espera->num != 0))
    {
        struct heroi *heroi_removido = fila_retira(b->fila_de_espera);
        cjto_insere(b->presentes, heroi_removido->id);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", t, b->id, heroi_removido->id);

        // insere na LEF o entra()
        struct evento *novo_ev = malloc(sizeof(struct evento));
        if (!novo_ev)
            return;

        novo_ev->tipo = 5;
        novo_ev->id_heroi = heroi_removido->id;
        novo_ev->id_base = b->id;
        novo_ev->id_missao = -1;
        novo_ev->id_invasao = -1;

        if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t) < 0)
            free(novo_ev);
    }
}

void entra(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];
    struct base *b = &m->bases[ev->id_base];

    int tpb = 15 + h->paciencia * ((rand() % 20) + 1);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", t, h->id, b->id,
           b->presentes->num, b->lotacao_maxima, t + tpb);

    // insere o evento sai()
    struct evento *novo_ev = malloc(sizeof(struct evento));
    if (!novo_ev)
        return;

    novo_ev->tipo = 6;
    novo_ev->id_heroi = h->id;
    novo_ev->id_base = b->id;
    novo_ev->id_missao = -1;
    novo_ev->id_invasao = -1;

    if (fprio_insere(m->lef, novo_ev, novo_ev->tipo, t + tpb) < 0)
        free(novo_ev);
}

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

    // insere na LEF o viaja()
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

    // insere na LEF o avisa()
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

void viaja(struct mundo *m, int t, struct evento *ev)
{
    struct heroi *h = &m->herois[ev->id_heroi];

    struct base *b_origem = &m->bases[h->id_base];
    struct base *b_destino = &m->bases[ev->id_base];

    int distancia = sqrt(pow((b_origem->local.x - b_destino->local.x), 2) + pow((b_origem->local.y - b_destino->local.y), 2));
    int duracao = distancia / h->velocidade;

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", t, h->id,
           b_origem->id, b_destino->id, distancia, h->velocidade, t + duracao);

    // insere na LEF chega(t + duração, h, d)
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

void morre(struct mundo *m, int t, struct evento *ev)
{
    struct base *b = &m->bases[ev->id_base];
    struct heroi *h = &m->herois[ev->id_heroi];
    m->qtdMortes++;

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

// imprime um conjunto no formato "[ a b c ]" (sem quebra de linha)
static void imprime_conjunto(struct cjto_t *c)
{
    printf("[ ");
    for (int i = 0; i < c->cap; i++)
        if (c->flag[i])
            printf("%d ", i);
    printf("]");
}

// distância cartesiana (em metros) entre a base e o local da missão
static int distancia_base_missao(struct base *b, struct missao *ms)
{
    double dx = ms->local.x - b->local.x;
    double dy = ms->local.y - b->local.y;

    return (int)sqrt(dx * dx + dy * dy);
}

// bases ordenadas por distância crescente (desempate pelo id)
struct base_dist
{
    int id;
    int dist;
};

static int compara_base_dist(const void *a, const void *b)
{
    const struct base_dist *x = a, *y = b;

    if (x->dist != y->dist)
        return x->dist - y->dist;
    return x->id - y->id;
}

void missao(struct mundo *m, int t, struct evento *ev)
{
    struct missao *missao = &m->missoes[ev->id_missao];

    missao->qtdTentativas++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: ", t, missao->id, missao->qtdTentativas);
    imprime_conjunto(missao->habilidades);
    printf("\n");

    // calcula a distância de cada base ativa ao local da missão e ordena
    struct base_dist *ordem = malloc(m->NBases * sizeof(struct base_dist));
    if (!ordem)
        return;

    int n = 0;
    for (int i = 0; i < m->NBases; i++)
    {
        if (m->bases[i].estado != 1)
            continue;

        ordem[n].id = i;
        ordem[n].dist = distancia_base_missao(&m->bases[i], missao);
        n++;
    }

    qsort(ordem, n, sizeof(struct base_dist), compara_base_dist);

    // analisa as bases em ordem crescente de distância até achar uma apta
    int idBmp = -1;
    struct cjto_t *habsBmp = NULL;

    for (int k = 0; k < n && idBmp == -1; k++)
    {
        struct base *b = &m->bases[ordem[k].id];

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", t, missao->id, b->id, ordem[k].dist);
        for (int j = 0; j < b->presentes->cap; j++)
            if (b->presentes->flag[j] && m->herois[j].estado == 1)
                printf("%d ", j);
        printf("]\n");

        // união das habilidades dos heróis ativos presentes na base
        struct cjto_t *uniao = cjto_cria(m->NHabilidades);
        if (!uniao)
            continue;

        for (int j = 0; j < b->presentes->cap; j++)
        {
            if (!b->presentes->flag[j] || m->herois[j].estado != 1)
                continue;

            struct heroi *h = &m->herois[j];

            printf("%6d: MISSAO %d HAB HEROI %2d: ", t, missao->id, h->id);
            imprime_conjunto(h->habilidades);
            printf("\n");

            struct cjto_t *novo = cjto_uniao(uniao, h->habilidades);
            cjto_destroi(uniao);
            uniao = novo;

            if (!uniao)
                break;
        }

        if (!uniao)
            continue;

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", t, missao->id, b->id);
        imprime_conjunto(uniao);
        printf("\n");

        if (cjto_contem(uniao, missao->habilidades) == 1)
        {
            idBmp = b->id;
            habsBmp = uniao;
        }
        else
            cjto_destroi(uniao);
    }

    free(ordem);

    if (idBmp != -1)
    {
        struct base *b = &m->bases[idBmp];
        b->qtdMissoes++;
        m->qtdMissoesCumpridas++;

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: ", t, missao->id, b->id);
        imprime_conjunto(habsBmp);
        printf("\n");
        cjto_destroi(habsBmp);

        for (int i = 0; i < b->presentes->cap; i++)
        {
            if (!b->presentes->flag[i])
                continue;

            struct heroi *h = &m->herois[i];

            // só heróis ativos participam (mortos e debilitados não)
            if (h->estado != 1)
                continue;

            double risco = missao->perigo / (h->paciencia + h->experiencia + 1);

            if (risco > (rand() % 31))
            {
                // cria e insere na LEF o evento MORRE (agora, H, B)
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
        printf("%6d: MISSAO %d IMPOSSIVEL\n", t, missao->id);

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

    printf("%6d: INVADE BASE %d INVADE %d\n", t, b->id, invasao->id);

    printf("%6d: INVADE %d BASE %d HEROIS INATIVOS [", t, invasao->id, b->id);
    int primeiro = 1;
    for (int i = 0; i < b->presentes->cap; i++)
    {
        if (!b->presentes->flag[i])
            continue;
        printf("%s%d", primeiro ? "" : ", ", i);
        primeiro = 0;
    }
    printf("]\n");

    for (int i = 0; i < b->presentes->cap; i++)
    {
        if (!b->presentes->flag[i])
            continue;
        struct heroi *h = &m->herois[i];

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

    printf("%6d: INVADE %d BASE %d INATIVA\n", t, invasao->id, b->id);

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

    printf("%6d: FIM\n", t);

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