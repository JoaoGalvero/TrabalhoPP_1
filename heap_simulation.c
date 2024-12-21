#include <stdio.h>

#define TAM_HEAP 100

typedef struct {
    int inicio;
    int tamanho;
} AreaLivre;

int heap[TAM_HEAP];
AreaLivre listaLivres[TAM_HEAP];
int qtdLivres = 0;

void inicializar_heap() {
    for (int i = 0; i < TAM_HEAP; i++) {
        heap[i] = 0;
    }
    listaLivres[0].inicio = 0;
    listaLivres[0].tamanho = TAM_HEAP;
    qtdLivres = 1;
}

void exibir_heap() {
    printf("Estado do heap:\n");
    for (int i = 0; i < TAM_HEAP; i++) {
        printf("%d", heap[i]);
    }
    printf("\n");
}

void exibir_areas_livres() {
    printf("\nÁreas livres:\n");
    for (int i = 0; i < qtdLivres; i++) {
        printf("Início: %d, Tamanho: %d\n", listaLivres[i].inicio, listaLivres[i].tamanho);
    }
}

void alocar_area_first(int tamanho) {
    for (int i = 0; i < qtdLivres; i++) {
        if (listaLivres[i].tamanho >= tamanho) {
            int inicio = listaLivres[i].inicio;
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap[j] = 1;
            }

            listaLivres[i].inicio += tamanho;
            listaLivres[i].tamanho -= tamanho;

            if (listaLivres[i].tamanho == 0) {
                for (int k = i; k < qtdLivres - 1; k++) {
                    listaLivres[k] = listaLivres[k + 1];
                }
                qtdLivres--;
            }

            printf("\nAlocação concluída (first). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
    }
    printf("\nErro: Não há espaço suficiente no heap.\n");
}

void alocar_area_best(int tamanho) {
    int melhorIndice = -1;
    int menorTamanho = TAM_HEAP + 1;

    for (int i = 0; i < qtdLivres; i++) {
        if (listaLivres[i].tamanho >= tamanho && listaLivres[i].tamanho < menorTamanho) {
            melhorIndice = i;
            menorTamanho = listaLivres[i].tamanho;
        }
    }

    if (melhorIndice != -1) {
        int inicio = listaLivres[melhorIndice].inicio;
        for (int j = inicio; j < inicio + tamanho; j++) {
            heap[j] = 1;
        }

        listaLivres[melhorIndice].inicio += tamanho;
        listaLivres[melhorIndice].tamanho -= tamanho;

        if (listaLivres[melhorIndice].tamanho == 0) {
            for (int k = melhorIndice; k < qtdLivres - 1; k++) {
                listaLivres[k] = listaLivres[k + 1];
            }
            qtdLivres--;
        }

        printf("\nAlocação concluída (best). Início: %d, Tamanho: %d\n", inicio, tamanho);
    } else {
        printf("\nErro: Não há espaço suficiente no heap.\n");
    }
}

void alocar_area_worst(int tamanho) {
    int piorIndice = -1;
    int maiorTamanho = -1;

    for (int i = 0; i < qtdLivres; i++) {
        if (listaLivres[i].tamanho >= tamanho && listaLivres[i].tamanho > maiorTamanho) {
            piorIndice = i;
            maiorTamanho = listaLivres[i].tamanho;
        }
    }

    if (piorIndice != -1) {
        int inicio = listaLivres[piorIndice].inicio;
        for (int j = inicio; j < inicio + tamanho; j++) {
            heap[j] = 1;
        }

        listaLivres[piorIndice].inicio += tamanho;
        listaLivres[piorIndice].tamanho -= tamanho;

        if (listaLivres[piorIndice].tamanho == 0) {
            for (int k = piorIndice; k < qtdLivres - 1; k++) {
                listaLivres[k] = listaLivres[k + 1];
            }
            qtdLivres--;
        }

        printf("\nAlocação concluída (worst). Início: %d, Tamanho: %d\n", inicio, tamanho);
    } else {
        printf("\nErro: Não há espaço suficiente no heap.\n");
    }
}

void alocar_area_next(int tamanho, int *ultimoIndice) {
    for (int i = *ultimoIndice; i < qtdLivres; i++) {
        if (listaLivres[i].tamanho >= tamanho) {
            int inicio = listaLivres[i].inicio;
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap[j] = 1;
            }

            listaLivres[i].inicio += tamanho;
            listaLivres[i].tamanho -= tamanho;

            if (listaLivres[i].tamanho == 0) {
                for (int k = i; k < qtdLivres - 1; k++) {
                    listaLivres[k] = listaLivres[k + 1];
                }
                qtdLivres--;
            }

            *ultimoIndice = i;
            printf("\nAlocação concluída (next). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
    }

    printf("\nErro: Não há espaço suficiente no heap.\n");
}

void liberar_area(int inicio, int tamanho) {
    for (int i = inicio; i < inicio + tamanho; i++) {
        heap[i] = 0;
    }

    listaLivres[qtdLivres].inicio = inicio;
    listaLivres[qtdLivres].tamanho = tamanho;
    qtdLivres++;

    printf("\nÁrea liberada. Início: %d, Tamanho: %d\n", inicio, tamanho);
}

int main() {
    inicializar_heap();
    exibir_heap();
    exibir_areas_livres();

    return 0;
}
