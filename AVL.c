#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARVORE_QUANT 10000

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    float valor;
    int altura;
} No;

typedef struct arvore {
    struct no* raiz;
} Arvore;

Arvore* cria() {
    Arvore* arvore = malloc(sizeof(Arvore));
    arvore->raiz = NULL;
    return arvore;
}

int vazia(Arvore* arvore) {
    return (arvore->raiz == NULL);
}

int altura(No* no) {
    if (no == NULL)
        return 0;
    return no->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void atualizaAltura(No* no) {
    if (no != NULL) {
        no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
    }
}

No* criaNo(float valor) {
    No* no = malloc(sizeof(No));
    no->pai = NULL;
    no->esquerda = NULL;
    no->direita = NULL;
    no->valor = valor;
    no->altura = 1;
    return no;
}

No* rotacaoDireita(No* y, Arvore* arvore) {
    No* x = y->esquerda;
    No* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

    if (y == arvore->raiz) {
        arvore->raiz = x;
    }

    // Atualiza o ponteiro pai de y e x
    x->pai = y->pai;
    y->pai = x;

    return x;
}

No* rotacaoEsquerda(No* x, Arvore* arvore) {
    No* y = x->direita;
    No* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

    if (x == arvore->raiz) {
        arvore->raiz = y;
    }

    // Atualiza o ponteiro pai de x e y
    y->pai = x->pai;
    x->pai = y;

    return y;
}


int fb(No* no) {
    if (no == NULL)
        return 0;
    return altura(no->esquerda) - altura(no->direita);
}

int esforco = 0;

No* inserir(No* no, float valor, Arvore* arvore, No* pai) {
    //encontrou o no onde deve ser inserido o valor
    if (no == NULL) {
        No* novoNo = criaNo(valor);
        novoNo->pai = pai;
        return novoNo;
    }

    //busca recursivamente o no do codigo acima
    if (valor < no->valor) {
        no->esquerda = inserir(no->esquerda, valor, arvore, no);
        esforco++;
    } else if (valor > no->valor) {
        no->direita = inserir(no->direita, valor, arvore, no);
        esforco++;
    } else {
        // Valor j� existe
        return no;
    }

    atualizaAltura(no);

    int balance = fb(no);

    // Casos de desbalanceamento

    // Caso LL
    if (balance > 1 && valor < no->esquerda->valor) {
        no = rotacaoDireita(no, arvore);
        esforco++;
    }

    // Caso RR
    if (balance < -1 && valor > no->direita->valor) {
        no = rotacaoEsquerda(no, arvore);
        esforco++;
    }

    // Caso LR
    if (balance > 1 && valor > no->esquerda->valor) {
        no->esquerda = rotacaoEsquerda(no->esquerda, arvore);
        no = rotacaoDireita(no, arvore);
    }

    // Caso RL
    if (balance < -1 && valor < no->direita->valor) {
        no->direita = rotacaoDireita(no->direita, arvore);
        no = rotacaoEsquerda(no, arvore);
    }

    // Retornar o n� ajustado
    return no;
}


void adicionar(Arvore* arvore, float valor) {
    arvore->raiz = inserir(arvore->raiz, valor, arvore, NULL);
}



void printer(No* v) {
    if (v == NULL) {
        return;
    } else if (v->pai == NULL) {
        printf("%.1f, altura = %d, pai = sem pai\n", v->valor, altura(v));
    } else {
        printf("%.1f, altura = %d, pai = %.1f\n", v->valor, altura(v), v->pai->valor);
    }

    printer(v->esquerda);
    printer(v->direita);
}

void random_num(int numeros[], int tam) {
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }
}

int main() {
    /*
    srand(time(NULL));

    int numeros[ARVORE_QUANT];
    for (int i = 0; i < ARVORE_QUANT; i++)
        numeros[i] = i + 1;

    random_num(numeros, ARVORE_QUANT); // randomiza o array de valores

    Arvore* arvore = cria();

    for (int i = 0; i < ARVORE_QUANT + 1; i++)
        adicionar(arvore, numeros[i]);
    */

    Arvore* arvore = cria();

    for (int i = 1; i <= 10000; i++) {
        adicionar(arvore, i);
    }
    

    printf("�rvore AVL em ordem pre-order:\n");
    printer(arvore->raiz);
    printf("\n\n");
    printf("altura = %d\n",altura(arvore->raiz));
    printf("esforco = %d",esforco);

    return 0;
}
