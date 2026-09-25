export function createStatusMessage(element) {
  return {
    show(text, kind = "success") {
      element.textContent = text;
      element.dataset.kind = kind;
    },
  };
}