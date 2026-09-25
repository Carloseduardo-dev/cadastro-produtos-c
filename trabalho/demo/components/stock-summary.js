const currency = new Intl.NumberFormat("pt-BR", { style: "currency", currency: "BRL" });

export function createStockSummary() {
  const itemCount = document.querySelector("#summary-count");
  const stockValue = document.querySelector("#summary-total");
  const catalogueCount = document.querySelector("#catalogue-count");

  return {
    render(quantity, total) {
      itemCount.textContent = String(quantity).padStart(2, "0");
      stockValue.textContent = currency.format(total);
      catalogueCount.textContent = `${quantity} ${quantity === 1 ? "produto" : "produtos"}`;
    },
  };
}