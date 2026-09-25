#ifndef ARQUIVO_PRODUTOS_H
#define ARQUIVO_PRODUTOS_H

#include <stddef.h>

#include "produto.h"

#define ARQUIVO_PRODUTOS_LIMITE 1000

typedef enum {
    ARQUIVO_PRODUTOS_OK,
    ARQUIVO_PRODUTOS_NAO_ENCONTRADO,
    ARQUIVO_PRODUTOS_DUPLICADO,
    ARQUIVO_PRODUTOS_ERRO,
    ARQUIVO_PRODUTOS_LIMITE_ATINGIDO
} ResultadoArquivoProdutos;

ResultadoArquivoProdutos arquivo_produtos_adicionar(const Produto *produto);
ResultadoArquivoProdutos arquivo_produtos_listar(
    Produto produtos[], size_t capacidade, size_t *quantidade);
ResultadoArquivoProdutos arquivo_produtos_buscar(int codigo, Produto *produto);
ResultadoArquivoProdutos arquivo_produtos_calcular_total(double *total);
ResultadoArquivoProdutos arquivo_produtos_atualizar_quantidade(int codigo, int quantidade);

#endif