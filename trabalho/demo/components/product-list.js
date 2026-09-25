const currency = new Intl.NumberFormat("pt-BR", { style: "currency", currency: "BRL" });

function createCell(label, value, className = "") {
  const cell = document.createElement("td");
  cell.dataset.label = label;
  if (className) cell.className = className;
  cell.textContent = value;
  return cell;
}

function createCategoryCell(category) {
  const cell = document.createElement("td");
  cell.dataset.label = "Categoria";
  const tag = document.createElement("span");
  tag.className = "category-tag";
  tag.textContent = category;
  cell.append(tag);
  return cell;
}

function createUpdateCell(product, onUpdate) {
  const cell = document.createElement("td");
  cell.dataset.label = "Ação";
  const button = document.createElement("button");
  button.className = "row-action";
  button.type = "button";
  button.textContent = "Alterar estoque";
  button.addEventListener("click", () => onUpdate(product));
  cell.append(button);
  return cell;
}

export function createProductList({ body, emptyState, onUpdate }) {
  return {
    render(products) {
      body.replaceChildren();
      for (const product of products) {
        const row = document.createElement("tr");
        row.dataset.codigo = String(product.codigo);
        row.append(
          createCell("Código", String(product.codigo), "product-code"),
          createCell("Produto", product.nome, "product-name"),
          createCategoryCell(product.categoria),
          createCell("Quantidade", String(product.quantidade), "numeric-cell"),
          createCell("Preço unitário", currency.format(product.preco), "numeric-cell"),
          createUpdateCell(product, onUpdate),
        );
        body.append(row);
      }
      emptyState.hidden = products.length > 0;
    },
  };
}