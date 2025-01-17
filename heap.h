#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>

#define TAM_HEAP 20
#define TAM_ID 10

typedef struct {
    int inicio;
    int tamanho;
} AreaLivre;

typedef struct {
    int heap[TAM_HEAP];
    AreaLivre listaLivres[TAM_HEAP];
    int qtdLivres;
    int totalLivres;
    char tipoAlocacao;
} Heap;

typedef struct {
    const char* id[TAM_ID];
    int totalId;
    int tamanho;
    int posicao_heap;
} Variavel;

// Funções
void inicializar_heap();
void exibir_heap();
void exibir_areas_livres();
void set_heap(const char* tipo);
void novo(const char* id, int tamanho);
void del(const char* id);
void liberar_area(int inicio, int tamanho);
void exibir_lista_variaveis();
void atribui(const char* id1, const char* id2);

#endif
