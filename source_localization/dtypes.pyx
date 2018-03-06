from dtypes cimport *


from libcpp.vector cimport vector
from libcpp.string cimport  string
import numpy as np
cimport numpy as np
from libc.stdlib cimport malloc, free
from cython.operator cimport dereference, address
import configparser
import plotly.offline as plt
import plotly.graph_objs as go


np.import_array()


cdef class PyAntenna:
    def __cinit__(self,):
        self.c_ant = NULL

    def __reduce_cython__(self):
        pass

    def load(self, filename):
        cdef string c_str = filename.encode("utf-8")

        if self.c_ant is not NULL:
            del self.c_ant

        self.c_ant = new Antenna(c_str)

    def __dealoc__(self):
        if self.c_ant is not NULL:
            del self.c_ant

    def set_name(self, name):
        cdef string c_str = name.encode("utf-8")

        if (self.c_ant is not NULL):
            self.c_ant.set_name(c_str)

    def set_model(self, coord):
        """ Load model coordinates.
        Format is x = [[x0, x1, x2, ...],
                       [y0, y1, y2, ...],
                       [z0, z1, z2, ...]]
        """

        if self.c_ant is NULL:
            self.c_ant = new Antenna()

        cdef vector[vector[float]] c_coord = coord
        self.c_ant.set_model(c_coord)

    def set_base(self, coord):

        cdef vector[float] c_coord1d
        cdef vector[vector[float]] c_coord2d

        if self.c_ant is NULL:
            self.c_ant = new Antenna()

        if len(coord.shape) is 1:
            c_coord1d = coord
            self.c_ant.set_base(c_coord1d)

        if len(coord.shape) is 2:
            c_coord2d = coord
            self.c_ant.set_base(c_coord2d)



    def set_phase_center(self, coord):

        cdef vector[float] c_coord1d
        cdef vector[vector[float]] c_coord2d

        if self.c_ant is NULL:
            self.c_ant = new Antenna()

        if len(coord.shape) is 1:
            c_coord1d = coord
            self.c_ant.set_phase_center(c_coord1d)

        if len(coord.shape) is 2:
            c_coord2d = coord
            self.c_ant.set_phase_center(c_coord2d)

    def set_orientation(self, float val):

        if self.c_ant is NULL:
            self.c_ant = new Antenna()

        self.c_ant.set_orientation(val)

    def get_model(self):

        if self.c_ant is NULL:
            return None

        el_list = self.c_ant.get_model()
        el_arr = np.asarray(el_list)

        if el_arr.shape[0] is 1:
            el_arr.transpose()

        return el_arr

    def get_elements(self):

        if self.c_ant is NULL:
            return None

        el_list = self.c_ant.get_elements()
        el_arr = np.asarray(el_list)

        if el_arr.shape[0] is 1:
            el_arr.transpose()

        return el_arr

    def get_base(self):

        if self.c_ant is NULL:
            return None

        return np.asarray( self.c_ant.get_base() )

    def get_phase_center(self):

        if self.c_ant is NULL:
            return None

        return np.asarray( self.c_ant.get_phase_center() )

    def get_stages(self):

        if self.c_ant is NULL:
            return None

        return self.c_ant.get_elements_on_stages()

    def get_orientation(self):

        if self.c_ant is NULL:
            return None

        return self.c_ant.get_orientation()

    def get_channels_total(self):

        if self.c_ant is NULL:
            return None

        return self.c_ant.get_channels_total()

    def show(self):
        """Antenna visualization via plotly
        """
        data = self.get_trace()
        plt.plot(data)

    def get_trace(self):
        """ returns plotly trace for plotting"""
        if self.c_ant is NULL:
            return None

        cart_coord = np.asarray(self.c_ant.get_elements())

        # Преобразуем вектор в матрицу
        if len(cart_coord) is 1:
            cart_coord = [cart_coord]

        color = 'green'
        reference_element = go.Scatter3d(x=cart_coord[0, 0:1],
                                y=cart_coord[1, 0:1],
                                z=cart_coord[2, 0:1],
                                mode='markers',
                                marker=go.Marker(symbol='square', color='red'))

        data = [reference_element]

        data.append(go.Scatter3d(x=cart_coord[0, 1:],
                                y=cart_coord[1, 1:],
                                z=cart_coord[2, 1:],
                                mode='markers',
                                marker=go.Marker(symbol='square', color=color)))

        base = self.get_base()

        for i in range(self.c_ant.get_channels_total()):
            line = np.zeros((3, 2))
            line[0, 0] = base[0]
            line[1, 0] = base[1]
            line[2, 0] = cart_coord[2, i]
            line[0, 1] = cart_coord[0, i]
            line[1, 1] = cart_coord[1, i]
            line[2, 1] = cart_coord[2, i]
            data.append(go.Scatter3d(x=line[0, :],
                                     y=line[1, :],
                                     z=line[2, :],
                                     mode='lines',
                                     line=go.Line(color=color, width=6)))

        data.append(go.Scatter3d(x=np.array([base[0], base[0]]),
                                 y=np.array([base[1], base[1]]),
                                 z=np.array([base[2], max(cart_coord[2, :])]),
                                 mode='lines',
                                 line=go.Line(color=color, width=10)))
        return data


    def save(self, filename):
        """Save antenna parameters
        """

        config = configparser.ConfigParser()
        coordinates = self.get_elements()
        channels_total = coordinates.shape[1]
        config["DEFAULT"]['channels number'] = str(channels_total)


        for i in range(channels_total):
            config['DEFAULT']['x' + str(i)] = str(coordinates[0, i])
            config['DEFAULT']['y' + str(i)] = str(coordinates[1, i])
            config['DEFAULT']['z' + str(i)] = str(coordinates[2, i])

        with open(filename, 'w') as configfile:
            config.write(configfile)

cdef object py_antenna_factory(Antenna *ptr):
    cdef PyAntenna py_ant = PyAntenna()
    py_ant.c_ant = ptr
    return py_ant

cdef Antenna *antenna_factory(PyAntenna ant):
    cdef Antenna * a = ant.c_ant
    return a


class AntennaSystem:

    def __init__(self):
        self.antennas = list()

    def load(self, filename):
        """ load antenna system parameters
        """

        antenna_system_config = configparser.ConfigParser()
        antenna_system_config.read(filename)

        post_total = int(antenna_system_config['LandingSystem']['post_total'])
        path_to_dir = ""
        for i in range(len(filename)-1, 0, -1):
            if filename[i] == '/':
                path_to_dir = filename[:i+1]
                break


        self.antennas = list()


        for i in range(post_total):
            ant_model = antenna_system_config['Post' + str(i)]['antenna']
            base = np.zeros(3)
            base[0] = float(antenna_system_config['Post' + str(i)]['x'])
            base[1] = float(antenna_system_config['Post' + str(i)]['y'])
            base[2] = float(antenna_system_config['Post' + str(i)]['z'])
            orientation = float(antenna_system_config['Post' + str(i)]['orientation'])

            antenna = PyAntenna()
            antenna.load(path_to_dir + ant_model + '.ini')
            antenna.set_base(base)
            antenna.set_orientation(orientation)
            antenna.set_name(ant_model)
            self.antennas.append(antenna)

    def save(self, filename):
        """ save antenna system parameters
        """
        antenna_system_config = configparser.ConfigParser()
        post_total = len(self.antennas)
        antenna_system_config['LandingSystem']['post_total'] = str(post_total)

        for i in range(post_total):
            base = self.antennas[i].get_base()
            antenna_system_config['Post' + str(i)]['x'] = base[0]
            antenna_system_config['Post' + str(i)]['y'] = base[1]
            antenna_system_config['Post' + str(i)]['z'] = base[2]
            antenna_system_config['Post' + str(i)]['orientation'] = self.antennas[i].get_orientation()
            antenna_system_config['Post' + str(i)]['antenna'] = self.antennas[i].get_name()

        with open(filename, 'w') as configfile:
            antenna_system_config.write(configfile)


    def append(self, antenna):
        self.antennas.append(antenna)

    def __getitem__(self, item):
        return self.antennas[item]

    def __len__(self):
        return len(self.antennas)

    def pop(self, item):
        return self.antennas.pop(item)

    def clear(self):
        self.antennas.clear()

    def show(self):
       """Antenna System visualization via plotly
        """

       data = []
       for i in range(len(self.antennas)):
           new_data = self.antennas[i].get_trace()
           for j in range(len(new_data)):
               data.append(new_data[j])

       plt.plot(data)


cdef class PySpecFrame:
    def __cinit__(self, channels_total = 0, samp_per_ch = 0):
        self.c_frame = new SpecFrame(channels_total, samp_per_ch)

    def __dealoc__(self):
        if self.c_frame is not NULL:
            del self.c_frame

    def initialize(self, **kw):
        cdef complex2d c_data

        # if "f0" in kw:
        #     self.c_frame.set_central_frequency(kw['f0'])
        #
        # if "fs" in kw:
        #     self.c_frame.set_sampling_frequency(kw['fs'])
        #
        # if 'f_res' in kw:
        #     self.c_frame.set_frequency_resolution(kw['f_res'])
        #
        # if 'sig' in kw:
        #     # sig_list = kw['sig'].tolist()
        #     sig = kw['sig']
        #     self.convert_python_data(sig)


    # cdef void set_python_data(self, np.ndarray [np.complex128_t, ndim=2] data):
    #
    #     cdef int channels_total = len(data)
    #     cdef int samp_per_ch = len(data[0])
    #     cdef complex2d c_data
    #
    #     c_data = <complex2d > malloc( channels_total * sizeof(complex1d))
    #     for ch in range(channels_total):
    #         c_data[ch] = <complex1d> malloc(samp_per_ch * sizeof(complex))
    #
    #
    #     for ch in range(channels_total):
    #         for s in range(samp_per_ch):
    #             c_data[ch][s] = data[ch, s]
    #
    #     self.c_frame.set_data(address(c_data), channels_total, samp_per_ch)

    cdef np.ndarray[np.complex128_t, ndim=2] get_python_data(self):
        cdef int channels_total =  self.c_frame.get_channels_total()
        cdef int samp_per_channel = self.c_frame.get_length()
        cdef complex2d data = self.c_frame.get_data()

        output = np.zeros(shape=(channels_total, samp_per_channel), dtype=np.complex128)
        for i in range(channels_total):
            for j in range(samp_per_channel):
                output[i, j] = data[i][j]

        return output


    def get_data(self):
        return self.get_python_data()

    def get_central_frequency(self):
        return self.c_frame.get_central_frequency()

    def get_sampling_frequency(self):
        return self.c_frame.get_sampling_frequency()

    def get_frequency_resolution(self):
        return self.c_frame.get_frequency_resolution()


cdef SpecFrame *spec_frame_factory(PySpecFrame frame):
    cdef SpecFrame *f = frame.c_frame
    return f

cdef PySpecFrame py_spec_frame_factory(SpecFrame *c_frame):
    cdef PySpecFrame frame = PySpecFrame()
    frame.c_frame = c_frame
    return  frame

cdef class PyFrameReader:
    def __cinit__(self):
        self.c_frame = new SpecFrame(0,0)

    def open(self, filename):
        cdef string c_str = filename.encode("utf-8")
        self.c_reader.open(c_str)

    def read(self):
         self.c_reader.read(self.c_frame)
         return py_spec_frame_factory(self.c_frame)

    def close(self):
        self.c_reader.close()