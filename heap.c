#include "heap.h"
#include <string.h>

static Heap heap;
static Variavel lista[TAM_HEAP];
static int prox_posicao = 0;
int ultimoIndice = 0;

Variavel inicializar_variavel(const char* nome, int tamanho){
    Variavel v;
    v.id[0] = nome;
    v.totalId = 1;
    v.tamanho = tamanho;
    v.posicao_heap = 0;

    for(int i = 1; i < TAM_ID; i++){
        v.id[i] = NULL;
    }

    return v;
}

void inicializar_lista_variaveis(){
    for(int i = 0; i < TAM_HEAP; i++){
        Variavel x = inicializar_variavel(NULL, 0);
        lista[i] = x;
    }
}

void inicializar_heap() {
    inicializar_lista_variaveis();
    for (int i = 0; i < TAM_HEAP; i++) {
        heap.heap[i] = 0;
    }
    heap.listaLivres[0].inicio = 0;
    heap.listaLivres[0].tamanho = TAM_HEAP;
    heap.qtdLivres = 1;
    heap.totalLivres = TAM_HEAP;
    heap.tipoAlocacao = 'f'; // Default para 'f' (first)
}

// Função para exibir o estado do heap
void exibir_heap() {
    printf("\nEstado do heap:\n");
    for (int i = 0; i < TAM_HEAP; i++) {
        printf("%d", heap.heap[i]);
    }
    printf("\n");
}

// Função para exibir as áreas livres
void exibir_areas_livres() {
    printf("\nÁreas livres:\n");
    for (int i = 0; i < heap.qtdLivres; i++) {
        printf("Início: %d, Tamanho: %d\n", heap.listaLivres[i].inicio, heap.listaLivres[i].tamanho);
    }
}


void set_heap(const char* tipo) {
    if (strcmp(tipo, "first") == 0) {
        heap.tipoAlocacao = 'f';  // 'f' para First Fit
    } else if (strcmp(tipo, "next") == 0) {
        heap.tipoAlocacao = 'n';  // 'n' para Next Fit
    } else if (strcmp(tipo, "best") == 0) {
        heap.tipoAlocacao = 'b';  // 'b' para Best Fit
    } else if (strcmp(tipo, "worst") == 0) {
        heap.tipoAlocacao = 'w';  // 'w' para Worst Fit
    } else {
        printf("Erro: Tipo de alocação inválido. Use 'first', 'next', 'best' ou 'worst'.\n");
    }

    printf("Tipo de alocação configurado para: %s\n", tipo);
}

// Função para alocar área usando o método first fit
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

            printf("\nAlocação concluída. Início: %d, Tamanho: %d\n", inicio, tamanho);
            *ultimoIndice = i;
            printf("%i", *ultimoIndice);
            return;
        }
    }
}

// Função para alocar área usando o método best fit
void alocar_area_best(int tamanho) {
    int melhorIndice = -1;
    int menorTamanho = TAM_HEAP + 1;

    // Procura pela melhor área (best fit)
    for (int i = 0; i < heap.qtdLivres; i++) {
        // Verifica se a área livre é suficiente e se é a menor possível
        if (heap.listaLivres[i].tamanho >= tamanho && heap.listaLivres[i].tamanho < menorTamanho) {
            melhorIndice = i;
            menorTamanho = heap.listaLivres[i].tamanho;
        }
    }

    // Se encontrou uma área adequada
    if (melhorIndice != -1) {
        int inicio = heap.listaLivres[melhorIndice].inicio;
        
        // Aloca a área no heap
        for (int j = inicio; j < inicio + tamanho; j++) {
            heap.heap[j] = 1;  // Marca a memória como ocupada
        }

        // Atualiza a área livre
        heap.listaLivres[melhorIndice].inicio += tamanho;
        heap.listaLivres[melhorIndice].tamanho -= tamanho;

        // Se a área livre restante for 0, remove a entrada da lista de áreas livres
        if (heap.listaLivres[melhorIndice].tamanho == 0) {
            for (int k = melhorIndice; k < heap.qtdLivres - 1; k++) {
                heap.listaLivres[k] = heap.listaLivres[k + 1];
            }
            heap.qtdLivres--;  // Decrementa a quantidade de áreas livres
        }

        // Atualiza a lista de alocações
        lista[prox_posicao].posicao_heap = inicio;
        prox_posicao++;  // Avança para a próxima posição da lista
        printf("\nAlocação concluída (best fit). Início: %d, Tamanho: %d\n", inicio, tamanho);
    } else {
        printf("\nErro: Não há espaço suficiente no heap.\n");
    }
}


// Função para alocar área usando o método worst fit
void alocar_area_worst(int tamanho) {
    int piorIndice = -1;
    int maiorTamanho = -1;

    // Encontrar a maior área livre que tenha pelo menos o tamanho necessário
    for (int i = 0; i < heap.qtdLivres; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho && heap.listaLivres[i].tamanho > maiorTamanho) {
            piorIndice = i;
            maiorTamanho = heap.listaLivres[i].tamanho;
        }
    }

    if (piorIndice != -1) {
        int inicio = heap.listaLivres[piorIndice].inicio;

        // Alocar o espaço necessário no heap
        for (int j = inicio; j < inicio + tamanho; j++) {
            heap.heap[j] = 1;
        }

        // Atualizar a área livre
            heap.listaLivres[piorIndice].inicio += tamanho;
            heap.listaLivres[piorIndice].tamanho -= tamanho;
            heap.totalLivres -= tamanho;
            lista[prox_posicao++].posicao_heap = inicio;

        // Se a área livre ficou com tamanho 0, remove da lista de áreas livres
        if (heap.listaLivres[piorIndice].tamanho == 0) {
            for (int k = piorIndice; k < heap.qtdLivres - 1; k++) {
                heap.listaLivres[k] = heap.listaLivres[k + 1];
            }
            heap.qtdLivres--;  // Decrementa a quantidade de áreas livres
        }

        printf("\nAlocação concluída (worst). Início: %d, Tamanho: %d\n", inicio, tamanho);
    } else {
        printf("\nErro: Não há espaço suficiente no heap.\n");
    }
}

// Função para alocar área usando o método next fit
void alocar_area_next(int tamanho, int* ultimoIndice) {
    // Começa a busca a partir do índice armazenado em ultimoIndice
    for (int i = *ultimoIndice; i < heap.qtdLivres; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho) {
            int inicio = heap.listaLivres[i].inicio;

            // Aloca o espaço necessário no heap
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap.heap[j] = 1;
            }

            // Atualiza a área livre
            heap.listaLivres[i].inicio += tamanho;
            heap.listaLivres[i].tamanho -= tamanho;
            heap.totalLivres -= tamanho;

            // Se a área livre ficou com tamanho 0, remove da lista de áreas livres
            if (heap.listaLivres[i].tamanho == 0) {
                for (int k = i; k < heap.qtdLivres - 1; k++) {
                    heap.listaLivres[k] = heap.listaLivres[k + 1];
                }
                heap.qtdLivres--;
            }

            // Registra a alocação na lista de variáveis
            lista[prox_posicao++].posicao_heap = inicio;
            *ultimoIndice = i;  // Atualiza o último índice para o próximo uso

            printf("\nAlocação concluída (next). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
    }

    // Se não encontrou uma área suficiente, começa de novo do início da lista
    for (int i = 0; i < *ultimoIndice; i++) {
        if (heap.listaLivres[i].tamanho >= tamanho) {
            int inicio = heap.listaLivres[i].inicio;

            // Aloca o espaço necessário no heap
            for (int j = inicio; j < inicio + tamanho; j++) {
                heap.heap[j] = 1;
            }

            // Atualiza a área livre
            heap.listaLivres[i].inicio += tamanho;
            heap.listaLivres[i].tamanho -= tamanho;
            heap.totalLivres -= tamanho;

            // Se a área livre ficou com tamanho 0, remove da lista de áreas livres
            if (heap.listaLivres[i].tamanho == 0) {
                for (int k = i; k < heap.qtdLivres - 1; k++) {
                    heap.listaLivres[k] = heap.listaLivres[k + 1];
                }
                heap.qtdLivres--;
            }

            // Registra a alocação na lista de variáveis
            lista[prox_posicao++].posicao_heap = inicio;
            *ultimoIndice = i;  // Atualiza o último índice para o próximo uso

            printf("\nAlocação concluída (next). Início: %d, Tamanho: %d\n", inicio, tamanho);
            return;
        }
    }

    printf("\nErro: Não há espaço suficiente no heap.\n");
}


// Função para liberar área
void liberar_area(int inicio, int tamanho) {
    // Liberar os espaços no heap
    for (int i = inicio; i < inicio + tamanho; i++) {
        heap.heap[i] = 0;
    }

    // Adicionar a área liberada à lista de áreas livres
    heap.listaLivres[heap.qtdLivres].inicio = inicio;
    heap.listaLivres[heap.qtdLivres].tamanho = tamanho;

    // Incrementa a quantidade de áreas livres
    heap.qtdLivres++;

    // Ordenar a lista de áreas livres por início (ordem crescente)
    for (int i = heap.qtdLivres - 1; i > 0; i--) {
        if (heap.listaLivres[i].inicio < heap.listaLivres[i - 1].inicio) {
            // Troca as áreas livres de posição para manter a ordem
            AreaLivre temp = heap.listaLivres[i];
            heap.listaLivres[i] = heap.listaLivres[i - 1];
            heap.listaLivres[i - 1] = temp;
        } else {
            break; // Já está ordenado
        }
    }

    printf("\nÁrea liberada. Início: %d, Tamanho: %d\n", inicio, tamanho);
}


void novo(const char* nome, int tamanho) {
    if(tamanho <= heap.totalLivres){
    Variavel new = inicializar_variavel(nome, tamanho);
    lista[prox_posicao] = new;

    //Alocando na heap
    heap.tipoAlocacao == 'f'? alocar_area_first(tamanho, &ultimoIndice) :
    heap.tipoAlocacao == 'n'? alocar_area_next(tamanho, &ultimoIndice) :
    heap.tipoAlocacao == 'b'? alocar_area_best(tamanho) : alocar_area_worst(tamanho);

    //ajustando o ultimoIndice para o next fit
    
    }
    else{
        printf("Erro: Não há espaço suficiente no heap.");
    }
}

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

void del(const char* nome) {
    for (int i = 0; i < prox_posicao; i++) {
        for (int u = 0; u < TAM_ID; u++) {
            if (lista[i].id[u] != NULL && strcmp(lista[i].id[u], nome) == 0) {
                liberar_area(lista[i].posicao_heap, lista[i].tamanho);
                
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


void atribui(const char *id1, const char *id2){
    for(int i = 0; i < prox_posicao; i++){
        int u = 0;
        while(lista[i].id[u] != NULL){
            if(strcmp(lista[i].id[u], id1) == 0){
                int j = 0;
                while(lista[i].id[j] != NULL && j < TAM_HEAP){
                    j++;
                }
                
                if(j < TAM_HEAP) {
                    lista[i].id[j] = id2;
                    lista[i].totalId++;
                    printf("\n%s = %s.\n",id2, id1);
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


void exibir_lista_variaveis(){
    for(int i = 0; i < prox_posicao; i++){
        int u = 0;
        if(lista[i].id[u] != NULL){
            printf("\n[%d] Nome: ", i);
                while(lista[i].id[u] != NULL){
                    printf("[%s] ", lista[i].id[u]);
                    u++;
            }
            printf("\n[%d] Tamanho: %d\n", i, lista[i].tamanho);
            printf("[%d] Posição na Heap: %d\n", i, lista[i].posicao_heap);
        }
    }
}