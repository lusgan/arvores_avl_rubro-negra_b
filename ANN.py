import numpy as np
from scipy.interpolate import lagrange
import numpy as np
from scipy.interpolate import lagrange

# Dados fornecidos
x_values = np.array([0.15, 0.20, 0.25, 0.30, 0.35, 0.40])
f_values = np.array([0.12, 0.16, 0.19, 0.22, 0.25, 0.27])

# Ponto de interesse
target_f = 0.23

# Interpolação Polinomial de Lagrange
poly_lagrange = lagrange(f_values - target_f, x_values)
x_estimate_lagrange = poly_lagrange(0)

coefficients_newton = np.polyfit(f_values - target_f, x_values, 2)
x_estimate_newton = np.polyval(coefficients_newton, 0)

print("Estimativa usando Interpolação Polinomial de Lagrange:", x_estimate_lagrange)
print("Estimativa usando Interpolação Polinomial de Newton:", x_estimate_newton)


import numpy as np
print("Qustão 2\n")
# Valor máximo de |cos(x)| no intervalo [1, 2]
max_cos_x = np.abs(np.cos(1))

# Limite de erro desejado
error_limit = 1e-6

# Calcular o espaçamento mínimo necessário (h)
min_spacing = np.sqrt(8 * error_limit / max_cos_x)

# Calcular o número mínimo de pontos necessários
min_points = int(np.ceil((2 - 1) / min_spacing)) + 1

print("Número mínimo de pontos necessários:", min_points)

import numpy as np
import matplotlib.pyplot as plt
print("\nQustão 3\n")
# Dados fornecidos
x = np.array([0.5, 0.75, 1, 1.5, 2.0, 2.5, 3.0])
fx = np.array([-2.8, -0.6, 1, 3.2, 4.8, 6.0, 7.0])

# Ajuste de uma curva polinomial de grau 2 (pode ser alterado conforme necessário)
coefficients = np.polyfit(x, fx, 2)

# Criar uma função polinomial a partir dos coeficientes
poly_function = np.poly1d(coefficients)

# Gerar pontos para o gráfico da curva ajustada
x_fit = np.linspace(min(x), max(x), 100)
y_fit = poly_function(x_fit)
# Ajuste de uma curva polinomial de grau 2
coefficients = np.polyfit(x, fx, 2)

# Criar uma função polinomial a partir dos coeficientes
poly_function = np.poly1d(coefficients)

# Plotar os dados de dispersão e a curva ajustada
plt.scatter(x, fx, label='Dados de dispersão')
plt.plot(x_fit, y_fit, label='Curva ajustada (grau 2)', color='red')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.title('Ajuste de Curva aos Dados')
plt.show()
# Imprimir a equação da curva ajustada
equation_str = f"Ajuste: {coefficients[0]:.4f}x^2 + {coefficients[1]:.4f}x + {coefficients[2]:.4f}"
print(equation_str)
print("\nQustão 4\n")
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Dados fornecidos
t = np.array([-9, -6, -4, -2, 0, 2, 4])
u = np.array([30, 10, 9, 6, 5, 4, 4])

# Definir as funções para ajuste
def x_function(t, a, b):
    return t / (a * t + b)

def y_function(t, a, b):
    return a * b * t

# Ajuste usando curve_fit para x(t) = t/(at + b)
try:
    params_x, covariance_x = curve_fit(x_function, t, u)
    y_fit_x = x_function(t, *params_x)
    r_squared_x = 1 - np.sum((u - y_fit_x)**2) / np.sum((u - np.mean(u))**2)
except Exception as e:
    print(f"Ajuste para x(t) falhou: {e}")
    params_x = [np.nan, np.nan]
    r_squared_x = np.nan

# Ajuste usando curve_fit para y(t) = abt
try:
    params_y, covariance_y = curve_fit(y_function, t, u)
    y_fit_y = y_function(t, *params_y)
    r_squared_y = 1 - np.sum((u - y_fit_y)**2) / np.sum((u - np.mean(u))**2)
except Exception as e:
    print(f"Ajuste para y(t) falhou: {e}")
    params_y = [np.nan, np.nan]
    r_squared_y = np.nan

# Comparar os R^2
print("R^2 para x(t) = t/(at + b):", r_squared_x)
print("Parâmetros para x(t):", params_x)
print("R^2 para y(t) = abt:", r_squared_y)
print("Parâmetros para y(t):", params_y)

# Plotar os dados e as curvas ajustadas
plt.scatter(t, u, label='Dados')
plt.plot(t, y_fit_x, label=f'x(t) Fit (R^2={r_squared_x:.4f})')
plt.plot(t, y_fit_y, label=f'y(t) Fit (R^2={r_squared_y:.4f})')
plt.xlabel('t')
plt.ylabel('u(t)')
plt.legend()
plt.title('Comparação dos Ajustes')
plt.show()


print("\nQustão 5\n")
import numpy as np

def polinomio_quadratico(x, coeficientes):
    return coeficientes[0] * x**2 + coeficientes[1] * x + coeficientes[2]

def interpolacao_por_partes(x, y):
    n = len(x) - 1
    coeficientes = np.zeros((n, 3))

    for i in range(n):
        A = np.array([
            [x[i]**2, x[i], 1],
            [x[i+1]**2, x[i+1], 1],
            [2*x[i], 1, 0],
        ])
        B = np.array([y[i], y[i+1], 0])
        coeficientes[i, :] = np.linalg.solve(A, B)

        # Imprime a equação do polinômio quadrático para o intervalo atual
        print(f'Intervalo {i+1}: P(x) = {coeficientes[i, 0]:.4f}x^2 + {coeficientes[i, 1]:.4f}x + {coeficientes[i, 2]:.4f}')

    return coeficientes

def avaliar_interpolacao(x_interp, coeficientes, x_intervalos):
    y_interp = np.zeros_like(x_interp)

    for i, coef in enumerate(coeficientes):
        mask = np.logical_and(x_interp >= x_intervalos[i], x_interp <= x_intervalos[i+1])
        y_interp[mask] = polinomio_quadratico(x_interp[mask], coef)

    return y_interp

x_tabela = np.array([0, 1/6, 1/3, 1/2, 2/3, 5/6, 1])
y_tabela = np.array([1, 3, 2, 1, 0, 2, 1])

coeficientes_interp = interpolacao_por_partes(x_tabela, y_tabela)

x_intervalos = np.linspace(0, 1, 1000)
y_interp = avaliar_interpolacao(x_intervalos, coeficientes_interp, x_tabela)

import matplotlib.pyplot as plt

plt.scatter(x_tabela, y_tabela, label='Pontos da Tabela')
plt.plot(x_intervalos, y_interp, label='Interpolação por Partes')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.legend()
plt.show()

