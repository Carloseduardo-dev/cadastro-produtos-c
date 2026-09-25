export function createAddProductForm({ form, productService, statusMessage, onAdded }) {
  form.addEventListener("submit", (event) => {
    event.preventDefault();
    const values = new FormData(form);
    const result = productService.add({
      codigo: Number(values.get("codigo")),
      nome: values.get("nome"),
      categoria: values.get("categoria"),
      quantidade: Number(values.get("quantidade")),
      preco: Number(values.get("preco")),
    });

    if (result === "created") {
      statusMessage.show("Produto cadastrado com sucesso.");
      form.reset();
      onAdded();
    } else if (result === "duplicate") {
      statusMessage.show("Já existe um produto com esse código.", "error");
    } else {
      statusMessage.show("Não foi possível cadastrar o produto.", "error");
    }
  });
}