import numpy as np
import sympy

# solve the equation
# f(x) = x^2 + y^2 - 1 = 0
# g(x) = x^3 - y = 0


def f(x, y):
    return x**2 + y**2 - 1


def g(x, y):
    return x**3 - y


# solve two equations using Newton's method
def solve(functionMatrix, jacobian, x, y, x0, y0, eps, maxIter):
    for k in range(maxIter):
        Jx0y0 = jacobian.subs([(x, x0), (y, y0)])
        f1f2x0y0 = functionMatrix.subs([(x, x0), (y, y0)])

        # solve Jx0y0 * [dx, dy] = -f1f2x0y0
        dxdy = -Jx0y0.LUsolve(f1f2x0y0)

        x0 += dxdy[0]
        y0 += dxdy[1]

        if max(abs(dxdy)) < eps:
            targetK = k + 1
            break
    return targetK, x0, y0


def main():
    x0 = .8
    y0 = .6
    eps = 1e-5
    maxIter = 100

    x = sympy.Symbol('x')
    y = sympy.Symbol('y')

    functionMatrix = sympy.Matrix([f(x, y), g(x, y)])
    jacobian = functionMatrix.jacobian([x, y])

    k, x, y = solve(functionMatrix, jacobian, x, y, x0, y0, eps, maxIter)
    print('k =', k)
    print("x = %.10f, y = %.10f" % (x, y))


main()
