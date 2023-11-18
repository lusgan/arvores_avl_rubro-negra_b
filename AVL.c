#include<stdio.h> 
#include<stdlib.h> 
#include <time.h>

int quantidade_arvore = 1000;
int esforco = 0;
int inseridos = 0;

struct No 
{ 
	int valor; 
	struct No *esquerda; 
	struct No *direita; 
    struct No *pai;
	int altura; 
}; 
 
int altura(struct No *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->altura; 
} 

int maximo(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

struct No* novoNo(int valor) 
{ 
	struct No* no = (struct No*)malloc(sizeof(struct No)); 
	no->valor = valor;
	no->pai = NULL; 
	no->esquerda = NULL; 
	no->direita = NULL; 
	no->altura = 1; 
	return(no); 
} 

struct No *rotacaoDireita(struct No *a) 
{ 
    esforco++;
	struct No *x = a->esquerda; 
	struct No *b = x->direita; 

	x->direita = a; 
	a->esquerda = b; 

	a->altura = maximo(altura(a->esquerda),altura(a->direita)) + 1; 
	x->altura = maximo(altura(x->esquerda),altura(x->direita)) + 1; 
 
	return x; 
} 

struct No *rotacaoEsquerda(struct No *a) 
{ 
    esforco++;
	struct No *x = a->direita; 
	struct No *b = x->esquerda; 
 
	x->esquerda = a; 
	a->direita = b; 

	a->altura = maximo(altura(a->esquerda),altura(a->direita)) + 1; 
	x->altura = maximo(altura(x->esquerda),altura(x->direita)) + 1; 

	return x; 
} 

int fb(struct No *no)
{ 
	if (no == NULL) 
		return 0; 
	return altura(no->esquerda) - altura(no->direita); 
} 

struct No* inserir(struct No* no, int valor, struct No* pai){ //busca recursivamente o local adequado para inserir o no

    esforco++;
	if (no == NULL){ // encontrou o local
		struct No* novo = novoNo(valor);
		novo->pai = pai;
        inseridos++;
		return novo;
	}

	if (valor < no->valor) 
		no->esquerda = inserir(no->esquerda, valor, no);

	else if (valor > no->valor) 
		no->direita = inserir(no->direita, valor, no);

	else//valor passado ja existente na arvore
		return no; 


	no->altura = 1 + maximo(altura(no->esquerda),altura(no->direita)); 

	int fator_balanceamento = fb(no); 

    //RD
	if (fator_balanceamento > 1 && valor < no->esquerda->valor) 
		return rotacaoDireita(no); 

    //RE
	if (fator_balanceamento < -1 && valor > no->direita->valor) 
		return rotacaoEsquerda(no); 

    //RDD
	if (fator_balanceamento > 1 && valor > no->esquerda->valor) 
	{ 
		no->esquerda = rotacaoEsquerda(no->esquerda); 
		return rotacaoDireita(no); 
	} 

    //RDE
	if (fator_balanceamento < -1 && valor < no->direita->valor) 
	{ 
		no->direita = rotacaoDireita(no->direita); 
		return rotacaoEsquerda(no); 
	} 

	return no; 
} 

void preOrdem(struct No *raiz) 
{ 
	if(raiz != NULL) 
	{ 	
		printf("valor %d, altura = %d\n", raiz->valor, raiz->altura); 
		preOrdem(raiz->esquerda); 
		preOrdem(raiz->direita); 
	} 
} 

int contarElementosNo(struct No* no) {
    if (no == NULL) {
        return 0;
    }
    return 1 + contarElementosNo(no->esquerda) + contarElementosNo(no->direita);
}

struct No *noValorMinimo(struct No* no)
{
    struct No* atual = no;
 
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
 
    return atual;
}
 
struct No* deletarNo(struct No* raiz, int valor)
{
    esforco++;
    if (raiz == NULL)
        return raiz;
 
    if ( valor < raiz->valor )
        raiz->esquerda = deletarNo(raiz->esquerda, valor);
 
    else if( valor > raiz->valor )
        raiz->direita = deletarNo(raiz->direita, valor);
 
    else
    {
        if((raiz->esquerda == NULL) || (raiz->direita == NULL)){
            struct No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
 
            if (temp == NULL){
                temp = raiz;
                raiz = NULL;
            }
            else
             *raiz = *temp; 
     
            free(temp);
        }

        else{
            struct No* temp = noValorMinimo(raiz->direita);
            raiz->valor = temp->valor;
            raiz->direita = deletarNo(raiz->direita, temp->valor);
        }
    }
 
    if (raiz == NULL)
      return raiz;
 
    raiz->altura = 1 + maximo(altura(raiz->esquerda),altura(raiz->direita));

    int fator_balanceamento = fb(raiz); 
    if (fator_balanceamento > 1 && fb(raiz->esquerda) >= 0)
        return rotacaoDireita(raiz);
 
    if (fator_balanceamento > 1 && fb(raiz->esquerda) < 0){
        raiz->esquerda =  rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }
 
    if (fator_balanceamento < -1 && fb(raiz->direita) <= 0)
        return rotacaoEsquerda(raiz);
 
    if (fator_balanceamento < -1 && fb(raiz->direita) > 0){
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }
 
    return raiz;
}

void deletarArvore(struct No *raiz) {
    if (raiz == NULL) {
        return;
    }

    deletarArvore(raiz->esquerda);
    deletarArvore(raiz->direita);

    free(raiz);
}


int main() { 

    int qtd_elementos[100]; // vetor que guarda a qtd de elementos analisados a cada iteracao, na iteracao 0 sao 100, na iteracao1 sao 200 etc.
    float esforcos_medios[100];// esforco medio na iteracao 0,1,2 etc. esse vetor trabalha em parceria com o de cima
    quantidade_arvore = 100; // qtd inicial de elementos inseridos ou removidos na arvore, esse valor posteriormente sera incrementado de 100 em 100.

    int caso = 2; //caso 1 -> insercao
                  //caso 2-> remocao

    srand( (unsigned)time(NULL) );

    if(caso == 1){

        while(quantidade_arvore<10001){

            float esforco_medio = 0;
        
            for(int i = 0; i<30;i++){
                //insere valores aleatórios
                struct No *raiz = NULL; 
                for(int i = 0; inseridos<quantidade_arvore ;i++){
                    int valor = rand()%quantidade_arvore;
                    raiz = inserir(raiz,valor,NULL);
                }

                esforco_medio+= esforco;
                deletarArvore(raiz);
                esforco = 0;
                inseridos = 0;

            }

            //printf("%d elementos, Esforco medio = %.2f\n",quantidade_arvore,esforco_medio/50);
            qtd_elementos[quantidade_arvore/100 - 1] = quantidade_arvore;  
            esforcos_medios[quantidade_arvore/100 - 1] = esforco_medio/50;
            quantidade_arvore+=100;
        }

        //sobreescreve o arquivo avl_inserir.txt todos os valores de qtd de elementos, e esforco medio.
        FILE *arquivo = fopen("avl_inserir.txt", "w");
        for(int i = 0; i<quantidade_arvore/100 - 1; i++){
            printf("%d %.2f\n",qtd_elementos[i],esforcos_medios[i]);
            fprintf(arquivo,"%d %.2f\n",qtd_elementos[i],esforcos_medios[i]);
        }
        fclose(arquivo);

    }

    else if(caso == 2){

    }
    


    //deleta valores
	/*for(int i = 500; i<1000;i++){
		raiz = deletarNo(raiz,i);
	}*/

	return 0;
}