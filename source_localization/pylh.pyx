import dtypes as dt
cimport dtypes as dt
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