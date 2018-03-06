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

cdef extern from "lhpel.h":
    cdef cppclass LhPel:
        LhPel(dt.Antenna *ant, dt.SpecFrame *data, int samp_start, int samp_stop, bool verbose)
        double calculate(double alpha, double betta) const
        void set_verbose(bool arg)

cdef extern  from "peleng.h" nogil:
    cdef cppclass Peleng:
        Peleng();
        double azimuth;
        double elevation;
        double probability;
        vector[vector[float]] phase_center;
        long int time;

cdef extern from "pelengator.h" nogil:
    cdef cppclass Pelengator:
        Pelengator(const dt.Antenna & ant, double frequency_resolution, double sampling_frequency)

        void set_signal_param(double f0, int samp_start, int samp_stop)
        Peleng estimate(dt.SpecFrame *frame)
        void turn_on_interpolation(bool turn_on)
        void set_verbose(bool)

cdef extern from "triangulator.h" nogil:
    cdef cppclass Triangulator:
        Triangulator();
        void set_active(bool arg);
        bool is_active();
        vector[vector[float]] estimate(vector[Peleng] pelengs);

cdef class PyLhPel:
    cdef LhPel *lh
    def __cinit__(self, dt.PyAntenna ant, dt.PySpecFrame frame, samp_start, samp_stop, verbose = False):
        cdef dt.Antenna *a = dt.antenna_factory(ant)
        cdef dt.SpecFrame *s = dt.spec_frame_factory(frame)
        self.lh = new LhPel(a, s, samp_start, samp_stop, verbose)

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
        cdef dt.Antenna *c_ant = dt.antenna_factory(py_ant)
        self.pelengator = new Pelengator(c_ant[0], frequency_resolution, sampling_frequency)

    def __dealoc__(self):
        if self.pelengator is not NULL:
            del self.pelengator

    def set_signal_parameters(self, double f0, int samp_start, int samp_stop):
        self.pelengator.set_signal_param(f0, samp_start, samp_stop)

    def estimate(self, dt.PySpecFrame py_frame):
        cdef dt.SpecFrame *c_frame = dt.spec_frame_factory(py_frame)
        cdef Peleng estimation = self.pelengator.estimate(c_frame)
        return py_peleng_factory(estimation)

    def turn_on_interpolation(self, bool turn_on):
        self.pelengator.turn_on_interpolation(turn_on)

    def set_verbose(self, arg):
        self.pelengator.set_verbose(arg)


cdef class PyTriangulator:
    cdef Triangulator c_triangulator
    def __cinit__(self):
        self.c_triangulator = Triangulator()

    def estimate(self, pelengs):
        cdef vector[Peleng] c_pelengs
        cdef vector[vector[float]] target

        for i in range(len(pelengs)):
            c_pelengs.push_back(peleng_factory(pelengs[i]))

        target = self.c_triangulator.estimate(c_pelengs)
        return np.asarray(target)

cdef class PyPeleng:
    cdef float azimuth
    cdef float elevation
    cdef float probability
    cdef long int time
    cdef np.ndarray phase_center

    def __init__(self):
        self.phase_center = np.zeros(3)
        self.azimuth = float('nan')
        self.elevation = float('nan')
        self.probability = float('nan')
        self.time = 0

    def get_azimuth(self):
        return self.azimuth

    def set_azimuth(self, azimuth):
        self.azimuth = azimuth

    def get_elevation(self):
        return self.elevation

    def set_elevation(self, elevation):
        self.elevation = elevation

    def get_probability(self):
        return self.probability

    def set_probability(self, probability):
        self.probability = probability

    def get_phase_center(self):
        return self.phase_center

    def set_phase_center(self, phase_center):
        self.phase_center = phase_center


cdef Peleng peleng_factory(PyPeleng pel):
    cdef Peleng c_pel = Peleng()
    c_pel.azimuth = pel.azimuth
    c_pel.elevation = pel.elevation
    c_pel.phase_center = pel.phase_center.tolist()
    c_pel.time = pel.time
    return c_pel

cdef PyPeleng py_peleng_factory(Peleng c_pel):
    cdef PyPeleng pel = PyPeleng()
    pel.azimuth = c_pel.azimuth
    pel.elevation = c_pel.elevation
    pel.phase_center = c_pel.phase_center
    pel.probability = c_pel.probability
    pel.time = c_pel.time