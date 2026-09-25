#include "entrada.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ler_linha(const char *mensagem, char *destino, size_t tamanho) {
    size_t comprimento;

    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    comprimento = strlen(destino);
    if (comprimento > 0 && destino[comprimento - 1] == '\n') {
        destino[--comprimento] = '\0';
    } else {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
        printf("Entrada muito longa. Tente novamente.\n");
        return -1;
    }

    if (comprimento > 0 && destino[comprimento - 1] == '\r') {
        destino[comprimento - 1] = '\0';
    }
    return 1;
}

int entrada_ler_inteiro(const char *mensagem, int minimo, int *valor) {
    char linha[80];
    char *fim;
    long numero;
    int resultado;

    for (;;) {
        resultado = ler_linha(mensagem, linha, sizeof(linha));
        if (resultado == 0) {
            return 0;
        }
        if (resultado < 0 || linha[0] == '\0') {
            printf("Digite um numero inteiro valido.\n");
            continue;
        }

        errno = 0;
        numero = strtol(linha, &fim, 10);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }
        if (errno == 0 && fim != linha && *fim == '\0' && numero >= minimo && numero <= INT_MAX) {
            *valor = (int)numero;
            return 1;
        }
        printf("Valor invalido. Digite um inteiro maior ou igual a %d.\n", minimo);
    }
}

int entrada_ler_preco(const char *mensagem, double *valor) {
    char linha[80];
    char *fim;
    size_t indice;
    int resultado;
    double numero;

    for (;;) {
        resultado = ler_linha(mensagem, linha, sizeof(linha));
        if (resultado == 0) {
            return 0;
        }
        if (resultado < 0 || linha[0] == '\0') {
            printf("Digite um preco valido, por exemplo 129.90.\n");
            continue;
        }
        for (indice = 0; linha[indice] != '\0'; indice++) {
            if (linha[indice] == ',') {
                linha[indice] = '.';
            }
        }

        errno = 0;
        numero = strtod(linha, &fim);
        while (isspace((unsigned char)*fim)) {
            fim++;
        }
        if (errno == 0 && fim != linha && *fim == '\0' && isfinite(numero) && numero >= 0.0) {
            *valor = numero;
            return 1;
        }
        printf("Preco invalido. Use um numero maior ou igual a zero.\n");
    }
}

int entrada_ler_texto(const char *mensagem, char *destino, size_t tamanho) {
    int resultado;

    for (;;) {
        resultado = ler_linha(mensagem, destino, tamanho);
        if (resultado == 0) {
            return 0;
        }
        if (resultado < 0) {
            continue;
        }
        if (destino[0] == '\0' || strchr(destino, '|') != NULL) {
            printf("O campo nao pode ficar vazio nem conter o caractere |.\n");
            continue;
        }
        return 1;
    }
}