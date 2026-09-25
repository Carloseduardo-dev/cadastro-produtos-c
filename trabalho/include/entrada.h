#ifndef ENTRADA_H
#define ENTRADA_H

#include <stddef.h>

int entrada_ler_inteiro(const char *mensagem, int minimo, int *valor);
int entrada_ler_preco(const char *mensagem, double *valor);
int entrada_ler_texto(const char *mensagem, char *destino, size_t tamanho);

#endif