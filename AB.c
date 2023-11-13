#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ORDEM 1 // Defina a ordem da árvore B aqui

typedef struct No {
    int total;
    int* chaves;
    struct No** filhos;
    struct No* pai;
} No;

typedef struct ArvoreB {
    No* raiz;
    int ordem;
} ArvoreB;

No* criaNo(ArvoreB* arvore) {
    int max = arvore->ordem * 2;
    No* no = (No*)malloc(sizeof(No));
    no->pai = NULL;
    no->chaves = (int*)malloc(sizeof(int) * (max + 1));
    no->filhos = (No**)malloc(sizeof(No*) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;
    
    return no;
}

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = (ArvoreB*)malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);
    return a;
}

void percorreArvore(No* no, int nivel) {
    if (no != NULL) {
        int i;
        for (i = 0; i < no->total; i++) {
            for (int j = 0; j < nivel; j++) {
                printf("    ");
            }
            if (no->pai) {
                printf("Pai: %d, Chave: %d\n", no->pai->chaves[0], no->chaves[i]);
            } else {
                printf("Raiz, Chave: %d\n", no->chaves[i]);
            }
            percorreArvore(no->filhos[i], nivel + 1);
        }
        for (int j = 0; j < nivel; j++) {
            printf("    ");
        }
        if (no->pai) {
            printf("Pai: %d\n", no->pai->chaves[0]);
        } else {
            printf("Raiz\n");
        }
        percorreArvore(no->filhos[i], nivel + 1);
    }
}

int localizaChave(ArvoreB* arvore, int chave) {
    No* no = arvore->raiz;

    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);
        if (i < no->total && no->chaves[i] == chave) {
            return 1; // encontrou
        } else {
            no = no->filhos[i];
        }
    }
    return 0; // não encontrou
}

int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (no->chaves[meio] == chave) {
            return meio; // encontrou
        } else if (no->chaves[meio] > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; // não encontrou
}

No* localizaNo(ArvoreB* arvore, int chave) {
    No* no = arvore->raiz;

    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);
        if (no->filhos[i] == NULL)
            return no; // encontrou nó
        else
            no = no->filhos[i];
    }
    return NULL; // não encontrou nenhum nó
}

void adicionaChaveNo(No* no, No* direita, int chave) {
    int i = pesquisaBinaria(no, chave);
    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }

    no->chaves[i] = chave;
    no->filhos[i + 1] = direita;
    if (direita != NULL)
        direita->pai = no;
    no->total++;
}

int transbordo(ArvoreB* arvore, No* no) {
    return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
    int meio = no->total / 2;
    No* novo = criaNo(arvore);
    novo->pai = no->pai;

    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL)
            novo->filhos[novo->total]->pai = novo;

        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL)
        novo->filhos[novo->total]->pai = novo;

    no->total = meio;
    return novo;
}

void adicionaChave(ArvoreB* arvore, int chave) {
    No* no = localizaNo(arvore, chave);
    adicionaChaveRecursivo(arvore, no, NULL, chave);
}
void removeChaveNo(No* no, int chave) {
    int i = pesquisaBinaria(no, chave);

    if (i < no->total && no->chaves[i] == chave) {
        for (int j = i; j < no->total - 1; j++) {
            no->chaves[j] = no->chaves[j + 1];
            no->filhos[j + 1] = no->filhos[j + 2];
        }

        no->total--;
    }
}
void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
    adicionaChaveNo(no, novo, chave);
    if (transbordo(arvore, no)) {
        int promovido = no->chaves[arvore->ordem];
        No* novo = divideNo(arvore, no);
        if (no->pai == NULL) {
            No* raiz = criaNo(arvore);
            raiz->filhos[0] = no;
            adicionaChaveNo(raiz, novo, promovido);
            no->pai = raiz;
            novo->pai = raiz;
            arvore->raiz = raiz;
        } else
            adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
}

void visita(int chave) {
    printf("%d ", chave);
}

int main() {
    ArvoreB* arvore = criaArvore(ORDEM);

    int numInsercoes, i, chave;

    printf("Quantas inserções você deseja fazer? ");
    scanf("%d", &numInsercoes);

    srand(time(NULL));
    for (i = 0; i < numInsercoes; i++) {
        chave = rand() % 10000;
        adicionaChave(arvore, chave);
    }

    printf("Árvore B:\n");
    percorreArvore(arvore->raiz, 0);

    printf("\n");

    // Remova uma chave
    int chaveRemover;
    printf("Qual chave você deseja remover? ");
    scanf("%d", &chaveRemover);

    No* noRemover = localizaNo(arvore, chaveRemover);
    removeChaveNo(noRemover, chaveRemover);

    printf("Árvore B após a remoção:\n");
    percorreArvore(arvore->raiz, visita);

    return 0;
}