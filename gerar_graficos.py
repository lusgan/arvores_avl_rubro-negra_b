import matplotlib.pyplot as plt
import numpy as np

def ler_arquivo(nome_arquivo):
    dados = {"casos": [], "esforco_insercao": []}
    
    with open(nome_arquivo, 'r') as arquivo:
        for linha in arquivo:
            casos, esforco_insercao = map(float, linha.strip().split())
            dados["casos"].append(casos)
            dados["esforco_insercao"].append(esforco_insercao)
    
    return dados

def criar_grafico_comparativo(arquivos, labels, tarefa, tamanho_y):
    plt.figure(figsize=(10,6))
    
    for i, arquivo in enumerate(arquivos):
        dados = ler_arquivo(arquivo)
        plt.plot(dados["casos"], dados["esforco_insercao"], marker='o', label=f'{labels[i]} - {tarefa}')

    plt.title(f'Comparação de Esforço Computacional entre Estruturas de Árvore - {tarefa}')
    plt.xlabel('Número de nós')
    plt.ylabel('Esforço Computacional Médio')
    plt.grid(True)
    plt.legend()
    plt.ylim(28000, tamanho_y)  # Limita o eixo y entre 28000 e 1900000
    plt.show()

if __name__ == "__main__":
    arquivosAdicao = ["avl_insercao.txt", "arbi.txt", "ab1_insercao.txt", "ab2_insercao.txt", "ab3_insercao.txt"]
    labels = ["AVL", "ARB", "AB1", "AB2", "AB3"]

    criar_grafico_comparativo(arquivosAdicao, labels, "Inserção",4000000)
    
    arquivosRemocao = ["avl_remocao.txt", "arbr.txt", "ab1_remocao.txt", "ab2_remocao.txt", "ab3_remocao.txt"]
    criar_grafico_comparativo(arquivosRemocao, labels, "Remoção",1000000)
