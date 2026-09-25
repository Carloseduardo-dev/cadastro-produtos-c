# Demonstração web

Interface estática para testar no navegador o mesmo módulo C de persistência do projeto. O C é compilado para WebAssembly com Emscripten; `produtos.txt` fica apenas no sistema de arquivos em memória da instância WebAssembly e desaparece ao recarregar a página.

## Organização

- `app.js`: inicialização e conexão entre os componentes.
- `services/product-service.js`: chamadas JavaScript para a API C/WebAssembly.
- `components/`: lista, resumo do estoque, mensagens e formulários de cadastro, busca e atualização.
- `wasm/`: módulos compilados para o navegador.

## Build local

Instale o [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) e, nesta pasta, execute no PowerShell:

```powershell
.\build.ps1
python -m http.server 8000
```

Abra `http://localhost:8000`. Não abra `index.html` diretamente como arquivo, pois o navegador precisa carregar o módulo WebAssembly por HTTP.

## Publicar na Vercel

Crie um projeto Vercel conectado ao repositório e configure:

- Root Directory: `trabalho/demo`
- Framework Preset: `Other`
- Build Command: vazio
- Output Directory: `.`

O projeto publica os arquivos estáticos desta pasta, inclusive `wasm/produtos.js` e `wasm/produtos.wasm`. Não usa API, serviço externo, banco de dados, cookies ou armazenamento local do navegador.