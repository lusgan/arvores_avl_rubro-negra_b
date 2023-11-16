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

Arvore* inicializar_arvore();
No* criar_no(int valor);
void rotacionar_esquerda(Arvore* arvore, No* no);
void rotacionar_direita(Arvore* arvore, No* no);
void balancear(Arvore* arvore, No* no);
void inserir(Arvore* arvore, No* no);
void transplant(Arvore *arvore, No *u, No *v);
No* encontrar_minimo(Arvore* arvore, No* no);
No* encontrar_no(No* no, int valor);
void balancear_remocao(Arvore *arvore, No *no);
void remover(Arvore *arvore, No *no);
void exibir_pre_order(No* no);
int altura(No* no);
void random_num(int numeros[], int tam);

int main(int argc, char *argv[]) {
    srand(time(NULL));

    int random_numeros[ARVORE_QUANT];
    for (int i = 0; i < ARVORE_QUANT; i++)
        random_numeros[i] = i + 1;

    random_num(random_numeros, ARVORE_QUANT); // randomiza o array de valores

    Arvore *arvore = inicializar_arvore();

    No *numeros[ARVORE_QUANT];  
    for (int i = 0; i < ARVORE_QUANT; i++) {
        numeros[i] = criar_no(random_numeros[i]);
        inserir(arvore, numeros[i]);
    }

    printf("\nArvore rubro-negra com %d elementos:\n", ARVORE_QUANT);
    exibir_pre_order(arvore->raiz);
    printf("\nAltura: %d\n\n", altura(arvore->raiz));

    for (int i = 0; i < 100; i++) {
       remover(arvore, numeros[i]);
    }

    printf("\nArvore rubro-negra apos remocao:\n");
    exibir_pre_order(arvore->raiz);
    printf("\nAltura: %d\n\n", altura(arvore->raiz));
    
    return 0;
}

void transplant(Arvore *arvore, No *u, No *v) {
    if (u->pai == arvore->nulo)
        arvore->raiz = v;
    else if (u == u->pai->esquerda)
        u->pai->esquerda = v;
    else
        u->pai->direita = v;
    
    v->pai = u->pai;
}

No* encontrar_no(No* no, int valor) {
    if (no == NULL || no->valor == valor)
        return no;

    if (valor < no->valor)
        return encontrar_no(no->esquerda, valor);
    else
        return encontrar_no(no->direita, valor);
}

No* encontrar_minimo(Arvore *arvore, No* no) {
    while (no->esquerda != arvore->nulo)
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

Arvore* inicializar_arvore() {
    Arvore *arvore = (Arvore*) malloc(sizeof(Arvore));

    No *nulo = malloc(sizeof(No));
    nulo->pai = NULL;
    nulo->esquerda = NULL;
    nulo->direita = NULL;
    nulo->cor = Preto;
    nulo->valor = 0;

    arvore->nulo = nulo;
    arvore->raiz = arvore->nulo;

    return arvore;
}

No* criar_no(int valor) {
    No* novo = malloc(sizeof(No));

    novo->valor = valor;
    novo->pai = NULL;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->cor = Vermelho;

    return novo;
}

void inserir(Arvore* arvore, No *no) {
    No* pai = arvore->nulo;
    No* atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;

        if (no->valor < atual->valor)
            atual = atual->esquerda;
        else
            atual = atual->direita;
    }

    no->pai = pai;

    if (pai == arvore->nulo)
        arvore->raiz = no;
    else if (no->valor < pai->valor)
        pai->esquerda = no;
    else
        pai->direita = no;

    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;

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

void balancear_remocao(Arvore *arvore, No *no) {
    while (no != arvore->raiz && no->cor == Preto) {
        if (no == no->pai->esquerda) {
            No *aux = no->pai->direita;

            if (aux->cor == Vermelho) {
                aux->cor = Preto;
                no->pai->cor = Vermelho;

                rotacionar_esquerda(arvore, no->pai);

                aux = no->pai->direita;
            }
            if (aux->esquerda->cor == Preto && aux->direita->cor == Preto) {
                aux->cor = Vermelho;
                no = no->pai;
            }
            else {
                if(aux->direita->cor == Preto) {
                    aux->esquerda->cor = Preto;
                    aux->cor = Vermelho;

                    rotacionar_direita(arvore, aux);

                    aux = no->pai->direita;
                }

                aux->cor = no->pai->cor;
                no->pai->cor = Preto;
                aux->direita->cor = Preto;

                rotacionar_esquerda(arvore, no->pai);

                no = arvore->raiz;
            }
        }
        else {
            No *aux = no->pai->esquerda;

            if (aux->cor == Vermelho) {
                aux->cor = Preto;
                no->pai->cor = Vermelho;

                rotacionar_direita(arvore, no->pai);

                aux = no->pai->esquerda;
            }
            if (aux->direita->cor == Preto && aux->esquerda->cor == Preto) {
                aux->cor = Vermelho;
                no = no->pai;
            }
            else {
                if (aux->esquerda->cor == Preto) {
                    aux->direita->cor = Preto;
                    aux->cor = Vermelho;

                    rotacionar_esquerda(arvore, aux);

                    aux = no->pai->esquerda;
                }
        
                aux->cor = no->pai->cor;
                no->pai->cor = Preto;
                aux->esquerda->cor = Preto;

                rotacionar_direita(arvore, no->pai);

                no = arvore->raiz;
            }
        }
    }

    no->cor = Preto;
}

void remover(Arvore *arvore, No *no) {
    No *aux1 = no;
    No *aux2;
    Cor aux1_cor = aux1->cor;

    if (no->esquerda == arvore->nulo) {
        aux2 = no->direita;
        transplant(arvore, no, no->direita);
    }
    else if (no->direita == arvore->nulo) {
        aux2 = no->esquerda;
        transplant(arvore, no, no->esquerda);
    }
    else {
        aux1 = encontrar_minimo(arvore, no->direita);
        aux1_cor = aux1->cor;
        aux2 = aux1->direita;

        if (aux1->pai == no) {
            aux2->pai = no;
        }
        else {
            transplant(arvore, aux1, aux1->direita);
            aux1->direita = no->direita;
            aux1->direita->pai = aux1;
        }

        transplant(arvore, no, aux1);

        aux1->esquerda = no->esquerda;
        aux1->esquerda->pai = aux1;
        aux1->cor = no->cor;
    }

    if (aux1_cor == Preto)
        balancear_remocao(arvore, aux2);
}
