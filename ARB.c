#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int arvore_quant = 100; // incrementa de 100 em 100 ate chegar em 10000 nos
int cont_insere = 0; // contabiliza o custo de insercao da operacao
int cont_remove = 0; // contabiliza o custo de remocao da operacao

enum coracao {Vermelho, Preto};
typedef enum coracao Cor;

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
void balancear_remocao(Arvore *arvore, No *no);
void remover(Arvore *arvore, No *no);
void exibir_pre_order(No* no);
int altura(No* no);
void shuffle_num(int numeros[]);

No* search(No* raiz, int valor) {
    // Se o nó atual for nulo ou a chave for igual ao valor do nó, retorne o nó
    if (raiz == NULL || raiz->valor == valor)
        return raiz;

    // Se a chave for menor que o valor do nó, faça a busca na subárvore esquerda
    if (valor < raiz->valor)
        return search(raiz->esquerda, valor);

    // Caso contrário, faça a busca na subárvore direita
    return search(raiz->direita, valor);
}

int countNodes(Arvore* arvore, No* root) {
    if (root == NULL) {
        return 0;
    }

    // Conta o nó atual e recursivamente conta os nós nas subárvores esquerda e direita
    if (root == arvore->nulo)
        return 0 + countNodes(arvore, root->esquerda) + countNodes(arvore, root->direita);
    else
        return 1 + countNodes(arvore, root->esquerda) + countNodes(arvore, root->direita);
}

int main(int argc, char *argv[]) {
    FILE *arquivo1 = fopen("arb_insercao.txt", "w");
    if (arquivo1 == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    FILE *arquivo2 = fopen("arb_remocao.txt", "w");
    if (arquivo2 == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    int custo_medio_insercao = 0;
    int custo_medio_remocao = 0;

    int r = 0; // iterar por todas as 10 repeticoes para fazer a media
    int i = 0; // iterador padrao dos demais loops

    float num_repeticoes = 100.0; // numero de repeticoes para gerar as medias

    srand((unsigned)time(NULL));

    while (arvore_quant < 10001) {
        printf("Caso %d\n", arvore_quant);
        for (r = 0; r < 100; r++) {
            printf("Iteracao: %d\n", r);

            int random_numeros[arvore_quant]; // geracao de array com numeros "aleatorios" de 1 ate arvore_quant
            for (i = 0; i < arvore_quant; i++)
                random_numeros[i] = i + 1;
            shuffle_num(random_numeros);

            Arvore *arvore = inicializar_arvore();

            No *numeros[arvore_quant];  
            for (i = 0; i < arvore_quant; i++) {
                if (i == arvore_quant - 1)
                    cont_insere = 0;

                numeros[i] = criar_no(random_numeros[i]);
                inserir(arvore, numeros[i]);
            }

            custo_medio_insercao += cont_insere;

            printf("Custo de insercao: %d\n", cont_insere);

            remover(arvore, numeros[0]);

            custo_medio_remocao += cont_remove;

            printf("Custo de remocao: %d\n", cont_remove);

            printf("\n");

            cont_insere = 0;
            cont_remove = 0;
        }

        printf("Custo medio de insercao: %.2f\n", (custo_medio_insercao) / num_repeticoes);
        printf("Custo medio de remocao: %.2f\n", (custo_medio_remocao) / num_repeticoes);

        FILE *arquivo1 = fopen("arb_insercao.txt", "a");
        fprintf(arquivo1, "%d %.2f\n", arvore_quant, custo_medio_insercao / num_repeticoes);
        fclose(arquivo1);

        FILE *arquivo2 = fopen("arb_remocao.txt", "a");
        fprintf(arquivo2, "%d %.2f\n", arvore_quant, custo_medio_remocao / num_repeticoes);
        fclose(arquivo2);

        arvore_quant += 100;

        custo_medio_insercao = 0;
        custo_medio_remocao = 0;

        printf("\n");
    }

    return 0;
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

void rotacionar_esquerda(Arvore* arvore, No* no) {
    No* direita = no->direita;
    no->direita = direita->esquerda;

    if (direita->esquerda != arvore->nulo) {
        direita->esquerda->pai = no; // se tiver um filho a esquerda em direita, ele sera o pai do no
       
        cont_insere++;
    }

    direita->pai = no->pai; // ajusta no pai do no a direita

    if (no->pai == arvore->nulo) {
        arvore->raiz = direita;
       
        cont_insere++;
    }
    else if (no == no->pai->esquerda) {
        no->pai->esquerda = direita; // corrige relacao pai-filho do novo pai (esquerda)
       
        cont_insere++;
    }
    else {
        no->pai->direita = direita; // corrige relacao pai-filho do novo pai (direita)
       
        cont_insere++;
    }

    direita->esquerda = no; // corrige relacao pai-filho entre o no pivo e o no a direita
    no->pai = direita;

    cont_insere++;
}

void rotacionar_direita(Arvore* arvore, No* no) {
    No* esquerda = no->esquerda;
    no->esquerda = esquerda->direita;

    if (esquerda->direita != arvore->nulo) {
        esquerda->direita->pai = no; // se tiver filho a direita em esquerda, ele sera o pai do no
       
        cont_insere++;
    }

    esquerda->pai = no->pai; // ajusta no pai do no a esquerda

    if (no->pai == arvore->nulo) {
        arvore->raiz = esquerda;
       
        cont_insere++;
    }
    else if (no == no->pai->esquerda) {
        no->pai->esquerda = esquerda; // corrige relacao pai-filho do novo pai (esquerda)
       
        cont_insere++;
    }
    else {
        no->pai->direita = esquerda; // corrige relacao pai-filho do novo pai (direita)
       
        cont_insere++;
    }

    esquerda->direita = no; // corrige relacao pai-filho entre o no pivo e o no a esquerda
    no->pai = esquerda;

    cont_insere++;
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
        
                cont_insere++;
            }
            else {
                if (no == no->pai->direita) {
                    no = no->pai; // nivel anterior
                    rotacionar_esquerda(arvore, no); // caso 3
        
                    cont_insere++;
                }
                else {
                    no->pai->cor = Preto; // caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionar_direita(arvore, no->pai->pai);
        
                    cont_insere++;
                }
        
                cont_insere++;
            }
        
            cont_insere++;
        }
        else {
            No *tio = no->pai->pai->esquerda;
            if (tio != NULL && tio->cor == Vermelho) {
                tio->cor = Preto; // caso 2
                no->pai->cor = Preto;
                no->pai->pai->cor = Vermelho;
                no = no->pai->pai; // nivel anterior
        
                cont_insere++;
            }
            else {
                if (no == no->pai->esquerda) {
                    no = no->pai; // nivel anterior
                    rotacionar_direita(arvore, no); // caso 3
        
                    cont_insere++;
                }
                else {
                    no->pai->cor = Preto; // caso 4
                    no->pai->pai->cor = Vermelho;
                    rotacionar_esquerda(arvore, no->pai->pai);
        
                    cont_insere++;
                }
        
                cont_insere++;
            }
        
            cont_insere++;
        }
        
        cont_insere++;
    }

    if (arvore->raiz != NULL) {
        arvore->raiz->cor = Preto; // caso 1
        
        cont_insere++;
    }

    cont_insere++;
}

void inserir(Arvore* arvore, No *no) {
    No* pai = arvore->nulo;
    No* atual = arvore->raiz;

    while (atual != arvore->nulo) {
        pai = atual;

        if (no->valor < atual->valor) {
            atual = atual->esquerda;
            
            cont_insere++;
        }
        else {
            atual = atual->direita;
            
            cont_insere++;
        }

        cont_insere++;
    }

    no->pai = pai;

    if (pai == arvore->nulo) {
        arvore->raiz = no;

        cont_insere++;
    }
    else if (no->valor < pai->valor) {
        pai->esquerda = no;

        cont_insere++;
    }
    else {
        pai->direita = no;

        cont_insere++;
    }

    no->direita = arvore->nulo;
    no->esquerda = arvore->nulo;

    balancear(arvore, no);

    cont_insere++;
}

void transplant(Arvore *arvore, No *u, No *v) {
    if (u->pai == arvore->nulo) {
        arvore->raiz = v;

        cont_remove++;
    }
    else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;

        cont_remove++;
    }
    else {
        u->pai->direita = v;

        cont_remove++;
    }
    
    v->pai = u->pai;

    cont_remove++;
}

No* encontrar_minimo(Arvore *arvore, No* no) {
    while (no->esquerda != arvore->nulo) {
        no = no->esquerda;
        cont_remove++;
    }

    cont_remove++;

    return no;
}

void balancear_remocao(Arvore *arvore, No *no) {
    if (no == NULL)
        return;

    while (no != arvore->raiz && no->cor == Preto) {
        if (no == no->pai->esquerda) {
            No *irmao = no->pai->direita;

            if (irmao != NULL) {
                if (irmao->cor == Vermelho) {
                    irmao->cor = Preto;
                    no->pai->cor = Vermelho;
                    rotacionar_esquerda(arvore, no->pai);
                    irmao = no->pai->direita;

                    cont_remove++;
                }

                if (irmao != NULL && irmao->esquerda != NULL && irmao->direita != NULL &&
                    irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
                    irmao->cor = Vermelho;
                    no = no->pai;

                    cont_remove++;
                } 
                else {
                    if (irmao != NULL && irmao->direita != NULL && irmao->direita->cor == Preto) {
                        irmao->esquerda->cor = Preto;
                        irmao->cor = Vermelho;
                        rotacionar_direita(arvore, irmao);
                        irmao = no->pai->direita;

                        cont_remove++;
                    }

                    if (irmao != NULL) {
                        irmao->cor = no->pai->cor;
                        no->pai->cor = Preto;
                        if (irmao->direita != NULL) {
                            irmao->direita->cor = Preto;

                            cont_remove++;
                        }
                        rotacionar_esquerda(arvore, no->pai);

                        cont_remove++;
                    }

                    no = arvore->raiz;

                    cont_remove++;
                }

                cont_remove++;
            }

            cont_remove++;
        } 
        else {
            No *irmao = no->pai->esquerda;

            if (irmao->cor == Vermelho) {
                irmao->cor = Preto;
                no->pai->cor = Vermelho;
                rotacionar_direita(arvore, no->pai);
                irmao = no->pai->esquerda;

                cont_remove++;
            }

            if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
                irmao->cor = Vermelho;
                no = no->pai;

                cont_remove++;
            } 
            else {
                if (irmao->esquerda->cor == Preto) {
                    irmao->direita->cor = Preto;
                    irmao->cor = Vermelho;
                    rotacionar_esquerda(arvore, irmao);
                    irmao = no->pai->esquerda;

                    cont_remove++;
                }

                irmao->cor = no->pai->cor;
                no->pai->cor = Preto;
                irmao->esquerda->cor = Preto;
                rotacionar_direita(arvore, no->pai);
                no = arvore->raiz;

                cont_remove++;
            }

            cont_remove++;
        }

        cont_remove++;
    }

    no->cor = Preto;

    cont_remove++;
}

void remover(Arvore *arvore, No *no) {
    No *aux1 = no;
    No *aux2;
    Cor aux1_cor = aux1->cor;

    if (no->esquerda == arvore->nulo) {
        aux2 = no->direita;
        transplant(arvore, no, no->direita);

        cont_remove++;
    } 
    else if (no->direita == arvore->nulo) {
        aux2 = no->esquerda;
        transplant(arvore, no, no->esquerda);

        cont_remove++;
    } 
    else {
        aux1 = encontrar_minimo(arvore, no->direita);
        aux1_cor = aux1->cor;
        aux2 = aux1->direita;

        if (aux1->pai == no) {
            aux2->pai = aux1;

            cont_remove++;
        } 
        else {
            transplant(arvore, aux1, aux1->direita);
            aux1->direita = no->direita;
            aux1->direita->pai = aux1;

            cont_remove++;
        }

        transplant(arvore, no, aux1);

        aux1->esquerda = no->esquerda;
        aux1->esquerda->pai = aux1;
        aux1->cor = no->cor;

        cont_remove++;
    }

    if (aux1_cor == Preto) {
        balancear_remocao(arvore, aux2);
        cont_remove++;
    }

    cont_remove++;
}

void exibir_pre_order(No* no) {
    if (no != NULL) {
        printf("%d ", no->valor);
        exibir_pre_order(no->esquerda);
        exibir_pre_order(no->direita);
    }
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

void shuffle_num(int numeros[]) {
    for (int i = 0; i < arvore_quant; i++) {
        int troca = rand() % arvore_quant;
        int aux = numeros[i];
        numeros[i] = numeros[troca];
        numeros[troca] = aux;
    }
}
