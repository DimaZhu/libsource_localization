import dtypes as dt
cimport dtypes as dt
from libcpp.vector cimport vector
import numpy as np
cimport numpy as np
from libcpp cimport bool

cdef extern from "lh.h":
    cdef cppclass Lh:
        Lh();
        double calculate(double alpha, double betta) const
        void set_verbose(bool arg)

cdef extern from "lh_pel.h":
    cdef cppclass Lh_Pel:
        Lh_Pel(dt.Antenna *ant, dt.SpecFrame *data, int samp_start, int samp_stop, bool verbose)
        double calculate(double alpha, double betta) const
        void set_verbose(bool arg)

cdef extern from "pelengator.h" nogil:
    cdef cppclass Pelengator:
        Pelengator(const dt.Antenna & ant, double frequency_resolution, double sampling_frequency)

        void set_signal_param(double f0, int samp_start, int samp_stop)
        vector[double] estimate(dt.SpecFrame *frame)
        void turn_on_interpolation(bool turn_on)
        void set_verbose(bool)


cdef class PyLh_Pel:
    cdef Lh_Pel *lh
    def __cinit__(self, dt.PyAntenna ant, dt.PySpecFrame frame, samp_start, samp_stop, verbose = False):
        cdef dt.Antenna *a = dt.Antenna_factory(ant)
        cdef dt.SpecFrame *s = dt.SpecFrame_factory(frame)
        self.lh = new Lh_Pel(a, s, samp_start, samp_stop, verbose)



    def __dealoc__(self):
        if self.lh is not NULL:
            del self.lh

    def calc(self, peleng):
        cdef double output

        output = self.lh.calculate(peleng[0], peleng[1])
        return output

    def set_verbose(self, arg):
        self.lh.set_verbose(arg)

    def __reduce_cython__(self):
        pass

cdef class PyPelengator:
    cdef Pelengator *pelengator
    def __init__(self, dt.PyAntenna py_ant, frequency_resolution, sampling_frequency):
        cdef dt.Antenna *c_ant = dt.Antenna_factory(py_ant)
        self.pelengator = new Pelengator(c_ant[0], frequency_resolution, sampling_frequency)

    def __dealoc__(self):
        if self.pelengator is not NULL:
            del self.pelengator

    def set_signal_parameters(self, double f0, int samp_start, int samp_stop):
        self.pelengator.set_signal_param(f0, samp_start, samp_stop)

    def estimate(self, dt.PySpecFrame py_frame):
        cdef dt.SpecFrame *c_frame = dt.SpecFrame_factory(py_frame)
        cdef vector[double] estimation = self.pelengator.estimate(c_frame)
        return np.asarray(estimation)

    def turn_on_interpolation(self, bool turn_on):
        self.pelengator.turn_on_interpolation(turn_on)

    def set_verbose(self, arg):
        self.pelengator.set_verbose(arg)



