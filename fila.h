#ifndef FILA
#define FILA
#include "entidades.c"

/* tirei as structs daqui e coloquei em entidades */

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria();

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi(struct fila_t *f);

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere(struct fila_t *f, void *item);

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira(struct fila_t *f);

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho(struct fila_t *f);

// Imprime o conteúdo da fila
void fila_imprime(struct fila_t *f);

#endif
