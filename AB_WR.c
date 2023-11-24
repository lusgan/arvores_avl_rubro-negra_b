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
    void removerChaveRecursivo(ArvoreB* ArvoreB, No* no, int chave);
    void removeChaveNo(No* no, int chave);
    void rearranjaNo(ArvoreB* ArvoreB ,No* no, int indice);
    void mergeNos(ArvoreB* ArvoreB ,No* no, int indice);

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
            cont++;
            for (int i = 0; i < no->total; i++){
                percorreArvore(no->filhos[i]); //visita o filho a esquerda
                
                printf("%d ",no->chaves[i]);
                cont++;
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
                cont++;
                return meio; //encontrou	
            } else if (no->chaves[meio] > chave) {
                cont++;
                    fim	= meio - 1;	
            } else {
                cont++;
                inicio = meio + 1;
            }
        }
        return inicio; //não encontrou	
    }

    int localizaChave(ArvoreB* ArvoreB, int chave) {	
        No *no = ArvoreB->raiz;
        
        while (no != NULL) {
            cont++;
            int i = pesquisaBinaria(no, chave);

            if (i < no->total && no->chaves[i] == chave) {
                cont++;
                return 1; //encontrou
            } else {
                cont++;
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
{                cont++;
                return no;} //encontrou nó
            else{
                cont++;
                no = no->filhos[i];
            }
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
            cont++;
            novo->filhos[novo->total] = no->filhos[i];
            novo->chaves[novo->total] = no->chaves[i];
            if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
            cont++;
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
            } else{
            cont++;
                adicionaChaveRecursivo(ArvoreB, no->pai, novo, promovido);
            }
        }
    }

    void adicionaChave(ArvoreB* ArvoreB, int chave) {
        No* no = localizaNo(ArvoreB, chave);

        adicionaChaveRecursivo(ArvoreB, no, NULL, chave);
    }

 void removerChave(ArvoreB* ArvoreB, int chave) {
    No* no = localizaNo(ArvoreB, chave);
    removerChaveRecursivo(ArvoreB, no, chave);
}
void removeChaveNo(No* no, int chave) {
    // Encontra a posição da chave a ser removida
    int indice = pesquisaBinaria(no, chave);
    //printf("Chave: %d, Índice: %d, Total: %d\n", chave, indice, no->total);

    // Se a chave está presente no nó
    if (indice < no->total && no->chaves[indice] == chave) {
        cont++;
        // Se o nó é uma folha
        if (no->filhos[indice] == NULL) {
            // Move todas as chaves à direita da chave removida para a esquerda
            for (int i = indice; i < no->total - 1; i++) {
                cont++;
                no->chaves[i] = no->chaves[i + 1];
                no->filhos[i + 1] = no->filhos[i + 2];
            }

            // Atualiza o número total de chaves no nó
            no->total--;
        } else {
            cont++;

            // Se o nó não é uma folha, substitua a chave pelo seu sucessor
            No* sucessor = no->filhos[indice + 1];

            // Encontra o menor valor na subárvore direita (sucessor)
            while (sucessor->filhos[0] != NULL) {
                cont++;
                sucessor = sucessor->filhos[0];
            }

            // Substitui a chave pelo sucessor
            no->chaves[indice] = sucessor->chaves[0];

            // Chama a função recursivamente para remover o sucessor
            removeChaveNo(sucessor, sucessor->chaves[0]);
        }
    } else if (no->filhos[indice] != NULL) {
        cont++;
        // Se a chave não está no nó, mas o filho correspondente pode conter a chave
        removeChaveNo(no->filhos[indice], chave);
    } else {
        cont++;
        // Chave não encontrada na árvore
        //printf("Chave não encontrada na árvore.\n");
    }
}

void removerChaveRecursivo(ArvoreB* ArvoreB, No* no, int chave) {
    if (no == NULL) {
        //printf("Chave não encontrada na árvore.\n");
        return;
    }

    int indice = pesquisaBinaria(no, chave);

    if (indice < no->total && no->chaves[indice] == chave) {
        cont+=2;
        removeChaveNo(no, chave);
    } else if (no->filhos[indice] != NULL) {
        cont++;
        removerChaveRecursivo(ArvoreB, no->filhos[indice], chave);
    } else {
        cont++;
        //printf("Chave não encontrada na árvore.\n");
        return;
    }
}
void rearranjaNo(ArvoreB* ArvoreB ,No* no, int indice) {
    No* filhoEsquerda = no->filhos[indice];
    No* filhoDireita = no->filhos[indice + 1];

    // Tenta redistribuir uma chave do filho direito
    if (indice < no->total && filhoDireita->total > ArvoreB->ordem) {
        cont++;
        // Move a chave pai para o nó atual
        no->chaves[indice] = filhoDireita->chaves[0];

        // Move a primeira chave do filho direito para o pai
        filhoDireita->chaves[0] = filhoDireita->chaves[1];

        // Reduz o número de chaves no filho direito
        filhoDireita->total--;

        // Move o primeiro filho do filho direito para o nó atual
        no->filhos[indice + 1] = filhoDireita->filhos[0];

        // Atualiza os ponteiros do filho direito
        for (int i = 0; i < filhoDireita->total; i++) {
            cont++;
            filhoDireita->chaves[i] = filhoDireita->chaves[i + 1];
            filhoDireita->filhos[i] = filhoDireita->filhos[i + 1];
        }
        filhoDireita->filhos[filhoDireita->total] = filhoDireita->filhos[filhoDireita->total + 1];
    }
    // Tenta redistribuir uma chave do filho esquerdo
    else if (indice > 0 && filhoEsquerda->total > ArvoreB->ordem) {
        cont+= 2;
        // Move a chave pai para o nó atual
        no->chaves[indice - 1] = filhoEsquerda->chaves[filhoEsquerda->total - 1];

        // Move a última chave do filho esquerdo para o pai
        filhoEsquerda->chaves[filhoEsquerda->total - 1] = filhoEsquerda->chaves[filhoEsquerda->total - 2];

        // Reduz o número de chaves no filho esquerdo
        filhoEsquerda->total--;

        // Move o último filho do filho esquerdo para o nó atual
        no->filhos[indice] = filhoEsquerda->filhos[filhoEsquerda->total + 1];
    }
    // Fusão com o filho direito
    else if (indice < no->total) {
        cont++;
        mergeNos(ArvoreB,no, indice);
    }
    // Fusão com o filho esquerdo
    else {
        cont++;
        mergeNos(ArvoreB,no, indice - 1);
    }
}
void mergeNos(ArvoreB* ArvoreB ,No* no, int indice) {
    No* filhoEsquerda = no->filhos[indice];
    No* filhoDireita = no->filhos[indice + 1];

    // Move a chave pai para o filho esquerdo
    filhoEsquerda->chaves[ArvoreB->ordem - 1] = no->chaves[indice];

    // Move as chaves do filho direito para o filho esquerdo
    for (int i = 0; i < filhoDireita->total; i++) {
        cont++;
        filhoEsquerda->chaves[i + ArvoreB->ordem] = filhoDireita->chaves[i];
        filhoEsquerda->filhos[i + ArvoreB->ordem] = filhoDireita->filhos[i];
    }
    filhoEsquerda->filhos[2 * ArvoreB->ordem] = filhoDireita->filhos[filhoDireita->total];

    // Atualiza o número total de chaves no filho esquerdo
    filhoEsquerda->total += filhoDireita->total + 1;

    // Move as chaves no nó pai
    for (int i = indice + 1; i < no->total; i++) {
        cont++;
        no->chaves[i - 1] = no->chaves[i];
        no->filhos[i] = no->filhos[i + 1];
    }

    // Atualiza o número total de chaves no nó pai
    no->total--;

    // Libera a memória do filho direito
    free(filhoDireita);
}
    int isThere(int *vet,int size,int key){
    for(int i = 0;i<size;i++){
        if(vet[i] == key){
            return 0;
        }
    }
    return 1;
    }

void avgcase1() {
    FILE* fp = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB1_Insercao.txt", "w+");
    FILE* fp2 = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB1_remocao.txt", "w+");
    srand(time(NULL));
    double insetions[100];
    double deletions[100];
    for (int j = 0; j < 10; j += 1) {
        float delete = 0;
        float insertion = 0;
        float stopers = 100;
        int m = 0;
        int n = 0;
        ArvoreB* a = criaArvore(1);
        int k = 0;
        int vet[10000];
        // Inserir 100 chaves aleatórias
        while (k < 10000)
        {
            cont++;
            int key = rand() %10000;
            if(isThere(vet,k,key)){
                adicionaChave(a, key);
                cont++;
                insertion += cont;
                vet[k] = key;
                k++;
                if(k == stopers ){
                    insetions[m] += insertion;
                    m++;
                    stopers += 100;
                }
                cont = 0;
            }
        }
        stopers = 100;
        // Randomly delete key
        for (int i = 0; i<=10000; i += 1) {
            int index = vet[i];
            removerChave(a, index);
            delete += cont;
            if(i == stopers ){
                deletions[n] += delete;
                n++;
                stopers += 100;
            }
            cont = 0;
        }
        free(a);
    }
    for(int i =0;i<100;i++){
        fprintf(fp,"%d %lf\n",(i+1)*100,insetions[i]/(10));
        fprintf(fp2,"%d %lf\n",(i+1)*100,deletions[i]/(10));
    }
    fclose(fp);
    fclose(fp2);
}
void avgcase5() {
    FILE* fp = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB2_Insercao.txt", "w+");
    FILE* fp2 = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB2_remocao.txt", "w+");
    srand(time(NULL));
    double insetions[100];
    double deletions[100];
    for (int j = 0; j < 10; j += 1) {
        float delete = 0;
        float insertion = 0;
        float stopers = 100;
        int m = 0;
        int n = 0;
        ArvoreB* a = criaArvore(5);
        int k = 0;
        int vet[10000];
        // Inserir 100 chaves aleatórias
        while (k < 10000)
        {
            cont++;
            int key = rand() %10000;
            if(isThere(vet,k,key)){
                adicionaChave(a, key);
                cont++;
                insertion += cont;
                vet[k] = key;
                k++;
                if(k == stopers ){
                    insetions[m] += insertion;
                    m++;
                    stopers += 100;
                }
                cont = 0;
            }
        }
        stopers = 100;
        // Randomly delete key
        for (int i = 0; i<=10000; i += 1) {
            int index = vet[i];
            removerChave(a, index);
            delete += cont;
            if(i == stopers ){
                deletions[n] += delete;
                n++;
                stopers += 100;
            }
            cont = 0;
        }
        free(a);
    }
    for(int i =0;i<100;i++){
        fprintf(fp,"%d %lf\n",(i+1)*100,insetions[i]/(10));
        fprintf(fp2,"%d %lf\n",(i+1)*100,deletions[i]/(10));
    }
    fclose(fp);
    fclose(fp2);
}
void avgcase10() {
    FILE* fp = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB3_Insercao.txt", "w+");
    FILE* fp2 = fopen("C:\\Users\\leodu\\Downloads\\NEWEDA\\AB3_remocao.txt", "w+");
    srand(time(NULL));
    double insetions[100];
    double deletions[100];
    for (int j = 0; j < 10; j += 1) {
        float delete = 0;
        float insertion = 0;
        float stopers = 100;
        int m = 0;
        int n = 0;
        ArvoreB* a = criaArvore(10);
        int k = 0;
        int vet[10000];
        // Inserir 100 chaves aleatórias
        while (k < 10000)
        {
            cont++;
            int key = rand() %10000;
            if(isThere(vet,k,key)){
                adicionaChave(a, key);
                cont++;
                insertion += cont;
                vet[k] = key;
                k++;
                if(k == stopers ){
                    insetions[m] += insertion;
                    m++;
                    stopers += 100;
                }
                cont = 0;
            }
        }
        stopers = 100;
        // Randomly delete key
        for (int i = 0; i<=10000; i += 1) {
            int index = vet[i];
            removerChave(a, index);
            delete += cont;
            if(i == stopers ){
                deletions[n] += delete;
                n++;
                stopers += 100;
            }
            cont = 0;
        }
        free(a);
    }
    for(int i =0;i<100;i++){
        fprintf(fp,"%d %lf\n",(i+1)*100,insetions[i]/(10));
        fprintf(fp2,"%d %lf\n",(i+1)*100,deletions[i]/(10));
    }
    fclose(fp);
    fclose(fp2);
}

    int main() {
        avgcase1();
        avgcase5();
        avgcase10();
    }
