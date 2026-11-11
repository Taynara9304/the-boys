// programa principal do projeto "The Boys - 2026/2"
// Autor: Taynara da Silva Dias, GRR 20261372

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entidades.h"
#include "universo.h"
#include "simulacao.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main()
{
  srand(time(NULL));

  // iniciar o mundo
  struct mundo *m = calloc(1, sizeof(struct mundo));
  inicializar_mundo(m);

  // executar o laço de simulação
  simula_mundo(m);

  // destruir o mundo
  destroi_mundo(m);

  return (0);
}
