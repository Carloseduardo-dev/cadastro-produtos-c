#ifndef PRODUTO_H
#define PRODUTO_H

#define PRODUTO_TAMANHO_NOME 80
#define PRODUTO_TAMANHO_CATEGORIA 50

typedef struct {
    int codigo;
    char nome[PRODUTO_TAMANHO_NOME];
    char categoria[PRODUTO_TAMANHO_CATEGORIA];
    int quantidade;
    double preco;
} Produto;

#endif