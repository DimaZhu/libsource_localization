import dtypes as dt
import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
import onestage

cdef public vector[vector[vector[double]]] pel_grid(vector[vector[float]] ant_coord, double f0, double df, double f_res, double fs):

    py_ant = dt.PyAntenna()
    py_ant.set_model(np.asarray(ant_coord))
    beam_width = onestage.beam_width_est(antenna=py_ant, f0=f0, df=df, fs=fs, f_res=f_res)

    samp_per_max = 3
    step_alpha = beam_width[0] / samp_per_max
    step_betta = beam_width[1] / samp_per_max
    # Делаем пространство поиска на один шаг больше в каджом направлении для успешной интерполяции
    # ФП на границе поиска
    alpha_line = np.linspace(-step_alpha, 2 * np.pi + step_alpha, 2 * np.pi/step_alpha + 2 , endpoint=True)
    betta_line = np.linspace(-np.pi/2 - step_betta, np.pi/2 + step_betta, np.pi/step_betta + 2, endpoint=True)

    grid = onestage.meshgrid(alpha_line, betta_line)

    return grid

cpdef pypel_grid(ant, f0, df, f_res, fs):
      ant_coord = ant.get_elements()
      return np.asarray(pel_grid(ant_coord, f0, df, f_res, fs))



