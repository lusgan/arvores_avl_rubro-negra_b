import matplotlib.pyplot as plt
import numpy as np

def ler_arquivo(nome_arquivo):
    dados = {"casos": [], "esforco_insercao": [], "esforco_remocao": []}
    
    with open(nome_arquivo, 'r') as arquivo:
        for linha in arquivo:
            casos, esforco_insercao, esforco_remocao = map(float, linha.strip().split())
            dados["casos"].append(casos)
            dados["esforco_insercao"].append(esforco_insercao)
            dados["esforco_remocao"].append(esforco_remocao)
    
    return dados

def criar_grafico_comparativo(arquivos, labels):
    plt.figure(figsize=(10, 6))
    
    for i, arquivo in enumerate(arquivos):
        dados = ler_arquivo(arquivo)
        plt.plot(dados["casos"], dados["esforco_insercao"], marker='o', label=f'{labels[i]} - Inserção')
        plt.plot(dados["casos"], dados["esforco_remocao"], marker='o', label=f'{labels[i]} - Remoção')

    plt.title('Comparação de Esforço Computacional entre Estruturas de Árvore')
    plt.xlabel('Número de nós')
    plt.ylabel('Esforço Computacional Médio')
    plt.grid(True)
    plt.legend()
    plt.ylim(28000, 1900000)  # Limita o eixo y entre 28000 e 1900000
    plt.show()

if __name__ == "__main__":
    arquivos = ["avl.txt", "arb.txt", "ab1.txt", "ab2.txt", "ab3.txt"]
    labels = ["AVL", "ARB", "AB1", "AB2", "AB3"]

    criar_grafico_comparativo(arquivos, labels)
