import matplotlib.pyplot as plt
import numpy as np

def ler_arquivo(nome_arquivo):
    dados = {"esforco": [], "quantidade": []}
    
    with open(nome_arquivo, 'r') as arquivo:
        for linha in arquivo:
            quantidade, esforco = map(float, linha.strip().split())
            dados["quantidade"].append(quantidade)
            dados["esforco"].append(esforco)
    
    return dados

def criar_grafico_comparativo(arquivos, labels):
    plt.figure(figsize=(10, 6))
    
    for i, arquivo in enumerate(arquivos):
        dados = ler_arquivo(arquivo)
        plt.plot(dados["quantidade"], dados["esforco"], marker='o', label=labels[i])

    plt.title('Comparação de Esforço Computacional entre Estruturas de Árvore')
    plt.xlabel('Quantidade de Dados')
    plt.ylabel('Esforço Computacional Médio')
    plt.grid(True)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    arquivos = ["avl.txt", "rubro_negra.txt", "arvore_b.txt"]
    labels = ["AVL", "Rubro-Negra", "Árvore B"]

    criar_grafico_comparativo(arquivos, labels)
