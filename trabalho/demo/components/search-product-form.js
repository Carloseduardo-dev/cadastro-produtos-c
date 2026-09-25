const currency = new Intl.NumberFormat("pt-BR", { style: "currency", currency: "BRL" });

export function createSearchProductForm({ form, productService, statusMessage }) {
  form.addEventListener("submit", (event) => {
    event.preventDefault();
    const codigo = Number(new FormData(form).get("codigo"));
    const product = productService.find(codigo);

    if (product === null) {
      statusMessage.show(`Nenhum produto encontrado com o código ${codigo}.`, "error");
      return;
    }
    statusMessage.show(`Encontrado: ${product.nome} · ${product.categoria} · ${product.quantidade} unidades · ${currency.format(product.preco)}.`);
    document.querySelector(`[data-codigo="${codigo}"]`)?.scrollIntoView({ behavior: "smooth", block: "center" });
  });
}