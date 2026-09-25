# Cadastro e controle de produtos

Projeto acadêmico em C para cadastro, consulta e controle de estoque de produtos. A aplicação de terminal grava os dados em `produtos.txt`; uma demonstração web reutiliza as rotinas C compiladas para WebAssembly.

## Funcionalidades

- Cadastrar, listar e buscar produtos por código.
- Calcular o valor total do estoque.
- Atualizar a quantidade de um produto.
- Validar dados de entrada e evitar códigos duplicados.
- Testar as mesmas operações em uma interface web responsiva.

## Estrutura

```text
trabalho/
├── include/       contratos e estrutura Produto
├── src/           aplicação C e API usada pela demonstração
├── demo/          interface web e build WebAssembly
├── evidencias/    capturas dos testes do programa de terminal
├── produtos.txt   dados de exemplo do programa C
└── Cadastro_de_Produtos.pdf
```

O código foi dividido entre entrada de dados, menu e acesso ao arquivo. A interface web fica isolada em `trabalho/demo/` e chama as operações C por meio de WebAssembly.

## Executar o programa C

Com Clang instalado, abra um terminal na pasta `trabalho` e execute:

```powershell
clang src\main.c src\menu.c src\entrada.c src\arquivo_produtos.c -I include -o produtos.exe
.\produtos.exe
```

O arquivo `trabalho/produtos.txt` é criado ou atualizado no diretório de execução.

## Executar a demonstração web

Instale e ative o [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). No PowerShell, a partir de `trabalho/demo`, compile o WebAssembly e inicie um servidor local:

```powershell
.\build.ps1
python -m http.server 8000
```

Acesse `http://localhost:8000`. Os detalhes de compilação e publicação estão no [README da demonstração](trabalho/demo/README.md).

### Publicação na Vercel

A demonstração é uma página estática. Configure o projeto da Vercel com:

- **Root Directory:** `trabalho/demo`
- **Framework Preset:** `Other`
- **Build Command:** vazio
- **Output Directory:** `.`

Os arquivos compilados `wasm/produtos.js` e `wasm/produtos.wasm` precisam estar no repositório para a publicação sem build. Na versão web, o arquivo de dados existe apenas na memória da sessão e é apagado ao recarregar a página; persistência entre visitas exigiria um backend e armazenamento permanente.

## Trabalho acadêmico

O [PDF com o código-fonte e as evidências](trabalho/Cadastro_de_Produtos.pdf) documenta as execuções do menu e o arquivo criado. O [enunciado original](trabalho_programacao_software_basico_c.pdf) está na raiz do repositório.
