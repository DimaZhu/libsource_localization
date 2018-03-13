"""
Рассчет экструмума эллептического параболоида
 для равномерной дискретизации x, y
"""
import sympy
from sympy import symbols

x1, x2, x3, x4, x5, x, dx = symbols('x1  x2 x3 x4 x5 x dx')
y1, y2, y3, y4, y5, y, dy = symbols('y1 y2 y3 y4 y5 y dy')
z1, z2, z3, z4, z5 = symbols('z1 z2 z3 z4 z5')

A = sympy.Matrix([[x**2, y**2, x, y, 1],
                  [(x + 2 * dx)**2, y**2, (x + 2 * dx), y, 1],
                  [(x + dx)**2, (y + dy)**2, (x + dx), (y + dy), 1],
                  [x**2, (y + 2*dy)**2, x, (y + 2*dy), 1],
                  [(x + 2*dx)**2, (y + 2*dy) ** 2, (x + 2*dx), (y + 2*dy), 1]
                  ])
b = sympy.Matrix([[z1], [z2], [z3], [z4], [z5]])

B = A.transpose() * A
print(B)
print(sympy.simplify(B))
C = sympy.factor(B)
print(C)
В = C.inv()
print(B)
# g = (A.transpose() * A).inv() * A.transpose * b
# print(g)
# print(sympy.simplify(g))