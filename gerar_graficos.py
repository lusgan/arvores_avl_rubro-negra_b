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

def criar_grafico_comparativo(arquivos, labels, tarefa, tamanho_y, nome_arquivo):
    plt.figure(figsize=(10, 6))
    
    for i, arquivo in enumerate(arquivos):
        dados = ler_arquivo(arquivo)
        plt.plot(dados["casos"], dados["esforco_insercao"], marker='o', label=f'{labels[i]} - {tarefa}')

    plt.title(f'Comparação de Esforço Computacional entre Estruturas de Árvore - {tarefa}')
    plt.xlabel('Número de nós')
    plt.ylabel('Esforço Computacional Médio')
    plt.grid(True)
    plt.legend()
    plt.ylim(0, 50)
    plt.savefig(nome_arquivo)
    plt.show()

if __name__ == "__main__":
    arquivosAdicao = ["avl_insercao.txt", "arb_insercao.txt", "AB1_Insercao.txt", "AB2_Insercao.txt", "AB3_Insercao.txt"]
    labels = ["AVL", "ARB", "AB1", "AB2", "AB3"]

    criar_grafico_comparativo(arquivosAdicao, labels, "Inserção", 0, "insercao.png")
    
    arquivosRemocao = ["avl_remocao.txt", "arb_remocao.txt", "AB1_remocao.txt", "AB2_remocao.txt", "AB3_remocao.txt"]
    criar_grafico_comparativo(arquivosRemocao, labels, "Remoção", 0, "remocao.png")
