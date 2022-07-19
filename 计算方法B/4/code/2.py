import numpy as np
import matplotlib.pyplot as plt


# Jacobi method solve the linear system A*x = b
def jacobi(A, b, epsilon):
    n = len(A)
    assert n == len(b)
    diag = np.diag(A)
    D = np.diag(diag)

    # R = I - D^-1 A
    # g = D^-1 b
    R = np.eye(n) - np.linalg.inv(D) @ A
    g = np.linalg.inv(D) @ b

    # X_{k+1} = R X_k + g
    x1 = np.zeros(n)
    x2 = np.ones(n)
    times = 0
    while np.linalg.norm(x1 - x2) > epsilon:
        x1 = x2
        x2 = R @ x1 + g
        times += 1
    return x2, times


# Gauss Seidel method solve the linear system A*x = b
def gaussSeidel(A, b, epsilon):
    n = len(A)
    assert n == len(b)
    diag = np.diag(A)
    D = np.diag(diag)
    L = np.tril(A, -1)
    U = np.triu(A, 1)

    # S = -(D + L)^-1 U
    # f = (D + L)^-1 b
    S = -np.linalg.inv(D + L) @ U
    f = np.linalg.inv(D + L) @ b

    # X(k+1) = S X(k) + f
    x1 = np.zeros(n)
    x2 = np.ones(n)
    times = 0
    while np.linalg.norm(x1 - x2) > epsilon:
        x1 = x2
        x2 = S @ x1 + f
        times += 1
    return x2, times


def test_solve():
    A = np.array([[2, -1, -1], [1, 5, -1], [1, 1, 10]])
    b = np.array([-5, 8, 11])

    x = jacobi(A, b)

    print('Jacobi:')
    print(x)
    print(A @ x)

    x = gaussSeidel(A, b, x, 1)

    print()
    print('Gauss Seidel:')
    print(x)
    print(A @ x)


# 当 $f(x) = \pi^2 \sin(\pi x)$，$n = 10, 20, 40, 80, 160$ 时，分别利用 Jacobi 和 Gauss-Seidel
# 方法求解 $A_h u_h = f_h$（迭代法的终止准则 $\epsilon = 10^{-10}$），
# 并比较 $u_h$ 与精确解 $u_e(x) = \sin(\pi x)$ 之间的误差 $e_h = ||u_h - u_e(x)||_2$，记录在一张表中


def f(x):
    return np.pi**2 * np.sin(np.pi * x)


def linear_fit(x, y):
    n = len(x)
    # 求和
    sum_x = sum(x)
    sum_y = sum(y)
    sum_xy = sum(x * y)
    sum_x2 = sum(x * x)
    # 求系数
    b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x)
    a2 = (sum_y - b * sum_x) / n
    a = (sum_x2 * sum_y - sum_x * sum_xy) / (n * sum_x2 - sum_x * sum_x)
    return a, b


def test_linear_fit():
    x = np.array(range(10))
    y = x + 10 + np.random.rand(10) * .1
    a, b = linear_fit(x, y)
    print('a =', a, 'b =', b)


# e_h = a * h ^ beta
# \ln e_h &= \ln a + \beta \ln h
def fitBeta(h, e_h, name):
    log_e_h = np.log(e_h)
    log_h = np.log(h)
    # use linear_fit
    log_a, beta = linear_fit(log_h, log_e_h)
    a = np.exp(log_a)

    # plot curve, with high resolution
    x = np.linspace(0, np.max(h), 100)
    y = a * x**beta
    plt.clf()
    plt.plot(x, y, 'r-', label='fit')
    plt.plot(h, e_h, 'bo', label='data')
    plt.legend()
    plt.xlabel('h')
    plt.ylabel('e_h')
    # save figure
    plt.savefig(name + '.png')
    return beta


def main():
    all_n = np.array([10, 20, 40, 80, 160])
    all_e_h_jacobi = []
    all_e_h_gaussSeidel = []
    all_times_jacobi = []
    all_times_gaussSeidel = []
    for n in all_n:
        # for n in [10]:
        h = 1 / n
        x = np.linspace(0, 1, n + 1)
        f_h = f(x)[1:-1]
        A = 2 * np.diag(np.ones(n - 1)) - np.diag(np.ones(n - 2), 1) - np.diag(np.ones(n - 2), -1)
        A_h = A / h**2
        u_e = np.sin(np.pi * x)[1:-1]

        u_h_jacobi, times_jacobi = jacobi(A_h, f_h, 1e-10)
        u_h_gaussSeidel, times_gaussSeidel = gaussSeidel(A_h, f_h, 1e-10)

        e_h_jacobi = np.linalg.norm(u_h_jacobi - u_e, 2)
        e_h_gaussSeidel = np.linalg.norm(u_h_gaussSeidel - u_e, 2)

        all_e_h_jacobi.append(e_h_jacobi)
        all_times_jacobi.append(times_jacobi)
        all_e_h_gaussSeidel.append(e_h_gaussSeidel)
        all_times_gaussSeidel.append(times_gaussSeidel)

        print('n =', n)
        print('u_e =', u_e)

        print('Jacobi:')
        print('e_h =', e_h_jacobi)
        print('u_h =', u_h_jacobi)
        print()
        print('Gauss Seidel:')
        print('e_h =', e_h_gaussSeidel)
        print('u_h =', u_h_gaussSeidel)
        print()

        if n == 10:
            for i in range(n - 1):
                print(f'u_{i+1} = {u_h_jacobi[i]}')

    all_e_h_jacobi = np.array(all_e_h_jacobi)
    all_e_h_gaussSeidel = np.array(all_e_h_gaussSeidel)

    for i in range(len(all_n)):
        # print(all_n[i], all_e_h_jacobi[i], all_e_h_gaussSeidel[i])
        print(f'{all_n[i]:5} {all_e_h_jacobi[i]:10.6f} {all_e_h_gaussSeidel[i]:10.6f}')

    print()

    # (3)
    all_h = 1 / all_n
    jacobi_beta = fitBeta(all_h, all_e_h_jacobi, name='Jacobi')
    gaussSeidel_beta = fitBeta(all_h, all_e_h_gaussSeidel, name='Gauss Seidel')
    print(f'Jacobi beta = {jacobi_beta:.6f}')
    print(f'Gauss Seidel beta = {gaussSeidel_beta:.6f}')
    print()

    # (4)
    # print times
    for i in range(len(all_n)):
        print(f'{all_n[i]:5} {all_times_jacobi[i]:10} {all_times_gaussSeidel[i]:10}')


# test_solve()
# test_linear_fit()
main()
