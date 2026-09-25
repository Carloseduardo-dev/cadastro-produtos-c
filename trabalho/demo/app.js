import { createAddProductForm } from "./components/add-product-form.js";
import { createProductList } from "./components/product-list.js";
import { createSearchProductForm } from "./components/search-product-form.js";
import { createStatusMessage } from "./components/status-message.js";
import { createStockSummary } from "./components/stock-summary.js";
import { createUpdateStockForm } from "./components/update-stock-form.js";
import { createProductService } from "./services/product-service.js";

const statusMessage = createStatusMessage(document.querySelector("#status-message"));
const actionButtons = document.querySelectorAll(".controls button");
const viewButtons = document.querySelectorAll(".menubar [data-view]");
const workspace = document.querySelector(".workspace");
const catalogue = document.querySelector("#product-catalogue");
const viewForms = {
  add: document.querySelector("#add-form"),
  search: document.querySelector("#search-form"),
  stock: document.querySelector("#update-form"),
};

viewButtons.forEach((button) => {
  button.addEventListener("click", () => {
    const selectedView = button.dataset.view;
    workspace.dataset.view = selectedView;
    catalogue.hidden = selectedView !== "stock";
    Object.entries(viewForms).forEach(([view, form]) => {
      form.hidden = view !== selectedView;
    });
    viewButtons.forEach((viewButton) => {
      viewButton.setAttribute("aria-pressed", String(viewButton === button));
    });
  });
});

actionButtons.forEach((button) => { button.disabled = true; });

async function startApplication() {
  try {
    const productService = await createProductService();
    const stockSummary = createStockSummary();
    let refreshCatalogue;

    const refreshAfterChange = () => refreshCatalogue();
    const updateStockForm = createUpdateStockForm({
      form: document.querySelector("#update-form"),
      productService,
      statusMessage,
      onUpdated: refreshAfterChange,
    });
    const productList = createProductList({
      body: document.querySelector("#product-rows"),
      emptyState: document.querySelector("#empty-state"),
      onUpdate: (product) => updateStockForm.fill(product),
    });

    refreshCatalogue = () => {
      const products = productService.list();
      productList.render(products);
      stockSummary.render(products.length, productService.calculateTotal());
    };

    createAddProductForm({
      form: document.querySelector("#add-form"),
      productService,
      statusMessage,
      onAdded: refreshAfterChange,
    });
    createSearchProductForm({
      form: document.querySelector("#search-form"),
      productService,
      statusMessage,
    });

    refreshCatalogue();
    actionButtons.forEach((button) => { button.disabled = false; });
  } catch (error) {
    statusMessage.show("Não foi possível carregar a demonstração. Atualize a página para tentar novamente.", "error");
    console.error(error);
  }
}

startApplication();