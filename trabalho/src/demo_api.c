#include "arquivo_produtos.h"

#include <emscripten/emscripten.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define TAMANHO_JSON (128 * 1024)

static Produto produtos[ARQUIVO_PRODUTOS_LIMITE];
static char json[TAMANHO_JSON];

static int adicionar_texto_json(size_t *posicao, const char *texto) {
    const unsigned char *caractere = (const unsigned char *)texto;

    while (*caractere != '\0') {
        char escape[7];
        int tamanho_escape = 0;

        switch (*caractere) {
            case '"':
                escape[0] = '\\';
                escape[1] = '"';
                tamanho_escape = 2;
                break;
            case '\\':
                escape[0] = '\\';
                escape[1] = '\\';
                tamanho_escape = 2;
                break;
            case '\n':
                escape[0] = '\\';
                escape[1] = 'n';
                tamanho_escape = 2;
                break;
            case '\r':
                escape[0] = '\\';
                escape[1] = 'r';
                tamanho_escape = 2;
                break;
            case '\t':
                escape[0] = '\\';
                escape[1] = 't';
                tamanho_escape = 2;
                break;
            default:
                if (*caractere < 0x20) {
                    tamanho_escape = snprintf(escape, sizeof(escape), "\\u%04x", *caractere);
                } else {
                    escape[0] = (char)*caractere;
                    tamanho_escape = 1;
                }
        }

        if (tamanho_escape < 0 || (size_t)tamanho_escape >= TAMANHO_JSON - *posicao) {
            return 0;
        }
        memcpy(json + *posicao, escape, (size_t)tamanho_escape);
        *posicao += (size_t)tamanho_escape;
        caractere++;
    }
    return 1;
}

static int adicionar_produto_json(size_t *posicao, const Produto *produto) {
    int tamanho;
    size_t restante = TAMANHO_JSON - *posicao;

    tamanho = snprintf(json + *posicao, restante,
                       "{\"codigo\":%d,\"nome\":\"", produto->codigo);
    if (tamanho < 0 || (size_t)tamanho >= restante) {
        return 0;
    }
    *posicao += (size_t)tamanho;
    if (!adicionar_texto_json(posicao, produto->nome)) {
        return 0;
    }

    restante = TAMANHO_JSON - *posicao;
    tamanho = snprintf(json + *posicao, restante, "\",\"categoria\":\"");
    if (tamanho < 0 || (size_t)tamanho >= restante) {
        return 0;
    }
    *posicao += (size_t)tamanho;
    if (!adicionar_texto_json(posicao, produto->categoria)) {
        return 0;
    }

    restante = TAMANHO_JSON - *posicao;
    tamanho = snprintf(json + *posicao, restante,
                       "\",\"quantidade\":%d,\"preco\":%.2f}",
                       produto->quantidade, produto->preco);
    if (tamanho < 0 || (size_t)tamanho >= restante) {
        return 0;
    }
    *posicao += (size_t)tamanho;
    return 1;
}

EMSCRIPTEN_KEEPALIVE
int demo_adicionar(int codigo, const char *nome, const char *categoria,
                   int quantidade, double preco) {
    Produto produto = {0};

    if (codigo <= 0 || nome == NULL || categoria == NULL || nome[0] == '\0' ||
        categoria[0] == '\0' || strchr(nome, '|') != NULL || strchr(categoria, '|') != NULL ||
        quantidade < 0 || !isfinite(preco) || preco < 0.0) {
        return ARQUIVO_PRODUTOS_ERRO;
    }

    produto.codigo = codigo;
    produto.quantidade = quantidade;
    produto.preco = preco;
    snprintf(produto.nome, sizeof(produto.nome), "%s", nome);
    snprintf(produto.categoria, sizeof(produto.categoria), "%s", categoria);
    if (strlen(nome) >= sizeof(produto.nome) || strlen(categoria) >= sizeof(produto.categoria)) {
        return ARQUIVO_PRODUTOS_ERRO;
    }
    return arquivo_produtos_adicionar(&produto);
}

EMSCRIPTEN_KEEPALIVE
const char *demo_listar_json(void) {
    size_t quantidade;
    size_t posicao = 0;
    size_t indice;
    ResultadoArquivoProdutos resultado;

    resultado = arquivo_produtos_listar(produtos, ARQUIVO_PRODUTOS_LIMITE, &quantidade);
    if (resultado != ARQUIVO_PRODUTOS_OK) {
        return "{\"erro\":\"Nao foi possivel listar os produtos.\"}";
    }
    json[posicao++] = '[';
    for (indice = 0; indice < quantidade; indice++) {
        if (indice > 0) {
            json[posicao++] = ',';
        }
        if (!adicionar_produto_json(&posicao, &produtos[indice])) {
            return "{\"erro\":\"A lista excedeu o limite da demonstracao.\"}";
        }
    }
    json[posicao++] = ']';
    json[posicao] = '\0';
    return json;
}

EMSCRIPTEN_KEEPALIVE
const char *demo_buscar_json(int codigo) {
    Produto produto;
    size_t posicao = 0;
    ResultadoArquivoProdutos resultado = arquivo_produtos_buscar(codigo, &produto);

    if (resultado == ARQUIVO_PRODUTOS_NAO_ENCONTRADO) {
        return "null";
    }
    if (resultado != ARQUIVO_PRODUTOS_OK) {
        return "{\"erro\":\"Nao foi possivel buscar o produto.\"}";
    }
    if (!adicionar_produto_json(&posicao, &produto)) {
        return "{\"erro\":\"Nao foi possivel formatar o produto.\"}";
    }
    json[posicao] = '\0';
    return json;
}

EMSCRIPTEN_KEEPALIVE
double demo_calcular_total(void) {
    double total;

    if (arquivo_produtos_calcular_total(&total) != ARQUIVO_PRODUTOS_OK) {
        return NAN;
    }
    return total;
}

EMSCRIPTEN_KEEPALIVE
int demo_atualizar_quantidade(int codigo, int quantidade) {
    if (codigo <= 0 || quantidade < 0) {
        return ARQUIVO_PRODUTOS_ERRO;
    }
    return arquivo_produtos_atualizar_quantidade(codigo, quantidade);
}