#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no {
    int total;
    int* chaves;
    struct no** filhos;
    struct no* pai; 
} No;

typedef struct arvoreB {
  No* raiz;
  int ordem;
} ArvoreB;

ArvoreB* criaArvore(int);
No* criaNo(ArvoreB*);
void percorreArvore(No*);
int pesquisaBinaria(No*, int);
int localizaChave(ArvoreB*, int);
No* localizaNo(ArvoreB*, int);
void adicionaChaveNo(No*, No*, int);
int transbordo(ArvoreB*,No*);
No* divideNo(ArvoreB*, No*);
void adicionaChaveRecursivo(ArvoreB*, No*, No*, int);
void adicionaChave(ArvoreB*, int);

ArvoreB* criaArvore(int ordem) {
    ArvoreB* a = malloc(sizeof(ArvoreB));
    a->ordem = ordem;
    a->raiz = criaNo(a);

    return a;
}

int cont = 0;

No* criaNo(ArvoreB* ArvoreB) {
    int max = ArvoreB->ordem * 2;
    No* no = malloc(sizeof(No));

    no->pai = NULL;
        
    no->chaves = malloc(sizeof(int) * (max + 1));
    no->filhos = malloc(sizeof(No) * (max + 2));
    no->total = 0;

    for (int i = 0; i < max + 2; i++)
        no->filhos[i] = NULL;

    return no;
}

void percorreArvore(No* no) {
    if (no != NULL) {
        for (int i = 0; i < no->total; i++){
            percorreArvore(no->filhos[i]); //visita o filho a esquerda
            
            printf("%d ",no->chaves[i]);
        }

        percorreArvore(no->filhos[no->total]); //visita ultimo filho (direita)
    }
}

int pesquisaBinaria(No* no, int chave) {
    int inicio = 0, fim = no->total - 1, meio;		
    
    while (inicio <= fim) {	
        cont++;
        
        meio = (inicio + fim) / 2;
        
        if (no->chaves[meio] == chave) {	
		    return meio; //encontrou	
        } else if (no->chaves[meio] > chave) {
                fim	= meio - 1;	
        } else {
            inicio = meio + 1;
        }
    }
    return inicio; //não encontrou	
}

int localizaChave(ArvoreB* ArvoreB, int chave) {	
    No *no = ArvoreB->raiz;
    
    while (no != NULL) {
        int i = pesquisaBinaria(no, chave);

        if (i < no->total && no->chaves[i] == chave) {
            return 1; //encontrou
        } else {
            no = no->filhos[i];
        }
    }

    return 0; //não encontrou	
}

No* localizaNo(ArvoreB* ArvoreB, int chave) {	
    No *no = ArvoreB->raiz;
    
    while (no != NULL) {
        cont++;

        int i = pesquisaBinaria(no, chave);

        if (no->filhos[i] == NULL)
            return no; //encontrou nó
        else
            no = no->filhos[i];
    }

    return NULL; //não encontrou nenhum nó
}

void adicionaChaveNo(No* no, No* novo, int chave) {
    int i = pesquisaBinaria(no, chave);
    
    cont++;

    for (int j = no->total - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->filhos[j + 2] = no->filhos[j + 1];
    }
    
    no->chaves[i] = chave;
    no->filhos[i + 1] = novo;

    no->total++;
}

int transbordo(ArvoreB* ArvoreB, No* no) {
    cont++;
    
    return no->total > ArvoreB->ordem * 2;
}

No* divideNo(ArvoreB* ArvoreB, No* no) {
    int meio = no->total / 2;
    No* novo = criaNo(ArvoreB);
    novo->pai = no->pai;

    cont++;
    
    for (int i = meio + 1; i < no->total; i++) {
        novo->filhos[novo->total] = no->filhos[i];
        novo->chaves[novo->total] = no->chaves[i];
        if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
        
        novo->total++;
    }

    novo->filhos[novo->total] = no->filhos[no->total];
    if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;    
    no->total = meio;
    return novo;
}

void adicionaChaveRecursivo(ArvoreB* ArvoreB, No* no, No* novo, int chave) {
    cont++;
    
    adicionaChaveNo(no, novo, chave);
    
    if (transbordo(ArvoreB, no)) {
        int promovido = no->chaves[ArvoreB->ordem]; 
        No* novo = divideNo(ArvoreB, no);

        if (no->pai == NULL) {
            cont++;
            
            No* pai = criaNo(ArvoreB);            
            pai->filhos[0] = no;
            adicionaChaveNo(pai, novo, promovido);
            
            no->pai = pai;
            novo->pai = pai;
            ArvoreB->raiz = pai;
        } else
            adicionaChaveRecursivo(ArvoreB, no->pai, novo, promovido);
    }
}

void adicionaChave(ArvoreB* ArvoreB, int chave) {
    No* no = localizaNo(ArvoreB, chave);

    adicionaChaveRecursivo(ArvoreB, no, NULL, chave);
}


void worstcase1()
{
    ArvoreB *a = criaArvore(1);
    FILE *fp = fopen("PerformanceWorstCase1.txt", "w+");

    for (int i = 0; i < 1000; i += 1)
    {
        adicionaChave(a, i);
        fprintf(fp, "%d ", cont);
        cont = 0;
    }
    fclose(fp);
}
void worstcase5()
{
    ArvoreB *a = criaArvore(5);
    FILE *fp = fopen("PerformanceWorstCase5.txt", "w+");

    for (int i = 0; i < 1000; i += 1)
    {
        adicionaChave(a, i);
        fprintf(fp, "%d ", cont);
        cont = 0;
    }
    fclose(fp);
}
void worstcase10()
{
    ArvoreB *a = criaArvore(10);
    FILE *fp = fopen("PerformanceWorstCase10.txt", "w+");

    for (int i = 0; i < 1000; i += 1)
    {
        adicionaChave(a, i);
        fprintf(fp, "%d ", cont);
        cont = 0;
    }
    fclose(fp);
}
void avgcase1()
{
    FILE *fp = fopen("C:\\Users\\leodu\\Downloads\\B\\PerformanceAverageCase1.txt", "w+");
    srand(time(NULL));
    for (int j = 0; j < 10; j += 1)
    {
        ArvoreB *a = criaArvore(1);
        for (int i = 0; i < 10000; i += 1)
        {
            adicionaChave(a, rand());
            fprintf(fp, "%d ", cont);
            cont = 0;
        }
        fprintf(fp,"\n");
        free(a);
    }
    fclose(fp);
}
void avgcase5()
{
    FILE *fp = fopen("C:\\Users\\leodu\\Downloads\\B\\PerformanceAverageCase5.txt", "w+");
    srand(time(NULL));
    for (int j = 0; j < 10; j += 1)
    {
        ArvoreB *a = criaArvore(5);
        for (int i = 0; i < 10000; i += 1)
        {
            adicionaChave(a, rand());
            fprintf(fp, "%d ", cont);
            cont = 0;
        }
        fprintf(fp,"\n");
        free(a);
    }
    fclose(fp);
}
void avgcase10()
{
    FILE *fp = fopen("C:\\Users\\leodu\\Downloads\\B\\PerformanceAverageCase10.txt", "w+");
    srand(time(NULL));
    for (int j = 0; j < 10; j += 1)
    {
        ArvoreB *a = criaArvore(10);
        for (int i = 0; i < 10000; i += 1)
        {
            adicionaChave(a, rand());
            fprintf(fp, "%d ", cont);
            cont = 0;
        }
        fprintf(fp,"\n");
        free(a);
    }
    fclose(fp);
}

int main() {
    avgcase1();
    avgcase5();
    avgcase10();
}
