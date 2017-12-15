import dtypes as dt
cimport dtypes as dt
from libcpp.vector cimport vector
import numpy as np
cimport numpy as np
from libcpp cimport bool


cdef extern from "pelengator.h" nogil:
    cdef cppclass Pelengator:
        Pelengator(const dt.Antenna & ant)

        void set_signal_param(double f0, double df, double f_res, double fs)
        vector[double] estimate(dt.SpecFrame frame)
        void turn_on_interpolation(bool turn_on)

cdef class PyPelengator:
    cdef Pelengator *pelengator
    def __init__(self, dt.PyAntenna py_ant):
        cdef dt.Antenna *c_ant = dt.Antenna_factory(py_ant)
        self.pelengator = new Pelengator(c_ant[0])

    def __dealoc__(self):
        if self.pelengator is not NULL:
            del self.pelengator

    def set_signal_parameters(self, double f0, double df, double f_res, double fs):
        self.pelengator.set_signal_param(f0, df, f_res, fs)

    def estimate(self, dt.PySpecFrame py_frame):
        cdef dt.SpecFrame *c_frame = dt.SpecFrame_factory(py_frame)
        cdef vector[double] estimation = self.pelengator.estimate(c_frame[0])
        return np.asarray(estimation)

    def turn_on_interpolation(self, bool turn_on):
        self.pelengator.turn_on_interpolation(turn_on)


