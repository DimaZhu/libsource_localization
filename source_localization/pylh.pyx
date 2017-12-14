import dtypes as dt
cimport dtypes as dt

cdef extern from "lh.h":
    cdef cppclass Lh:
        Lh();
        double calculate(double alpha, double betta) const
        void set_verbose(bool )

cdef extern from "lh_pel.h":
    cdef cppclass Lh_Pel:
        Lh_Pel(dt.Antenna *ant, dt.SpecFrame *data)
        double calculate(double alpha, double betta) const


cdef class PyLh_Pel:
    cdef Lh_Pel *lh
    def __cinit__(self, dt.PyAntenna ant, dt.PySpecFrame frame):
        cdef dt.Antenna *a = dt.Antenna_factory(ant)
        cdef dt.SpecFrame *s = dt.SpecFrame_factory(frame)
        self.lh = new Lh_Pel(a, s)

    def __dealoc__(self):
        if self.lh is not NULL:
            del self.lh

    def calc(self, peleng):
        cdef double output
        output = self.lh.calculate(peleng[0], peleng[1])
        return output

    def __reduce_cython__(self):
        pass