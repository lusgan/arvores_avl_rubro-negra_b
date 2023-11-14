#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARVORE_QUANT 10000

enum coloracao {Vermelho, Preto};
typedef enum coloracao Cor;

typedef struct no {
    struct no* pai;
    struct no* esquerda;
    struct no* direita;
    Cor cor;
    int valor;
} No;

typedef struct arvore {
    No *raiz;
    No *nulo;
} Arvore;

void rotacionar_esquerda(Arvore* arvore, No* no);
void rotacionar_direita(Arvore* arvore, No* no);
void balancear(Arvore* arvore, No* no);
void rotacionar_esquerda(Arvore* arvore, No* no);
void rotacionar_direita(Arvore* arvore, No* no);
void inicializar_arvore(Arvore* arvore);
No* criar_no(Arvore* arvore, int valor);
void inserir(Arvore* arvore, int valor);
void exibir_pre_order(No* no);
void random_num(int numeros[], int tam);
int altura(No* no);
No* encontrar_no(No* no, int valor);
No* encontrar_minimo(No* no);

void remover(Arvore* arvore, int valor);
void remover_fixup(Arvore* arvore, No* no);
void trocar_nos(Arvore* arvore, No* no1, No* no2);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int numeros[ARVORE_QUANT];
    for (int i = 0; i < ARVORE_QUANT; i++)
        numeros[i] = i + 1;

    random_num(numeros, ARVORE_QUANT); // randomiza o array de valores

    Arvore arvore;
    inicializar_arvore(&arvore);

    /*
    for (int i = 0; i < ARVORE_QUANT + 1; i++)
        inserir(&arvore, numeros[i]);
    */

    inserir(&arvore, 10);
    inserir(&arvore, 5);
    inserir(&arvore, 30);
    inserir(&arvore, 2);
    inserir(&arvore, 9);
    inserir(&arvore, 25);
    inserir(&arvore, 40);
    inserir(&arvore, 38);

    printf("Arvore rubro-negra com %d elementos:\n", ARVORE_QUANT);
    exibir_pre_order(arvore.raiz);
    printf("\nAltura: %d\n\n", altura(arvore.raiz));

    /*
    random_num(numeros, ARVORE_QUANT); // randomiza o array de valores
    for (int i = 0; i < ARVORE_QUANT + 1; i++) {
       remover(&arvore, numeros[i]);
    }
    */

    remover(&arvore, 30);

    printf("Arvore rubro-negra apos remocao:\n");
    exibir_pre_order(arvore.raiz);
    printf("\nAltura: %d\n", altura(arvore.raiz));

    return 0;
}

No* encontrar_no(No* no, int valor) {
    if (no == NULL || no->valor == valor)
        return no;

    if (valor < no->valor)
        return encontrar_no(no->esquerda, valor);
    else
        return encontrar_no(no->direita, valor);
}

No* encontrar_minimo(No* no) {
    if (no == NULL)
        return NULL;

    while (no->esquerda != NULL)
        no = no->esquerda;

    return no;
}

int altura(No* no){
    if (no == NULL)
        return 0;
    
    int esquerda = 0,direita = 0;

    if (no->esquerda != NULL)
        esquerda = altura(no->esquerda) + 1;

    if (no->direita != NULL)
        direita = altura(no->direita) + 1;

    return esquerda > direita ? esquerda : direita;
}

void random_num(int numeros[], int tam) {
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }
}

void exibir_pre_order(No* no) {
    if (no != NULL) {
        printf("%d ", no->valor);
        exibir_pre_order(no->esquerda);
        exibir_pre_order(no->direita);
    }
}

void inicializar_arvore(Arvore* arvore) {
    arvore->nulo = malloc(sizeof(No));
    arvore->nulo->cor = Preto;
    arvore->raiz = arvore->nulo;
}

No* criar_no(Arvore* arvore, int valor) {
    No* novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->esquerda = arvore->nulo;
    novo->direita = arvore->nulo;
    novo->cor = Vermelho;

    return novo;
}

void inserir(Arvore* arvore, int valor) {
    No* no = criar_no(arvore, valor);
    No* pai = arvore->nulo;
    No* atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;

        if (valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    no->pai = pai;

    if (pai == arvore->nulo)
        arvore->raiz = no;
    else if (valor < pai->valor)
        pai->esquerda = no;
    else
        pai->direita = no;

    balancear(arvore, no);
}

void balancear(Arvore* arvore, No* no) {
    while (no != arvore->raiz && no->pai != NULL && no->pai->cor == Vermelho) {
        if (no->pai == no->pai->pai->esquerda) {
            No *tio = no->pai->pai->direita;
            if (tio != NULL && tio->cor == Vermelho) {
                tio->cor = Preto; // caso 2
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai; // nivel anterior
            }
            else {
                if (no == no->pai->direita) {
                    no = no->pai; // nivel anterior
                    rotacionar_esquerda(arvore, no); // caso 3
                }
                else {
                    no->pai->cor = Preto; // caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionar_direita(arvore, no->pai->pai);
                }
            }
        }
        else {
            No *tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->cor == Vermelho) {
                tio->cor = Preto; // caso 2
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai; // nivel anterior
            }
            else {
                if (no == no->pai->esquerda) {
                    no = no->pai; // nivel anterior
                    rotacionar_direita(arvore, no); // caso 3
                }
                else {
                    no->pai->cor = Preto; // caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionar_esquerda(arvore, no->pai->pai);
                }
            }
        }
    }

    if (arvore->raiz != NULL)
        arvore->raiz->cor = Preto; // caso 1
}

void rotacionar_esquerda(Arvore* arvore, No* no) {
    No* direita = no->direita;
    no->direita = direita->esquerda;

    if (direita->esquerda != arvore->nulo)
        direita->esquerda->pai = no; // se tiver um filho a esquerda em direita, ele sera o pai do no

    direita->pai = no->pai; // ajusta no pai do no a direita

    if (no->pai == arvore->nulo)
        arvore->raiz = direita;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = direita; // corrige relacao pai-filho do novo pai (esquerda)
    else
        no->pai->direita = direita; // corrige relacao pai-filho do novo pai (direita)

    direita->esquerda = no; // corrige relacao pai-filho entre o no pivo e o no a direita
    no->pai = direita;
}

void rotacionar_direita(Arvore* arvore, No* no) {
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;

    if (esquerda->direita != arvore->nulo)
        esquerda->direita->pai = no; // se tiver filho a direita em esquerda, ele sera o pai do no

    esquerda->pai = no->pai; // ajusta no pai do no a esquerda

    if (no->pai == arvore->nulo)
        arvore->raiz = esquerda;
    else if (no == no->pai->esquerda)
        no->pai->esquerda = esquerda; // corrige relacao pai-filho do novo pai (esquerda)
    else
        no->pai->direita = esquerda; // corrige relacao pai-filho do novo pai (direita)

    esquerda->direita = no; // corrige relacao pai-filho entre o no pivo e o no a esquerda
    no->pai = esquerda;
}
