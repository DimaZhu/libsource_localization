import numpy as np


def interpolate_min(x, y, func):

    if x.shape[1] is not y.shape[0] or x.shape[2] is not y.shape[1]:
        raise RuntimeError("x and y matrices doesn't match")

    arg_min = x[:, 0, 0]
    fun_min = y[0, 0]

    for i in range(y.shape[0] - 2):
        for j in range(y.shape[1] - 2):

            x1 = x[0, i, j]
            y1 = x[1, i, j]
            z1 =    y[i, j]

            x2 = x[0, i, j+1]
            y2 = x[1, i, j+1]
            z2 =    y[i, j+1]

            x3 = x[0, i, j+2]
            y3 = x[1, i, j+2]
            z3 =    y[i, j+2]

            x4 = x[0, i+2, j]
            y4 = x[1, i+2, j]
            z4 =    y[i+2, j]

            x5 = x[0, i+1, j+1]
            y5 = x[1, i+1, j+1]
            z5 =    y[i+1, j+1]

            x6 = x[0, i+2, j+2]
            y6 = x[1, i+2, j+2]
            z6 =    y[i+2, j+2]

            observation_mat = np.matrix([[1, y1, x1, x1*y1, y1**2, x1**2],
                                         [1, y2, x2, x2*y2, y2**2, x2**2],
                                         [1, y3, x3, x3*y3, y3**2, x3**2],
                                         [1, y4, x4, x4*y4, y4**2, x4**2],
                                         [1, y5, x5, x5*y5, y5**2, x5**2],
                                         [1, y6, x6, x6*y6, y6**2, x6**2]])

            observations = np.matrix([[z1],
                                      [z2],
                                      [z3],
                                      [z4],
                                      [z5],
                                      [z6]])

            try:
                pol_coef = (observation_mat.getT() * observation_mat).getI() * observation_mat.getT() * observations
                a = np.matrix([[2 * pol_coef.item(5), pol_coef.item(3)],
                               [pol_coef.item(3), 2 * pol_coef.item(4)]])
                b = np.matrix([[-pol_coef.item(2)],
                               [-pol_coef.item(1)]])
                extreme = (a.getT() * a).getI() * a.getT() * b

                # Проверка на выход за область допустимых значений
                if (extreme[0] < x[0, 0, 0]) or (extreme[0] > x[0, 0, x.shape[2]-1]):
                    continue

                if (extreme[1] < x[1, 0, 0]) or (extreme[1] > x[1, x.shape[1]-1, 0]):
                    continue


                # Проверка на признак параболоида
                if pol_coef.item(5) * pol_coef.item(4) < 0:
                    continue

                fun_val = func(extreme)

                if fun_val < fun_min:
                    fun_min = fun_val
                    arg_min = extreme
            except np.linalg.linalg.LinAlgError:
                print("Got singular matrix during interpolation")



    return arg_min, fun_min

