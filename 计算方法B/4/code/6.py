import numpy as np
import sympy
import matplotlib.pyplot as plt


def solve(functionMatrix, jacobian, var, var_init, eps, maxIter):
    var_value = var_init
    for k in range(maxIter):
        # Jx0y0 = jacobian.subs([(x, x0), (y, y0)])
        pair_var_value = list(zip(var, var_value))
        Jvalue = jacobian.subs(pair_var_value)
        # f1f2x0y0 = functionMatrix.subs([(x, x0), (y, y0)])
        fvalue = functionMatrix.subs(pair_var_value)

        # solve Jx0y0 * [dx, dy] = -f1f2x0y0
        # dxdy = -Jx0y0.LUsolve(f1f2x0y0)
        dValue = -Jvalue.LUsolve(fvalue)

        # x0 += dxdy[0]
        # y0 += dxdy[1]
        for i in range(len(var)):
            var_value[i] += dValue[i]

        if max(abs(dValue)) < eps:
            targetK = k + 1
            break
    return targetK, var_value


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


# 当 $f(x) = \pi^2 \sin(\pi x) + \sin^3(\pi x)$，$n = 10, 20, 40, 80, 160$ 时，
# 利用牛顿迭代法求解上一小题中的非线性方程组，并比较 $u_h$ 与精确解 $u_e(x) = \sin(\pi x)$ 之间的误差
# $e_h = ||u_h - u_e(x)||_2$，记录在一张表中，并利用最小二乘法找出该情形下算法的收敛阶


# f(x) = \pi^2 \sin(\pi x) + \sin^3(\pi x)$
def f(x):
    return np.pi**2 * np.sin(np.pi * x) + np.sin(np.pi * x)**3


def main():
    all_n = np.array([10, 20, 40, 80, 160])
    all_e_h = []

    for n in all_n:
        eps = 1e-8
        maxIter = 100

        u = [0]
        for i in range(1, n):
            u.append(sympy.Symbol(f'u_{i}'))
        u.append(0)

        u_var = u[1:-1]

        h = 1 / n
        x = np.linspace(0, 1, n + 1)
        all_f_h = f(x)
        u_e = np.sin(np.pi * x)[1:-1]

        # 2u_k - u_{k+1} - u_{k-1} + (u_k^3 - f_k) h^2 = 0
        all_eqns = []
        for i in range(1, n):
            all_eqns.append(2 * u[i] - u[i - 1] - u[i + 1] + (u[i]**3 - all_f_h[i]) * h * h)

        # functionMatrix = sympy.Matrix([f(x, y), g(x, y)])
        # jacobian = functionMatrix.jacobian([x, y])

        functionMatrix = sympy.Matrix(all_eqns)
        jacobian = functionMatrix.jacobian(u_var)

        # k, x, y = solve(functionMatrix, jacobian, x, y, x0, y0, eps, maxIter)
        # print('k =', k)
        # print("x = %.10f, y = %.10f" % (x, y))

        u_var_init = np.zeros(n - 1)
        k, u_value = solve(functionMatrix, jacobian, u_var, u_var_init, eps, maxIter)

        e_h = np.linalg.norm(u_value - u_e, 2)
        all_e_h.append(e_h)

        print('k =', k)
        for i in range(len(u_value)):
            print(f'u_{i+1} = {u_value[i]}')
        print()

    all_e_h = np.array(all_e_h)
    for i in range(len(all_n)):
        print(f'{all_n[i]:5} {all_e_h[i]:10.6f}')

    print()

    all_h = 1 / all_n
    # fit beta
    beta = fitBeta(all_h, all_e_h, 'Newton')
    print('beta =', beta)


main()
