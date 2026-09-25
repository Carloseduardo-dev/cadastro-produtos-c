#include "arquivo_produtos.h"

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_ARQUIVO "produtos.txt"
#define NOME_TEMPORARIO "produtos.tmp"
#define NOME_BACKUP "produtos.bak"
#define TAMANHO_LINHA 256

static int ler_produto(FILE *arquivo, Produto *produto) {
    char linha[TAMANHO_LINHA];
    char extra;
    int campos_lidos;

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        return ferror(arquivo) ? -2 : 0;
    }
    if (strchr(linha, '\n') == NULL && !feof(arquivo)) {
        int caractere;
        while ((caractere = fgetc(arquivo)) != '\n' && caractere != EOF) {
        }
        return -1;
    }

    campos_lidos = sscanf(linha, "%d|%79[^|]|%49[^|]|%d|%lf %c",
                          &produto->codigo, produto->nome, produto->categoria,
                          &produto->quantidade, &produto->preco, &extra);
    if (campos_lidos != 5 || produto->codigo <= 0 || produto->quantidade < 0 ||
        !isfinite(produto->preco) || produto->preco < 0.0) {
        return -1;
    }
    return 1;
}

ResultadoArquivoProdutos arquivo_produtos_buscar(int codigo, Produto *produto) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "r");
    Produto atual;
    int resultado_leitura;

    if (arquivo == NULL) {
        return errno == ENOENT ? ARQUIVO_PRODUTOS_NAO_ENCONTRADO : ARQUIVO_PRODUTOS_ERRO;
    }
    while ((resultado_leitura = ler_produto(arquivo, &atual)) != 0) {
        if (resultado_leitura == -2) {
            fclose(arquivo);
            return ARQUIVO_PRODUTOS_ERRO;
        }
        if (resultado_leitura > 0 && atual.codigo == codigo) {
            if (produto != NULL) {
                *produto = atual;
            }
            return fclose(arquivo) == 0 ? ARQUIVO_PRODUTOS_OK : ARQUIVO_PRODUTOS_ERRO;
        }
    }

    if (fclose(arquivo) != 0) {
        return ARQUIVO_PRODUTOS_ERRO;
    }
    return ARQUIVO_PRODUTOS_NAO_ENCONTRADO;
}

ResultadoArquivoProdutos arquivo_produtos_adicionar(const Produto *produto) {
    FILE *arquivo;
    ResultadoArquivoProdutos resultado_busca;
    int erro_escrita;
    int erro_fechamento;

    resultado_busca = arquivo_produtos_buscar(produto->codigo, NULL);
    if (resultado_busca == ARQUIVO_PRODUTOS_OK) {
        return ARQUIVO_PRODUTOS_DUPLICADO;
    }
    if (resultado_busca != ARQUIVO_PRODUTOS_NAO_ENCONTRADO) {
        return resultado_busca;
    }

    arquivo = fopen(NOME_ARQUIVO, "a");
    if (arquivo == NULL) {
        return ARQUIVO_PRODUTOS_ERRO;
    }
    erro_escrita = fprintf(arquivo, "%d|%s|%s|%d|%.2f\n", produto->codigo, produto->nome,
                           produto->categoria, produto->quantidade, produto->preco) < 0;
    erro_fechamento = fclose(arquivo) != 0;
    return erro_escrita || erro_fechamento ? ARQUIVO_PRODUTOS_ERRO : ARQUIVO_PRODUTOS_OK;
}

ResultadoArquivoProdutos arquivo_produtos_listar(
    Produto produtos[], size_t capacidade, size_t *quantidade) {
    FILE *arquivo;
    Produto produto;
    int resultado_leitura;
    ResultadoArquivoProdutos resultado = ARQUIVO_PRODUTOS_OK;

    *quantidade = 0;
    arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        return errno == ENOENT ? ARQUIVO_PRODUTOS_OK : ARQUIVO_PRODUTOS_ERRO;
    }

    while ((resultado_leitura = ler_produto(arquivo, &produto)) != 0) {
        if (resultado_leitura == -2) {
            resultado = ARQUIVO_PRODUTOS_ERRO;
            break;
        }
        if (resultado_leitura < 0) {
            continue;
        }
        if (*quantidade == capacidade) {
            resultado = ARQUIVO_PRODUTOS_LIMITE_ATINGIDO;
            break;
        }
        produtos[(*quantidade)++] = produto;
    }

    if (fclose(arquivo) != 0 && resultado == ARQUIVO_PRODUTOS_OK) {
        resultado = ARQUIVO_PRODUTOS_ERRO;
    }
    return resultado;
}

ResultadoArquivoProdutos arquivo_produtos_calcular_total(double *total) {
    FILE *arquivo;
    Produto produto;
    int resultado_leitura;

    *total = 0.0;
    arquivo = fopen(NOME_ARQUIVO, "r");
    if (arquivo == NULL) {
        return errno == ENOENT ? ARQUIVO_PRODUTOS_OK : ARQUIVO_PRODUTOS_ERRO;
    }

    while ((resultado_leitura = ler_produto(arquivo, &produto)) != 0) {
        if (resultado_leitura == -2) {
            fclose(arquivo);
            return ARQUIVO_PRODUTOS_ERRO;
        }
        if (resultado_leitura > 0) {
            *total += produto.quantidade * produto.preco;
        }
    }
    return fclose(arquivo) == 0 ? ARQUIVO_PRODUTOS_OK : ARQUIVO_PRODUTOS_ERRO;
}

ResultadoArquivoProdutos arquivo_produtos_atualizar_quantidade(int codigo, int quantidade) {
    FILE *original;
    FILE *temporario;
    Produto produto;
    int resultado_leitura;
    int encontrado = 0;
    int erro_leitura;
    int erro_fechamento_original;
    int erro_fechamento_temporario;

    original = fopen(NOME_ARQUIVO, "r");
    if (original == NULL) {
        return errno == ENOENT ? ARQUIVO_PRODUTOS_NAO_ENCONTRADO : ARQUIVO_PRODUTOS_ERRO;
    }
    temporario = fopen(NOME_TEMPORARIO, "w");
    if (temporario == NULL) {
        fclose(original);
        return ARQUIVO_PRODUTOS_ERRO;
    }

    while ((resultado_leitura = ler_produto(original, &produto)) != 0) {
        if (resultado_leitura < 0) {
            fclose(original);
            fclose(temporario);
            remove(NOME_TEMPORARIO);
            return ARQUIVO_PRODUTOS_ERRO;
        }
        if (produto.codigo == codigo) {
            produto.quantidade = quantidade;
            encontrado = 1;
        }
        if (fprintf(temporario, "%d|%s|%s|%d|%.2f\n", produto.codigo, produto.nome,
                    produto.categoria, produto.quantidade, produto.preco) < 0) {
            fclose(original);
            fclose(temporario);
            remove(NOME_TEMPORARIO);
            return ARQUIVO_PRODUTOS_ERRO;
        }
    }

    erro_leitura = ferror(original);
    erro_fechamento_original = fclose(original) != 0;
    erro_fechamento_temporario = fclose(temporario) != 0;
    if (erro_leitura || erro_fechamento_original || erro_fechamento_temporario) {
        remove(NOME_TEMPORARIO);
        return ARQUIVO_PRODUTOS_ERRO;
    }
    if (!encontrado) {
        remove(NOME_TEMPORARIO);
        return ARQUIVO_PRODUTOS_NAO_ENCONTRADO;
    }

    remove(NOME_BACKUP);
    if (rename(NOME_ARQUIVO, NOME_BACKUP) != 0) {
        remove(NOME_TEMPORARIO);
        return ARQUIVO_PRODUTOS_ERRO;
    }
    if (rename(NOME_TEMPORARIO, NOME_ARQUIVO) != 0) {
        rename(NOME_BACKUP, NOME_ARQUIVO);
        remove(NOME_TEMPORARIO);
        return ARQUIVO_PRODUTOS_ERRO;
    }
    remove(NOME_BACKUP);
    return ARQUIVO_PRODUTOS_OK;
}