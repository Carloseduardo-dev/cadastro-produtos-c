export function createUpdateStockForm({ form, productService, statusMessage, onUpdated }) {
  form.addEventListener("submit", (event) => {
    event.preventDefault();
    const values = new FormData(form);
    const codigo = Number(values.get("codigo"));
    const quantidade = Number(values.get("quantidade"));
    const result = productService.updateQuantity(codigo, quantidade);

    if (result === "updated") {
      statusMessage.show("Quantidade atualizada com sucesso.");
      onUpdated();
    } else if (result === "not-found") {
      statusMessage.show(`Nenhum produto encontrado com o código ${codigo}.`, "error");
    } else {
      statusMessage.show("Não foi possível atualizar o estoque.", "error");
    }
  });

  return {
    fill(product) {
      form.elements.codigo.value = product.codigo;
      form.elements.quantidade.focus();
      form.scrollIntoView({ behavior: "smooth", block: "center" });
    },
  };
}