import criarModulo from "./wasm/produtos.js";

const moeda = new Intl.NumberFormat("pt-BR", { style: "currency", currency: "BRL" });
const linhasProdutos = document.querySelector("#product-rows");
const estadoVazio = document.querySelector("#empty-state");
const mensagem = document.querySelector("#status-message");
const botoesDeAcao = document.querySelectorAll(".controls button");
let modulo;

function exibirMensagem(texto, tipo = "success") {
  mensagem.textContent = texto;
  mensagem.dataset.kind = tipo;
}

function adicionarCelula(linha, texto, rotulo, classe) {
  const celula = document.createElement("td");
  celula.dataset.label = rotulo;
  if (classe) celula.className = classe;
  celula.textContent = texto;
  linha.append(celula);
  return celula;
}

function carregarProdutos() {
  return JSON.parse(modulo.ccall("demo_listar_json", "string", [], []));
}

function atualizarPainel() {
  const produtos = carregarProdutos();
  if (!Array.isArray(produtos)) throw new Error("Não foi possível ler os produtos.");

  linhasProdutos.replaceChildren();
  for (const produto of produtos) {
    const linha = document.createElement("tr");
    linha.dataset.codigo = String(produto.codigo);
    adicionarCelula(linha, String(produto.codigo), "Código", "product-code");
    adicionarCelula(linha, produto.nome, "Produto", "product-name");

    const categoria = document.createElement("td");
    categoria.dataset.label = "Categoria";
    const etiqueta = document.createElement("span");
    etiqueta.className = "category-tag";
    etiqueta.textContent = produto.categoria;
    categoria.append(etiqueta);
    linha.append(categoria);

    adicionarCelula(linha, String(produto.quantidade), "Quantidade", "numeric-cell");
    adicionarCelula(linha, moeda.format(produto.preco), "Preço unitário", "numeric-cell");

    const acoes = document.createElement("td");
    acoes.dataset.label = "Ação";
    const botaoAtualizar = document.createElement("button");
    botaoAtualizar.className = "row-action";
    botaoAtualizar.type = "button";
    botaoAtualizar.textContent = "Alterar estoque";
    botaoAtualizar.addEventListener("click", () => {
      const formulario = document.querySelector("#update-form");
      formulario.elements.codigo.value = produto.codigo;
      formulario.elements.quantidade.focus();
      formulario.scrollIntoView({ behavior: "smooth", block: "center" });
    });
    acoes.append(botaoAtualizar);
    linha.append(acoes);
    linhasProdutos.append(linha);
  }

  const quantidade = produtos.length;
  const total = modulo.ccall("demo_calcular_total", "number", [], []);
  if (!Number.isFinite(total)) throw new Error("Não foi possível calcular o valor do estoque.");

  document.querySelector("#summary-count").textContent = String(quantidade).padStart(2, "0");
  document.querySelector("#summary-total").textContent = moeda.format(total);
  document.querySelector("#catalogue-count").textContent = `${quantidade} ${quantidade === 1 ? "produto" : "produtos"}`;
  estadoVazio.hidden = quantidade > 0;
}

function obterProduto(resultado) {
  if (resultado === 0) return "Produto cadastrado com sucesso.";
  if (resultado === 2) return "Já existe um produto com esse código.";
  return "Não foi possível cadastrar o produto.";
}

document.querySelector("#add-form").addEventListener("submit", (evento) => {
  evento.preventDefault();
  const formulario = evento.currentTarget;
  const dados = new FormData(formulario);
  const resultado = modulo.ccall(
    "demo_adicionar",
    "number",
    ["number", "string", "string", "number", "number"],
    [Number(dados.get("codigo")), dados.get("nome"), dados.get("categoria"),
      Number(dados.get("quantidade")), Number(dados.get("preco"))],
  );
  exibirMensagem(obterProduto(resultado), resultado === 0 ? "success" : "error");
  if (resultado === 0) {
    formulario.reset();
    atualizarPainel();
  }
});

document.querySelector("#search-form").addEventListener("submit", (evento) => {
  evento.preventDefault();
  const codigo = Number(new FormData(evento.currentTarget).get("codigo"));
  const produto = JSON.parse(modulo.ccall("demo_buscar_json", "string", ["number"], [codigo]));
  if (produto === null) {
    exibirMensagem(`Nenhum produto encontrado com o código ${codigo}.`, "error");
    return;
  }
  if (produto.erro) {
    exibirMensagem(produto.erro, "error");
    return;
  }
  exibirMensagem(`Encontrado: ${produto.nome} · ${produto.categoria} · ${produto.quantidade} unidades · ${moeda.format(produto.preco)}.`);
  document.querySelector(`[data-codigo="${codigo}"]`)?.scrollIntoView({ behavior: "smooth", block: "center" });
});

document.querySelector("#update-form").addEventListener("submit", (evento) => {
  evento.preventDefault();
  const dados = new FormData(evento.currentTarget);
  const codigo = Number(dados.get("codigo"));
  const quantidade = Number(dados.get("quantidade"));
  const resultado = modulo.ccall("demo_atualizar_quantidade", "number", ["number", "number"], [codigo, quantidade]);

  if (resultado === 0) {
    exibirMensagem("Quantidade atualizada com sucesso.");
    atualizarPainel();
  } else if (resultado === 1) {
    exibirMensagem(`Nenhum produto encontrado com o código ${codigo}.`, "error");
  } else {
    exibirMensagem("Não foi possível atualizar o estoque.", "error");
  }
});

async function iniciarDemo() {
  try {
    modulo = await criarModulo({
      locateFile: (arquivo) => new URL(`./wasm/${arquivo}`, import.meta.url).href,
    });
    botoesDeAcao.forEach((botao) => { botao.disabled = false; });
    atualizarPainel();
  } catch (erro) {
    exibirMensagem("Não foi possível carregar a demonstração. Atualize a página para tentar novamente.", "error");
    console.error(erro);
  }
}

botoesDeAcao.forEach((botao) => { botao.disabled = true; });
iniciarDemo();