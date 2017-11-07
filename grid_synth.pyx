import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
from onestage import beam_width_est
import pydatatypes as pdt
cimport pydatatypes as pdt
np.import_array()

cdef public vector[ vector[ vector[float]]] pel_grid(pdt.Antenna *ant, double f0, double df, double f_res, double fs):
    py_ant = pdt.PyAntenna_factory(ant)
    beam_width = beam_width_est(antenna=py_ant, f0=f0, df=df, fs=fs, f_res=f_res)

    alpha_line = np.arrange(0, 2 * np.pi, beam_width[0])
    betta_line = np.arrange(0, np.pi, beam_width[1])

    grid = meshgrid(alpha_line, betta_line)

    return grid


cpdef vector[ vector[ vector[ float]]] meshgrid(x1, x2):

    x2 = x2.transpose()
    x1_mat = np.matmul(np.ones((1, x2.shape[0], 1)), x1)
    x2_mat = np.matmul(x2, np.ones((1, 1, x1.shape[0])))

    grid = np.concatenate((x1_mat, x2_mat), axis=0)
    return grid