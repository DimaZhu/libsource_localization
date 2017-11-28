import dtypes as dt
import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
import onestage

cdef public vector[vector[vector[float]]] pel_grid(vector[vector[float]] ant_coord, double f0, double df, double f_res, double fs):

    py_ant = dt.PyAntenna()
    py_ant.set_model(np.asarray(ant_coord))
    beam_width = onestage.beam_width_est(antenna=py_ant, f0=f0, df=df, fs=fs, f_res=f_res)

    alpha_line = np.arange(0, 2 * np.pi, beam_width[0])
    betta_line = np.arange(0, np.pi, beam_width[1])

    grid = onestage.meshgrid(alpha_line, betta_line)

    return grid


