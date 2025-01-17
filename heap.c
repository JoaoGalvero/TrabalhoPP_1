#include "heap.h"
#include <string.h>

// Variáveis globais
static Heap heap;
static Variavel lista[TAM_HEAP];
static int prox_posicao = 0;
int ultimoIndice = 0;

// Função para inicializar uma variável
Variavel inicializar_variavel(const char* nome, int tamanho) {
    Variavel v;
    v.id[0] = nome;
    v.totalId = 1;
    v.tamanho = tamanho;
    v.posicao_heap = 0;

    for (int i = 1; i < TAM_ID; i++) {
        v.id[i] = NULL;
    }

    return v;
}

// Função para inicializar a lista de variáveis
void inicializar_lista_variaveis() {
    for (int i = 0; i < TAM_HEAP; i++) {
        Variavel x = inicializar_variavel(NULL, 0);
        lista[i] = x;
    }
}

// Função para inicializar o heap
void inicializar_heap() {
    inicializar_lista_variaveis();
    for (int i = 0; i < TAM_HEAP; i++) {
        heap.heap[i] = 0;
    }
    heap.listaLivres[0].inicio = 0;
    heap.listaLivres[0].tamanho = TAM_HEAP;
    heap.qtdLivres = 1;
    heap.totalLivres = TAM_HEAP;
    heap.tipoAlocacao = 'f';
}

// Função para exibir o estado do heap
void exibir_heap() {
    printf("\nEstado do heap:\n");
    for (int i = 0; i < TAM_HEAP; i++) {
        printf("%d", heap.heap[i]);
    }
    printf("\n");
}

// Função para exibir as áreas livres no heap
void exibir_areas_livres() {
    printf("\nÁreas livres:\n");
    for (int i = 0; i < heap.qtdLivres; i++) {
        printf("Início: %d, Tamanho: %d\n", heap.listaLivres[i].inicio, heap.listaLivres[i].tamanho);
    }
}

// Função para configurar o tipo de alocação do heap
void set_heap(const char* tipo) {
    if (strcmp(tipo, "first") == 0) {
        heap.tipoAlocacao = 'f';
    } else if (strcmp(tipo, "next") == 0) {
        heap.tipoAlocacao = 'n';
    } else if (strcmp(tipo, "best") == 0) {
        heap.tipoAlocacao = 'b';
    } else if (strcmp(tipo, "worst") == 0) {
        heap.tipoAlocacao = 'w';
    } else {
        printf("\nErro: Tipo de alocação inválido. Use 'first', 'next', 'best' ou 'worst'.\n");
    }

    printf("\nTipo de alocação configurado para: %s\n", tipo);
}

// First fit
void alocar_area_first(int tamanho, int* ultimoIndice) {
    for (int i = 0; i < heap.qtdLivres; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho) {
            int inicio = heap.listaLivres[i].inicio;

            for (int j = inicio; j < inicio + tamanho; j++) {
                heap.heap[j] = 1;
            }

            heap.listaLivres[i].inicio += tamanho;
            heap.listaLivres[i].tamanho -= tamanho;
            heap.totalLivres -= tamanho;

            if (heap.listaLivres[i].tamanho == 0) {
                for (int k = i; k < heap.qtdLivres - 1; k++) {
                    heap.listaLivres[k] = heap.listaLivres[k + 1];
                }
                heap.qtdLivres--;
            }

            lista[prox_posicao++].posicao_heap = inicio;

            printf("\nAlocação concluída (first fit). Início: %d, Tamanho: %d\n", inicio, tamanho);
            *ultimoIndice = i;
            return;
        }
    }
}

// Best fit
void alocar_area_best(int tamanho, int* ultimoIndice) {
    int melhorIndice = -1;
    int menorTamanho = TAM_HEAP + 1;

    for (int i = 0; i < heap.qtdLivres; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho && heap.listaLivres[i].tamanho < menorTamanho) {
            melhorIndice = i;
            menorTamanho = heap.listaLivres[i].tamanho;
        }
    }

    if (melhorIndice != -1) {
        int inicio = heap.listaLivres[melhorIndice].inicio;

        for (int j = inicio; j < inicio + tamanho; j++) {
            heap.heap[j] = 1;
        }

        heap.listaLivres[melhorIndice].inicio += tamanho;
        heap.listaLivres[melhorIndice].tamanho -= tamanho;

        if (heap.listaLivres[melhorIndice].tamanho == 0) {
            for (int k = melhorIndice; k < heap.qtdLivres - 1; k++) {
                heap.listaLivres[k] = heap.listaLivres[k + 1];
            }
            heap.qtdLivres--;
        }

        lista[prox_posicao].posicao_heap = inicio;
        prox_posicao++;
        *ultimoIndice = melhorIndice;

        printf("\nAlocação concluída (best fit). Início: %d, Tamanho: %d", inicio, tamanho);
    } else {
        printf("\nErro: Não há espaço suficiente no heap.\n");
    }
}

// Worst fit
void alocar_area_worst(int tamanho, int* ultimoIndice) {
    int piorIndice = -1;
    int maiorTamanho = -1;

    for (int i = 0; i < heap.qtdLivres; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho && heap.listaLivres[i].tamanho > maiorTamanho) {
            piorIndice = i;
            maiorTamanho = heap.listaLivres[i].tamanho;
        }
    }

    if (piorIndice != -1) {
        int inicio = heap.listaLivres[piorIndice].inicio;

        for (int j = inicio; j < inicio + tamanho; j++) {
            heap.heap[j] = 1;
        }

        heap.listaLivres[piorIndice].inicio += tamanho;
        heap.listaLivres[piorIndice].tamanho -= tamanho;

        if (heap.listaLivres[piorIndice].tamanho == 0) {
            for (int k = piorIndice; k < heap.qtdLivres - 1; k++) {
                heap.listaLivres[k] = heap.listaLivres[k + 1];
            }
            heap.qtdLivres--;
        }

        lista[prox_posicao].posicao_heap = inicio;
        prox_posicao++;
        printf("\nAlocação concluída (worst fit). Início: %d, Tamanho: %d", inicio, tamanho);
        *ultimoIndice = piorIndice;
    }
}

// Next fit (Quebrado) (GPT)
void alocar_area_next(int tamanho, int* ultimoIndice) {
    int i = *ultimoIndice;  // Começa pela última posição alocada

    // Busca até o final da lista de áreas livres
    while (i < heap.qtdLivres) {
        if (heap.listaLivres[i].tamanho >= tamanho) {
            int inicio = heap.listaLivres[i].inicio;

            // Realiza a alocação
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap.heap[j] = 1;
            }

            // Atualiza a área livre
            heap.listaLivres[i].inicio += tamanho;
            heap.listaLivres[i].tamanho -= tamanho;
            heap.totalLivres -= tamanho;

            if (heap.listaLivres[i].tamanho == 0) {
                // Remove a área livre se seu tamanho for 0
                for (int k = i; k < heap.qtdLivres - 1; k++) {
                    heap.listaLivres[k] = heap.listaLivres[k + 1];
                }
                heap.qtdLivres--;
            }

            lista[prox_posicao++].posicao_heap = inicio;
            *ultimoIndice = i;  // Atualiza o último índice

            printf("\nAlocação concluída (next fit). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
        i++;
    }

    // Se não encontrar no final, reinicia a busca do início
    i = 0;
    while (i < *ultimoIndice) {
        if (heap.listaLivres[i].tamanho >= tamanho) {
            int inicio = heap.listaLivres[i].inicio;

            // Realiza a alocação
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap.heap[j] = 1;
            }

            // Atualiza a área livre
            heap.listaLivres[i].inicio += tamanho;
            heap.listaLivres[i].tamanho -= tamanho;
            heap.totalLivres -= tamanho;

            if (heap.listaLivres[i].tamanho == 0) {
                // Remove a área livre se seu tamanho for 0
                for (int k = i; k < heap.qtdLivres - 1; k++) {
                    heap.listaLivres[k] = heap.listaLivres[k + 1];
                }
                heap.qtdLivres--;
            }

            lista[prox_posicao++].posicao_heap = inicio;
            *ultimoIndice = i;  // Atualiza o último índice

            printf("\nAlocação concluída (next fit). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
        i++;
    }

    printf("\nErro: Não há espaço suficiente no heap.\n");
}

// Função para liberar uma área no heap
void liberar_area(int inicio, int tamanho, int* ultimoIndice) {
    for (int i = inicio; i < inicio + tamanho; i++) {
        heap.heap[i] = 0;
    }

    heap.listaLivres[heap.qtdLivres].inicio = inicio;
    heap.listaLivres[heap.qtdLivres].tamanho = tamanho;
    heap.totalLivres += tamanho;
    heap.qtdLivres++;

    for (int i = heap.qtdLivres - 1; i > 0; i--) {
        if (heap.listaLivres[i].inicio < heap.listaLivres[i - 1].inicio) {
            AreaLivre temp = heap.listaLivres[i];
            heap.listaLivres[i] = heap.listaLivres[i - 1];
            heap.listaLivres[i - 1] = temp;
        } else {
            break;
        }
    }

    for (int i = 0; i < heap.qtdLivres - 1; i++) {
        if (heap.listaLivres[i].inicio + heap.listaLivres[i].tamanho == heap.listaLivres[i + 1].inicio) {
            heap.listaLivres[i].tamanho += heap.listaLivres[i + 1].tamanho;

            for (int j = i + 1; j < heap.qtdLivres - 1; j++) {
                heap.listaLivres[j] = heap.listaLivres[j + 1];
                (*ultimoIndice)++;
            }

            heap.qtdLivres--;
            i--;
        }
    }

    printf("\nÁrea liberada. Início: %d, Tamanho: %d\n", inicio, tamanho);
}

// Função para criar uma nova variável e alocar espaço no heap
void novo(const char* nome, int tamanho) {
    if (tamanho <= heap.totalLivres) {
        Variavel new = inicializar_variavel(nome, tamanho);
        lista[prox_posicao] = new;

        if (heap.tipoAlocacao == 'f') {
            alocar_area_first(tamanho, &ultimoIndice);
        } else if (heap.tipoAlocacao == 'n') {
            alocar_area_next(tamanho, &ultimoIndice);
        } else if (heap.tipoAlocacao == 'b') {
            alocar_area_best(tamanho, &ultimoIndice);
        } else {
            alocar_area_worst(tamanho, &ultimoIndice);
        }
    } else {
        printf("Erro: Não há espaço suficiente no heap.");
    }
}

// Função para ajustar a lista de variáveis
void ajusta_lista(int u) {
    int i, j;
    for (i = 0; i < prox_posicao; i++) {
        if (lista[i].id[u] == NULL) {
            for (j = i; j < prox_posicao - 1; j++) {
                lista[j] = lista[j + 1];
            }
            lista[prox_posicao - 1] = inicializar_variavel(NULL, 0);
            prox_posicao--;
            i--;
        }
    }
}

// Função para deletar uma variável e liberar espaço no heap
void del(const char* nome) {
    for (int i = 0; i < prox_posicao; i++) {
        for (int u = 0; u < TAM_ID; u++) {
            if (lista[i].id[u] != NULL && strcmp(lista[i].id[u], nome) == 0) {
                liberar_area(lista[i].posicao_heap, lista[i].tamanho, &ultimoIndice);

                lista[i].id[u] = NULL;
                Variavel vazia = inicializar_variavel(NULL, 0);
                lista[i] = vazia;

                ajusta_lista(u);
                return;
            }
        }
    }
    printf("Variável %s não existe", nome);
}

// Função para atribuir um ID a uma variável
void atribui(const char *id1, const char *id2) {
    for (int i = 0; i < prox_posicao; i++) {
        int u = 0;
        while (lista[i].id[u] != NULL) {
            if (strcmp(lista[i].id[u], id1) == 0) {
                int j = 0;
                while (lista[i].id[j] != NULL && j < TAM_HEAP) {
                    j++;
                }

                if (j < TAM_HEAP) {
                    lista[i].id[j] = id2;
                    lista[i].totalId++;
                    printf("\n%s = %s.\n", id2, id1);
                    return;
                } else {
                    printf("\nErro: Espaço insuficiente para mais IDs na variável %s.\n", id1);
                    return;
                }
            }
            u++;
        }
    }
    printf("Erro: A variável %s não foi encontrada.\n", id1);
}

// Função para exibir a lista de variáveis
void exibir_lista_variaveis() {
    for (int i = 0; i < prox_posicao; i++) {
        int u = 0;
        if (lista[i].id[u] != NULL) {
            printf("\n[%d] Nome: ", i);
            while (lista[i].id[u] != NULL) {
                printf("[%s] ", lista[i].id[u]);
                u++;
            }
            printf("\n[%d] Tamanho: %d\n", i, lista[i].tamanho);
            printf("[%d] Posição na Heap: %d\n", i, lista[i].posicao_heap);
        }
    }
}