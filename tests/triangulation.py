import sympy
import numpy
# from sympy.physics.mechanics import ReferenceFrame, Vector
from sympy.vector import CoordSys3D
from sympy import sin, cos
from sympy.solvers.solveset import linsolve
import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go

def calculate_vertex(phase_center0, peleng0, phase_center1, peleng1):

    x0 = phase_center0[0]
    y0 = phase_center0[1]
    z0 = phase_center0[2]
    alpha0 = peleng0[0]
    betta0 = peleng0[1]

    x1 = phase_center1[0]
    y1 = phase_center1[1]
    z1 = phase_center1[2]
    alpha1 = peleng1[0]
    betta1 = peleng1[1]

    normal = sympy.Matrix([[sin(alpha0)*sin(betta1)*cos(betta0) - sin(alpha1)*sin(betta0)*cos(betta1)],
                           [sin(betta0)*cos(alpha1)*cos(betta1) - sin(betta1)*cos(alpha0)*cos(betta0)],
                           [-sin(alpha0)*cos(alpha1)*cos(betta0)*cos(betta1) + sin(alpha1)*cos(alpha0)*cos(betta0)*cos(betta1)]])
    print("Нормаль: ", normal)
    # Пересечение проекции прямой a с прямой b
    # Находим три точки плоскости, проходящей через прямую a и нормаль n
    # # Первая точка это (x0, y0, z0)
    # # Второая точка
    # x2 = x0 + normal[0, 0]
    # y2 = y0 + normal[1, 0]
    # z2 = z0 + normal[2, 0]
    # #Третья точка
    # x3 = x0 + cos(betta0) * cos(alpha0)
    # y3 = y0 + cos(betta0) * sin(alpha0)
    # z3 = z0 + sin(betta0)

    # [(y2 - y1) * (z3 - z0) - (z2 - z0) * (y3 - y0),
    #  -(x2 - x0) * (z3 - z0) + (z2 - z0) * (x3 - x0),
    #  (x2 - x0) * (y3 - y0) - (y2 - y0) * (x3 - x0)]
    #

    # x, y, z = sympy.symbols("x y z")
    # D = sympy.Matrix([[x - x0, y - y0, z - z0],
    #                   [x2 - x0, y2 - y0, z2 - z0],
    #                   [x3 - x0, y3 - y0, z3 - z0]])
    # print("Det D: ", D.det())
    A = sympy.Matrix([
                     [-normal[1, 0]*sin(betta0) + normal[2, 0]*sin(alpha0)*cos(betta0),
                      normal[0, 0]*sin(betta0) - normal[2, 0]*cos(alpha0)*cos(betta0),
                      -normal[0, 0]*sin(alpha0)*cos(betta0) + normal[1, 0]*cos(alpha0)*cos(betta0)],
                     [normal[0, 0], normal[1, 0], normal[2, 0]],
                     [cos(betta1)*sin(alpha1), - cos(betta1)*cos(alpha1), 0],
                     [sin(betta1), 0, -cos(betta1)*cos(alpha1)],
                     [0, sin(betta1), -cos(betta1)*sin(alpha1)]])

    # [x0 * ((y2 - y1) * (z3 - z0) - (z2 - z0) * (y3 - y0))
    #  - y0 * ((x2 - x0) * (z3 - z0) + (z2 - z0) * (x3 - x0))
    #  + z0 * ((x2 - x0) * (y3 - y0) - (y2 - y0) * (x3 - x0))],

    b = sympy.Matrix([
                      [normal[0, 0]*y0*sin(betta0) - normal[0, 0]*z0*sin(alpha0)*cos(betta0)
                       - normal[1, 0]*x0*sin(betta0) + normal[1, 0]*z0*cos(alpha0)*cos(betta0)
                       + normal[2, 0]*x0*sin(alpha0)*cos(betta0) - normal[2, 0]*y0*cos(alpha0)*cos(betta0)],
                      [normal[0, 0]*x1 + normal[1, 0]*y1 + normal[2, 0]*z1],
                      [x1*cos(betta1)*sin(alpha1) - y1*cos(betta1)*cos(alpha1)],
                      [x1*sin(betta1) - z1*cos(betta1)*cos(alpha1)],
                      [y1*sin(betta1) - z1*cos(betta1)*sin(alpha1)]])

    print("A: ", A)
    print("b: ", b)
    p = (A.transpose() * A).inv() * A.transpose() * b
    print("пересечение проекции a с прямой b: ", p)
    print(A[0, 0]*2 + A[0, 1]*7 + A[0, 2]*20)

    B = sympy.Matrix([[cos(betta0)*sin(alpha0), - cos(betta0)*cos(alpha0), 0],
                      [sin(betta0), 0, - cos(betta0)*cos(alpha0)],
                      [0, sin(betta0), -cos(betta0)*sin(alpha0)],
                      [normal[1, 0], -normal[0, 0], 0],
                      [normal[2, 0], 0, -normal[0, 0]],
                      [0, normal[2, 0], -normal[1, 0]]])

    c = sympy.Matrix([[x0*cos(betta0)*sin(alpha0) - y0*cos(betta0)*cos(alpha0)],
                      [x0*sin(betta0) - z0*cos(betta0)*cos(alpha0)],
                      [y0*sin(betta0) - z0*cos(betta0)*sin(alpha0)],
                      [p[0, 0]*normal[1, 0] - p[1, 0]*normal[0, 0]],
                      [p[0, 0]*normal[2, 0] - p[2, 0]*normal[0, 0]],
                      [p[1, 0]*normal[2, 0] - p[2, 0]*normal[1, 0]]])

    print("B :", B)
    print("c :", c)
    q = (B.transpose() * B).inv() * B.transpose() * c
    print("пересечение нормали с прямой q: ", q)
    vertex = (p + q) / 2

    r = 40
    data = [go.Scatter3d(x=np.array([-r*np.cos(betta0)*np.cos(alpha0) + x0, r*np.cos(betta0)*np.cos(alpha0) + x0]),
                         y=np.array([-r*np.cos(betta0)*np.sin(alpha0) + y0, r*np.cos(betta0)*np.sin(alpha0) + y0]),
                         z=np.array([-r*np.sin(betta0) + z0, r*np.sin(betta0) + z0]),
                         mode="lines",
                         name="прямая а"),
            go.Scatter3d(x=np.array([-r * np.cos(betta1) * np.cos(alpha1) + x1, r * np.cos(betta1) * np.cos(alpha1) + x1]),
                         y=np.array([-r * np.cos(betta1) * np.sin(alpha1) + y1, r * np.cos(betta1) * np.sin(alpha1) + y1]),
                         z=np.array([-r * np.sin(betta1) + z1, r * np.sin(betta1) + z1]),
                         mode="lines",
                         name="прямая b"),
            go.Scatter3d(x=np.array([float(p[0, 0])]),
                         y=np.array([float(p[1, 0])]),
                         z=np.array([float(p[2, 0])]),
                         mode="markers",
                         name="Пересечение проекции прямой a c прямой b",
                         marker=go.Marker(symbol='square', color="red")),
            go.Scatter3d(x=np.array([float(q[0, 0])]),
                         y=np.array([float(q[1, 0])]),
                         z=np.array([float(q[2, 0])]),
                         mode="markers",
                         name="Пересечение нормали с прямой b",
                         marker=go.Marker(symbol='square', color="green")),
            go.Scatter3d(x=np.array([float(vertex[0, 0])]),
                         y=np.array([float(vertex[1, 0])]),
                         z=np.array([float(vertex[2, 0])]),
                         mode="markers",
                         name="Вершина"),
            ]
    ply.plot(data)
    return vertex


phase_center0 = numpy.array([1, 1, 1])
peleng0 = numpy.array([numpy.radians(180), numpy.radians(45)])
phase_center1 = numpy.array([0, 7, 20])
peleng1 = numpy.array([numpy.radians(33), numpy.radians(20)])
#
x0, y0, z0, alpha0, betta0 = sympy.symbols("x0 y0 z0 alpha0 betta0")  # прямая в
x1, y1, z1, alpha1, betta1 = sympy.symbols("x1 y1 z1 alpha1 betta1")  # прямая b
i, j, k, x, y, z = sympy.symbols("i j k x y z")

# 1 Плоскость проходящая через b и параллельна a
N = CoordSys3D("N")

a0 = cos(betta0)*cos(alpha0)*N.i + cos(betta0)*sin(alpha0)*N.j + sin(betta0)*N.k
a1 = cos(betta1)*cos(alpha1)*N.i + cos(betta1)*sin(alpha1)*N.j + sin(betta1)*N.k
r = x*N.i + y*N.j + z*N.k
M0 = x0*N.i + y0*N.j + z0*N.k
M1 = x1*N.i + y1*N.j + z1*N.k

a0 = a0.subs([(alpha0, peleng0[0]), (betta0, peleng0[1])])
a1 = a1.subs([(alpha1, peleng1[0]), (betta1, peleng1[1])])
M0 = M0.subs([(x0, phase_center0[0]), (y0, phase_center0[1]), (z0, phase_center0[2])])
M1 = M1.subs([(x1, phase_center1[0]), (y1, phase_center1[1]), (z1, phase_center1[2])])

print("a0 is: ", a0)
print("a1 is: ", a1)
# normal = a0 ^ a1
# # nx, ny, nz = sympy.symbols("nx ny nz")
# # normal = nx*N.i + ny*N.j + nz*N.k
#
# print("normal to the plane through a1 and parallel to a0 is:", normal)
# print("\n")
#
# # Проекция прямой a на эту плоскость
# eq1 = (r - M0) & (a0 ^ normal)
# print("Projections intersecting planes: ", sympy.expand(eq1))
# eq2 = (r & normal) - (normal & M1)
# print("Projections intersecting planes: ", sympy.expand(eq2))
#
# # Прямая b образована пересечением плоскостей
# eq3 = (x - x1)*cos(betta1)*sin(alpha1) - (y - y1)*cos(betta1)*cos(alpha1)
# eq3 = eq3.subs([(x1, phase_center1[0]), (y1, phase_center1[1]), (alpha1, peleng1[0]), (betta1, peleng1[1])])
# print("Plane a1 intersecting planes: ", eq3)
# eq4 = (x - x1)*sin(betta1) - (z - z1)*cos(betta1)*cos(alpha1)
# eq4 = eq4.subs([(x1, phase_center1[0]), (z1, phase_center1[2]), (alpha1, peleng1[0]), (betta1, peleng1[1])])
# print("Plane a1 intersecting planes: ", eq4)
#
# # Точка пересечения проекции a0 и a1
# ans = linsolve([eq1, eq2, eq3, eq4], x, y, z)
# (px, py, pz) = next(iter(ans))
# P = px * N.i + py * N.j + pz * N.k
# print("Пересечение проекции a0 с a1: ", P)
#
#
# # Точка пересечения a0 и вектора нормали из точки P
# eq5 = (x - M0.to_matrix(N)[0])*a0.to_matrix(N)[1] - (y - M0.to_matrix(N)[1])*a0.to_matrix(N)[0]
# print("Plane a0 intersecting planes: ", eq5)
# eq6 = (x - M0.to_matrix(N)[0])*a0.to_matrix(N)[2] - (z - M0.to_matrix(N)[2])*a0.to_matrix(N)[0]
# print("Plane a0 intersecting planes: ", eq6)
#
# if normal.to_matrix(N)[0] != 0:
#     eq7 = (x - px)*normal.to_matrix(N)[1] - (y - py)*normal.to_matrix(N)[0]
#     eq8 = (x - px)*normal.to_matrix(N)[2] - (z - pz)*normal.to_matrix(N)[0]
#
# if normal.to_matrix(N)[1] != 0:
#     eq7 = (x - px)*normal.to_matrix(N)[1] - (y - py)*normal.to_matrix(N)[0]
#     eq8 = (y - py)*normal.to_matrix(N)[2] - (z - pz)*normal.to_matrix(N)[1]
#
# if normal.to_matrix(N)[2] != 0:
#     eq7 = (x - px) * normal.to_matrix(N)[2] - (z - pz) * normal.to_matrix(N)[0]
#     eq8 = (y - py) * normal.to_matrix(N)[2] - (z - pz) * normal.to_matrix(N)[1]
#
# print("Normal intersecting planes: ", eq7)
# print("Normal intersecting planes: ", eq8)
#
# # Точка пересечения проекции нормали из P и a0
# ans = linsolve([eq5, eq7, eq8], x, y, z)
# (qx, qy, qz) = next(iter(ans))
# Q = qx * N.i + qy * N.j + qz * N.k
# print("Пересечение нормали из точки P с прямой a0: ", Q)
#
#
# vertex = (P + Q) / 2
# print("Вершина, рассчитанная символьной арифметикой:", vertex)

vertex = calculate_vertex(phase_center0, peleng0, phase_center1, peleng1)
print("Вершина, расчитанная численной арифметикой: ", vertex)