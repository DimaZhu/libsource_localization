#defining NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
from dtypes cimport *


from libcpp.vector cimport vector
import numpy as np
cimport numpy as np
from libc.stdlib cimport malloc, free
from cython.operator cimport dereference, address

np.import_array()

def smthing():
    return 2
#cdef class PyAntenna:
#    def __cinit__(self,):
#        self.c_ant = NULL
#
#    def __reduce_cython__(self):
#        pass
#
#    def load(self, filename):
#        fname_bytes = filename.encode("utf-8")
#        cdef char* c_str = fname_bytes
#
#        if self.c_ant is not NULL:
#            del self.c_ant
#
#        self.c_ant = new Antenna(c_str)
#
#    def __dealoc__(self):
#        if self.c_ant is not NULL:
#            del self.c_ant
#
#    def set_model(self, coord):
#
#        if self.c_ant is NULL:
#            self.c_ant = new Antenna()
#
#        cdef vector[vector[float]] c_coord = coord
#        self.c_ant.set_model(c_coord)
#
#    def set_base(self, coord):
#
#        if self.c_ant is NULL:
#            self.c_ant = new Antenna()
#
#        cdef vector[float] c_coord = coord
#        self.c_ant.set_base(c_coord)
#
#    def set_orientation(self, float val):
#
#        if self.c_ant is NULL:
#            self.c_ant = new Antenna()
#
#        self.c_ant.set_orientation(val)
#
#    def get_model(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        el_list = self.c_ant.get_model()
#        el_arr = np.asarray(el_list)
#        return el_arr
#
#    def get_elements(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        el_list = self.c_ant.get_elements()
#        el_arr = np.asarray(el_list)
#        return el_arr
#
#    def get_base(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        return self.c_ant.get_base()
#
#    def get_stages(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        return self.c_ant.get_elements_on_stages()
#
#    def get_orientation(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        return self.c_ant.get_orientation()
#
#    def get_channels_total(self):
#
#        if self.c_ant is NULL:
#            return None
#
#        return self.c_ant.get_channels_total()
#
#    def show(self):
#        """Antenna visualization via plotly
#        """
#        import plotly.offline as plt
#        import plotly.graph_objs as go
#
#        if self.c_ant is NULL:
#            return None
#
#        cart_coord = np.asarray(self.c_ant.get_elements())
#        color = 'green'
#        elements = go.Scatter3d(x=cart_coord[0, :],
#                                y=cart_coord[1, :],
#                                z=cart_coord[2, :],
#                                mode='markers',
#                                marker=go.Marker(symbol='square', color=color))
#
#        data = [elements]
#
#        for i in range(self.c_ant.get_channels_total()):
#            line = np.zeros((3, 2))
#            line[0, 0] = 0
#            line[1, 0] = 0
#            line[2, 0] = cart_coord[2, i]
#            line[0, 1] = cart_coord[0, i]
#            line[1, 1] = cart_coord[1, i]
#            line[2, 1] = cart_coord[2, i]
#            data.append(go.Scatter3d(x=line[0, :],
#                                     y=line[1, :],
#                                     z=line[2, :],
#                                     mode='lines',
#                                     line=go.Line(color=color, width=6)))
#
#        data.append(go.Scatter3d(x=np.array([0, 0]),
#                                 y=np.array([0, 0]),
#                                 z=np.array([0, max(cart_coord[2, :])]),
#                                 mode='lines',
#                                 line=go.Line(color=color, width=10)))
#
#        plt.plot(data)
#
#
#cdef object PyAntenna_factory(Antenna *ptr):
#    cdef PyAntenna py_ant = PyAntenna()
#    py_ant.c_ant = ptr
#    return py_ant
#
#cdef Antenna *Antenna_factory(PyAntenna ant):
#    cdef Antenna * a = ant.c_ant
#    return a
#
#cdef class PySpecFrame:
#    def __cinit__(self, channels_total = 0, samp_per_ch = 0):
#        self.c_frame = new SpecFrame(channels_total, samp_per_ch)
#
#    def __dealoc__(self):
#        if self.c_frame is not NULL:
#            del self.c_frame
#
#    def initialize(self, **kw):
#        cdef complex2d c_data
#
#        if "f0" in kw:
#            self.c_frame.set_carrier(kw['f0'])
#
#        if "fs" in kw:
#            self.c_frame.set_sampling_frequency(kw['fs'])
#
#        if 'f_res' in kw:
#            self.c_frame.set_frequency_resolution(kw['f_res'])
#
#        if 'sig' in kw:
#            # sig_list = kw['sig'].tolist()
#            sig = kw['sig']
#            self.convert_python_data(sig)
#
#
#    cdef void set_python_data(self, np.ndarray [np.complex128_t, ndim=2] data):
#
#        cdef int channels_total = len(data)
#        cdef int samp_per_ch = len(data[0])
#        cdef complex2d c_data
#
#        c_data = <complex2d > malloc( channels_total * sizeof(complex1d))
#        for ch in range(channels_total):
#            c_data[ch] = <complex1d> malloc(samp_per_ch * sizeof(complex))
#
#
#        for ch in range(channels_total):
#            for s in range(samp_per_ch):
#                c_data[ch][s] = data[ch, s]
#
#        self.c_frame.set_data(address(c_data), channels_total, samp_per_ch)
#
#    cdef np.ndarray[np.complex128_t, ndim=2] get_python_data(self):
#        cdef int channels_total =  self.c_frame.get_channels_total()
#        cdef int samp_per_channel = self.c_frame.get_length()
#        cdef complex2d * data_ptr = self.c_frame.get_data()
#        cdef complex2d data = dereference(data_ptr)
#
#        output = np.zeros(shape=(channels_total, samp_per_channel))
#        for i in range(channels_total):
#            for j in range(samp_per_channel):
#                output[i, j] = data[i][j]
#
#        return output
#
#
#    def get_data(self):
#        return self.get_python_data()
#
#    def get_carrier(self):
#        return self.c_frame.get_carrier()
#
#    def get_sampling_frequency(self):
#        return self.c_frame.get_sampling_frequency()
#
#    def get_frequency_resolution(self):
#        return self.c_frame.get_frequency_resolution()
#
#    def set_data(self, data):
#        self.set_python_data(data)
#
#    def set_carrier(self, f0):
#        self.c_frame.set_carrier(f0)
#
#    def set_sampling_frequency(self, fs):
#        self.c_frame.set_sampling_frequency(fs)
#
#    def set_frequency_resolution(self, f_res):
#        self.c_frame.set_frequency_resolution(f_res)
#
#
#
#
#
#cdef SpecFrame *SpecFrame_factory(PySpecFrame frame):
#    cdef SpecFrame *f = frame.c_frame
#    return f
