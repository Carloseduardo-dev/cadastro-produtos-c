#include "menu.h"

#include "arquivo_produtos.h"
#include "entrada.h"

#include <stdio.h>

static void imprimir_produto(const Produto *produto) {
    printf("Codigo: %d | Nome: %s | Categoria: %s | Quantidade: %d | Preco: R$ %.2f\n",
           produto->codigo, produto->nome, produto->categoria,
           produto->quantidade, produto->preco);
}

static void cadastrar_produto(void) {
    Produto produto;
    ResultadoArquivoProdutos resultado;

    printf("\n=== Cadastro de produto ===\n");
    if (!entrada_ler_inteiro("Codigo: ", 1, &produto.codigo) ||
        !entrada_ler_texto("Nome: ", produto.nome, sizeof(produto.nome)) ||
        !entrada_ler_texto("Categoria: ", produto.categoria, sizeof(produto.categoria)) ||
        !entrada_ler_inteiro("Quantidade em estoque: ", 0, &produto.quantidade) ||
        !entrada_ler_preco("Preco unitario: R$ ", &produto.preco)) {
        printf("Cadastro cancelado.\n");
        return;
    }

    resultado = arquivo_produtos_adicionar(&produto);
    if (resultado == ARQUIVO_PRODUTOS_DUPLICADO) {
        printf("Ja existe um produto com esse codigo.\n");
    } else if (resultado == ARQUIVO_PRODUTOS_OK) {
        printf("Produto cadastrado com sucesso.\n");
    } else {
        printf("Nao foi possivel gravar o produto no arquivo.\n");
    }
}

static void listar_produtos(void) {
    Produto produtos[ARQUIVO_PRODUTOS_LIMITE];
    size_t quantidade;
    size_t indice;
    ResultadoArquivoProdutos resultado;

    printf("\n=== Produtos cadastrados ===\n");
    resultado = arquivo_produtos_listar(produtos, ARQUIVO_PRODUTOS_LIMITE, &quantidade);
    if (resultado == ARQUIVO_PRODUTOS_ERRO) {
        printf("Erro durante a leitura do arquivo.\n");
        return;
    }
    if (resultado == ARQUIVO_PRODUTOS_LIMITE_ATINGIDO) {
        printf("O limite de produtos para listagem foi atingido.\n");
    }
    if (quantidade == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    for (indice = 0; indice < quantidade; indice++) {
        imprimir_produto(&produtos[indice]);
    }
}

static void buscar_produto(void) {
    Produto produto;
    int codigo;
    ResultadoArquivoProdutos resultado;

    printf("\n=== Busca de produto ===\n");
    if (!entrada_ler_inteiro("Codigo para buscar: ", 1, &codigo)) {
        printf("Busca cancelada.\n");
        return;
    }

    resultado = arquivo_produtos_buscar(codigo, &produto);
    if (resultado == ARQUIVO_PRODUTOS_OK) {
        printf("Produto encontrado:\n");
        imprimir_produto(&produto);
    } else if (resultado == ARQUIVO_PRODUTOS_NAO_ENCONTRADO) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
    } else {
        printf("Erro durante a leitura do arquivo.\n");
    }
}

static void calcular_valor_estoque(void) {
    double total;

    printf("\n=== Valor total do estoque ===\n");
    if (arquivo_produtos_calcular_total(&total) != ARQUIVO_PRODUTOS_OK) {
        printf("Erro durante a leitura do arquivo.\n");
        return;
    }
    printf("Valor total do estoque: R$ %.2f\n", total);
}

static void atualizar_quantidade(void) {
    int codigo;
    int quantidade;
    ResultadoArquivoProdutos resultado;

    printf("\n=== Atualizar quantidade em estoque ===\n");
    if (!entrada_ler_inteiro("Codigo do produto: ", 1, &codigo) ||
        !entrada_ler_inteiro("Nova quantidade: ", 0, &quantidade)) {
        printf("Atualizacao cancelada.\n");
        return;
    }

    resultado = arquivo_produtos_atualizar_quantidade(codigo, quantidade);
    if (resultado == ARQUIVO_PRODUTOS_OK) {
        printf("Quantidade atualizada com sucesso.\n");
    } else if (resultado == ARQUIVO_PRODUTOS_NAO_ENCONTRADO) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
    } else {
        printf("Nao foi possivel atualizar o arquivo de produtos.\n");
    }
}

static void exibir_menu(void) {
    printf("\n===== CONTROLE DE PRODUTOS =====\n");
    printf("1. Cadastrar produto\n");
    printf("2. Listar produtos\n");
    printf("3. Buscar produto\n");
    printf("4. Calcular valor do estoque\n");
    printf("5. Atualizar quantidade\n");
    printf("6. Sair\n");
}

void menu_executar(void) {
    int opcao;

    for (;;) {
        exibir_menu();
        if (!entrada_ler_inteiro("Escolha uma opcao: ", 1, &opcao)) {
            printf("\nPrograma encerrado.\n");
            return;
        }
        switch (opcao) {
            case 1:
                cadastrar_produto();
                break;
            case 2:
                listar_produtos();
                break;
            case 3:
                buscar_produto();
                break;
            case 4:
                calcular_valor_estoque();
                break;
            case 5:
                atualizar_quantidade();
                break;
            case 6:
                printf("Programa encerrado.\n");
                return;
            default:
                printf("Opcao invalida. Escolha uma opcao de 1 a 6.\n");
        }
    }
}