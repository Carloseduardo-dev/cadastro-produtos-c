# Cadastro e consulta de produtos em C

Programa acadêmico desenvolvido em C para armazenar e consultar produtos de uma loja de informática, com os registros mantidos em arquivo.

## Funcionalidades

- Cadastrar produtos com código, nome, categoria, quantidade e preço unitário.
- Listar e buscar produtos cadastrados.
- Calcular o valor total do estoque.
- Atualizar a quantidade de um produto.

## Compilação e execução

Com Clang instalado, abra o terminal nesta pasta e execute:

```powershell
clang produtos.c -o produtos.exe
.\produtos.exe
```

O programa cria ou atualiza `produtos.txt` no diretório de execução. O arquivo incluído contém os dados de exemplo usados nas evidências.

## Arquivos

- `produtos.c`: código-fonte.
- `produtos.txt`: arquivo de dados de exemplo usado pelo programa.
- `Cadastro_de_Produtos.pdf`: código e evidências da execução.
- `evidencias/`: imagens PNG dos testes do menu e do arquivo criado.
- [`trabalho_programacao_software_basico_c.pdf`](../trabalho_programacao_software_basico_c.pdf): enunciado da atividade.