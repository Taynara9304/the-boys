// programa principal do projeto "The Boys - 2024/2"
// Autor: Taynara da Silva Dias, GRR 20261372

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "entidades.h"
#include "eventos.h"
#include "conjunto.h"
#include "fila.h"
#include "fprio.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main()
{
  srand(time(NULL));

  // iniciar o mundo
  struct mundo m;
  inicializar_mundo(&m);

  // executar o laço de simulação

  // destruir o mundo

  return (0);
}
