import createWasmModule from "../wasm/produtos.js";

const RESULT = Object.freeze({
  OK: 0,
  NOT_FOUND: 1,
  DUPLICATE: 2,
});

export async function createProductService() {
  const module = await createWasmModule({
    locateFile: (file) => new URL(`../wasm/${file}`, import.meta.url).href,
  });

  function readJson(functionName, argumentTypes = [], argumentsList = []) {
    const result = JSON.parse(module.ccall(functionName, "string", argumentTypes, argumentsList));
    if (result?.erro) throw new Error(result.erro);
    return result;
  }

  return {
    add(product) {
      const result = module.ccall(
        "demo_adicionar",
        "number",
        ["number", "string", "string", "number", "number"],
        [product.codigo, product.nome, product.categoria, product.quantidade, product.preco],
      );
      if (result === RESULT.OK) return "created";
      if (result === RESULT.DUPLICATE) return "duplicate";
      return "error";
    },
    list() {
      return readJson("demo_listar_json");
    },
    find(codigo) {
      return readJson("demo_buscar_json", ["number"], [codigo]);
    },
    calculateTotal() {
      const total = module.ccall("demo_calcular_total", "number", [], []);
      if (!Number.isFinite(total)) throw new Error("Não foi possível calcular o estoque.");
      return total;
    },
    updateQuantity(codigo, quantidade) {
      const result = module.ccall("demo_atualizar_quantidade", "number", ["number", "number"], [codigo, quantidade]);
      if (result === RESULT.OK) return "updated";
      if (result === RESULT.NOT_FOUND) return "not-found";
      return "error";
    },
  };
}