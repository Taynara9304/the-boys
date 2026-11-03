#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fila.h"

// aloca um inteiro no heap (necessário porque fila_destroi dá free nos itens)
static int *novo_int(int valor)
{
    int *p = malloc(sizeof(int));
    assert(p);
    *p = valor;
    return p;
}

int main()
{
    // 1. fila vazia
    struct fila_t *f = fila_cria();
    assert(f != NULL);
    assert(fila_tamanho(f) == 0);
    assert(fila_retira(f) == NULL); // retirar de fila vazia
    fila_imprime(f);                // não pode dar crash
    printf("[ok] fila vazia\n");

    // 2. inserção
    int *a = novo_int(10);
    int *b = novo_int(20);
    int *c = novo_int(30);

    assert(fila_insere(f, a) == 1);
    assert(fila_insere(f, b) == 2);
    assert(fila_insere(f, c) == 3);
    assert(fila_tamanho(f) == 3);
    printf("[ok] insercao\n");

    // 3. duplicado e argumentos inválidos
    assert(fila_insere(f, b) == -1);    // mesmo ponteiro
    assert(fila_tamanho(f) == 3);       // tamanho não muda
    assert(fila_insere(f, NULL) == -1); // item NULL
    assert(fila_insere(NULL, a) == -1); // fila NULL
    assert(fila_tamanho(NULL) == -1);
    assert(fila_retira(NULL) == NULL);
    assert(fila_destroi(NULL) == NULL);
    printf("[ok] duplicado e NULL\n");

    // 4. impressão
    printf("conteudo: ");
    fila_imprime(f);
    printf("\n");

    // 5. ordem FIFO (fila_retira não libera o item, então liberamos aqui)
    int *r = fila_retira(f);
    assert(r == a && *r == 10);
    free(r);
    assert(fila_tamanho(f) == 2);

    r = fila_retira(f);
    assert(r == b && *r == 20);
    free(r);

    r = fila_retira(f);
    assert(r == c && *r == 30);
    free(r);

    assert(fila_tamanho(f) == 0);
    assert(fila_retira(f) == NULL);
    printf("[ok] ordem FIFO\n");

    // 6. reutilizar a fila depois de esvaziar (testa se fim/prim foram resetados)
    int *d = novo_int(40);
    int *e = novo_int(50);
    assert(fila_insere(f, d) == 1);
    assert(fila_insere(f, e) == 2);
    r = fila_retira(f);
    assert(r == d);
    free(r);
    printf("[ok] reuso apos esvaziar\n");

    // 7. destruir com itens ainda dentro (e libera o 'e')
    f = fila_destroi(f);
    assert(f == NULL);
    printf("[ok] destroi\n");

    // 8. fila grande
    f = fila_cria();
    for (int i = 0; i < 1000; i++)
        assert(fila_insere(f, novo_int(i)) == i + 1);
    for (int i = 0; i < 1000; i++)
    {
        r = fila_retira(f);
        assert(r && *r == i);
        free(r);
    }
    assert(fila_tamanho(f) == 0);
    f = fila_destroi(f);
    printf("[ok] fila grande\n");

    printf("\nTodos os testes passaram!\n");
    return 0;
}