# distutils: language = c++
# distutils: sources = datatypes/antenna.cpp

from pydatatypes cimport Antenna
from pydatatypes cimport SpecFrame

import numpy as np
cimport numpy as np
from libcpp.vector cimport vector
np.import_array()

cdef class PyAntenna:
    cdef Antenna *c_ant

    def __cinit__(self,):
        self.c_ant = NULL

    def load(self, filename):
        fname_bytes = filename.encode("utf-8")
        cdef char* c_str = fname_bytes

        if self.c_ant is not NULL:
            del self.c_ant

        self.c_ant = new Antenna(c_str)

    def __dealoc__(self):
        if self.c_ant is not NULL:
            del self.c_ant

    def set_model(self, coord):
        cdef vector[vector[float]] c_coord = coord
        self.c_ant.set_model_coordinates(c_coord)

    def set_base(self, coord):
        cdef vector[float] c_coord = coord
        self.c_ant.set_base_coordinates(c_coord)

    def set_orientation(self, float val):
        self.c_ant.set_orientation(val)

    def get_model(self):
        el_list = self.c_ant.get_model_coordinates()
        el_arr = np.asarray(el_list)
        return el_arr

    def get_elements(self):
        el_list = self.c_ant.get_elements_coordinates()
        el_arr = np.asarray(el_list)
        return el_arr

    def get_base(self):
        return self.c_ant.get_base_coordinates()

    def get_stages(self):
        return self.c_ant.get_elements_on_stages()

    def get_orientation(self):
        return self.c_ant.get_orientation()

    def get_channels_total(self):
        return self.c_ant.get_channels_total()

    def show(self):
        """Antenna visualization via plotly
        """
        import plotly.offline as plt
        import plotly.graph_objs as go

        cart_coord = np.asarray(self.c_ant.get_elements_coordinates())
        color = 'green'
        elements = go.Scatter3d(x=cart_coord[0, :],
                                y=cart_coord[1, :],
                                z=cart_coord[2, :],
                                mode='markers',
                                marker=go.Marker(symbol='square', color=color))

        data = [elements]

        for i in range(self.c_ant.get_channels_total()):
            line = np.zeros((3, 2))
            line[0, 0] = 0
            line[1, 0] = 0
            line[2, 0] = cart_coord[2, i]
            line[0, 1] = cart_coord[0, i]
            line[1, 1] = cart_coord[1, i]
            line[2, 1] = cart_coord[2, i]
            data.append(go.Scatter3d(x=line[0, :],
                                     y=line[1, :],
                                     z=line[2, :],
                                     mode='lines',
                                     line=go.Line(color=color, width=6)))

        data.append(go.Scatter3d(x=np.array([0, 0]),
                                 y=np.array([0, 0]),
                                 z=np.array([0, max(cart_coord[2, :])]),
                                 mode='lines',
                                 line=go.Line(color=color, width=10)))

        plt.plot(data)

cdef class PySpecFrame:
    cdef SpecFrame *c_frame

    def __cinit__(self):
        self.c_frame = new SpecFrame()

    def initialize(self, signals, f0, fs, f_res):
        self.c_frame.set_carrier(f0)
        self.c_frame.set_sampling_frequency(fs)
        self.c_frame.set_frequency_resolution(f_res)
        sig_list = signals.tolist()
        self.c_frame.set_data(sig_list)

    def __dealoc__(self):
        if self.c_frame is not NULL:
            del self.c_frame
cdef object PyAntenna_factory(Antenna *ptr):
    cdef PyAntenna py_ant = PyAntenna()
    py_ant.c_ant = ptr
    return py_ant

# cdef public vector[float] pel_grid(Antenna *ant, double f0, double df, double fs, double f_res):
#     py_ant = PyAntenna_factory(ant)
#     beam_width = beam_width_est(antenna=py_ant, f0=f0, df=df, fs=fs, f_res=f_res)
#     retu