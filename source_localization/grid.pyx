import dtypes as dt
cimport dtypes as dt
import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
import onestage
from libc.stdio cimport printf

# public vector[vector[vector[double]]]

cdef api vector[vector[vector[double]]] pel_grid(vector[vector[float]] ant_coord, double f0, double df, double f_res, double fs):
    cdef float alpha_width
    cdef float betta_width
    cdef vector[vector[vector[double]]] grid

    # py_ant = dt.PyAntenna()
    # py_ant.set_model(np.asarray(ant_coord))
    #
    # if fs/2 <= f0:
    #     fs = 2.1 * f0

    # beam_width = onestage.beam_width_est(antenna=py_ant, f0=f0, df=df, fs=fs, f_res=f_res)
    # alpha_width = 180 / np.pi * beam_width[0]
    # betta_width = 180 / np.pi * beam_width[1]
    #
    # printf("Beam width, alpha [deg]  %f\n", alpha_width)
    # printf("Beam width, betta [deg] %f\n",  betta_width)

    samp_per_max = 3
    step_alpha = 24.6 / samp_per_max
    step_betta = 11.7 / samp_per_max
    # Делаем пространство поиска на один шаг больше в каджом направлении для успешной интерполяции
    # ФП на границе поиска
    alpha_line = np.linspace(-step_alpha, 2 * np.pi + step_alpha, 2 * np.pi/step_alpha + 3 , endpoint=True)
    betta_line = np.linspace(-np.pi/2 - step_betta, np.pi/2 + step_betta, np.pi/step_betta + 3, endpoint=True)

    grid = onestage.meshgrid(alpha_line, betta_line)


    return grid

cpdef pypel_grid(ant, f0, df, f_res, fs):
      ant_coord = ant.get_elements()
      return np.asarray(pel_grid(ant_coord, f0, df, f_res, fs))



