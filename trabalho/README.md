# Cadastro e consulta de produtos em C

Programa acadêmico desenvolvido em C para armazenar e consultar produtos de uma loja de informática, com os registros mantidos em arquivo.

## Funcionalidades

- Cadastrar produtos com código, nome, categoria, quantidade e preço unitário.
- Listar e buscar produtos cadastrados.
- Calcular o valor total do estoque.
- Atualizar a quantidade de um produto.

## Compilação e execução

Com Clang instalado, abra o terminal nesta pasta e compile os módulos:

```powershell
clang src\main.c src\menu.c src\entrada.c src\arquivo_produtos.c -I include -o produtos.exe
.\produtos.exe
```

O programa cria ou atualiza `produtos.txt` no diretório de execução. O arquivo incluído contém os dados de exemplo usados nas evidências.

## Arquivos

- `include/`: definição do produto e contratos entre módulos.
- `src/`: inicialização, interface/menus, entrada de dados e persistência em arquivo.
- `produtos.txt`: arquivo de dados de exemplo usado pelo programa.
- `Cadastro_de_Produtos.pdf`: código e evidências da execução.
- `evidencias/`: imagens PNG dos testes do menu e do arquivo criado.
- [`trabalho_programacao_software_basico_c.pdf`](../trabalho_programacao_software_basico_c.pdf): enunciado da atividade.

## Demonstração web

A interface para navegador fica em [`demo/`](demo/). Ela usa os módulos C compilados para WebAssembly e mantém os dados somente na memória da sessão. Consulte [`demo/README.md`](demo/README.md) para compilar e configurar a publicação estática na Vercel.