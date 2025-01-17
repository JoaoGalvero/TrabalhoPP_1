#include "heap.h"

int main() {
    inicializar_heap();
    set_heap("best");

    novo("a", 5);

    exibir_heap();
    exibir_areas_livres();

    atribui("a", "c");
    novo("b", 9);
    exibir_heap();
    del("a");
    exibir_heap();
    novo("q", 2);

    exibir_heap();
    exibir_areas_livres();

}
