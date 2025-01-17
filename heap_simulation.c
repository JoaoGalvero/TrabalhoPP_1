#include "heap.h"

//Main teste
int main() {
inicializar_heap();
    // Teste de alocação First Fit
    set_heap("first");
    novo("var1", 10);
    novo("var2", 20);
    exibir_heap();
    exibir_areas_livres();

    // Liberar uma área e testar novamente
    del("var1");
    exibir_heap();
    exibir_areas_livres();

    // Teste de alocação Best Fit
    set_heap("best");
    novo("var3", 5);
    novo("var4", 15);
    novo("var9", 1);
    exibir_heap();
    exibir_areas_livres();

    // Liberar uma área e testar novamente
    del("var4");
    exibir_heap();
    exibir_areas_livres();

    // Teste de alocação Worst Fit
    set_heap("next");
    novo("var5", 2);
    exibir_heap();
    exibir_areas_livres();

    // Liberar uma área e testar novamente
    del("var3");
    novo("u", 2);
    exibir_heap();
    exibir_areas_livres();
}