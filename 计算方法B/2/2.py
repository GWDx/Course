import sympy
import numpy as np

# standard = integrate ln x, {x,1,2}
x = sympy.Symbol('x')
standard = sympy.integrate(sympy.ln(x), (x, 1, 2))


def compositeTrapezoidIntegrate(f, a, b, n):
    h = (b - a) / n
    # ans = h (f(a)/2 + sum(f(a+i h),{i,1,n-1}) + f(b)/2)
    ans = h * (f(a) / 2 + sum(f(a + i * h) for i in range(1, n)) + f(b) / 2)
    return ans


def autoAccurancyControlSimpsonIntegrate(f, a, b, m, eps):
    n = m
    e = eps
    h = (b - a) / n
    T2 = compositeTrapezoidIntegrate(f, a, b, n)
    T1 = T2 + 100 * eps
    while abs(T1 - T2) > e:
        T1 = T2
        x2 = np.linspace(a, b, 2 * n + 1)
        # H = h sum(f(x[2i+1]),{i,0,n-1})
        H = h * sum(f(x2[2 * i + 1]) for i in range(n))
        T2 = (T1 + H) / 2
        h = h / 2
        n = n * 2
    return T2


def f(x):
    return np.log(x)


def main():
    # print(compositeTrapezoidIntegrate(f, 1, 2, 2))
    value = autoAccurancyControlSimpsonIntegrate(f, 1, 2, 2, 1e-4)
    print(value)
    print(standard.evalf())
    print(standard.evalf() - value)


main()
