cimport source_localization_headers as src


from libcpp.vector cimport vector
from libcpp.string cimport  string
from libcpp cimport bool
import numpy as np
cimport numpy as np
from libc.stdlib cimport malloc, free
from cython.operator cimport dereference, address
import configparser
import plotly.offline as plt
import plotly.graph_objs as go
import copy

np.import_array()

cdef class Antenna:
    cdef src.Antenna *c_ant
    def __cinit__(self,):
        self.c_ant = NULL

    def __reduce_cython__(self):
        pass

    def load(self, filename):
        cdef string c_str = filename.encode("utf-8")

        if self.c_ant is not NULL:
            del self.c_ant

        self.c_ant = new src.Antenna(c_str)

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
            self.c_ant = new src.Antenna()

        cdef vector[vector[float]] c_coord = coord
        self.c_ant.set_model(c_coord)

    def set_base(self, coord):

        cdef vector[float] c_coord1d
        cdef vector[vector[float]] c_coord2d

        if self.c_ant is NULL:
            self.c_ant = new src.Antenna()

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
            self.c_ant = new src.Antenna()

        if len(coord.shape) is 1:
            c_coord1d = coord
            self.c_ant.set_phase_center(c_coord1d)

        if len(coord.shape) is 2:
            c_coord2d = coord
            self.c_ant.set_phase_center(c_coord2d)

    def set_orientation(self, float val):

        if self.c_ant is NULL:
            self.c_ant = new src.Antenna()

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

cdef object py_antenna_factory(src.Antenna *ptr):
    cdef Antenna py_ant = Antenna()
    py_ant.c_ant = ptr
    return py_ant

cdef src.Antenna *antenna_factory(Antenna ant):
    cdef src.Antenna * a = ant.c_ant
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

            antenna = Antenna()
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
        self.antennas._clear()

    def show(self):
       """Antenna System visualization via plotly
        """

       data = []
       for i in range(len(self.antennas)):
           new_data = self.antennas[i].get_trace()
           for j in range(len(new_data)):
               data.append(new_data[j])

       plt.plot(data)


ctypedef fused fused_2d_array:
    float complex [:,:]
    double complex [:,:]
    float [:,:]
    double [:,:]


cdef class SpecFrameWriter:
    cdef src.SpecFrameWriter c_writer

    cdef src.SpecFrame *_get_c_frame(self, SpecFrame frame):
        cdef src.SpecFrame *f = frame.c_frame
        return f

    def _write_sampling_frequency(self, SpecFrame frame, double fs):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_sampling_frequency(c_frame, fs)

    def _write_central_frequency(self, SpecFrame frame, double f0):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_central_frequency(c_frame, f0)

    def _write_frequency_resolution(self, SpecFrame frame, double f_res):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_frequency_resolution(c_frame, f_res)

    def _write_post_id(self, SpecFrame frame, double post_id):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_post_id(c_frame, post_id)

    def _write_serial(self, SpecFrame frame, int serial):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_serial(c_frame, serial)

    def _write_bound(self, SpecFrame frame, int bound):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.write_bound(c_frame, bound)

    def _write_data(self, SpecFrame frame, fused_2d_array data):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        cdef int channels_total = data.shape[0]
        cdef int samp_per_channel = data.shape[1]
        cdef src.Complex2d c_data
        self.c_writer.resize(c_frame, channels_total, samp_per_channel)
        c_data = self.c_writer.get_mutable_data(c_frame)

        for ch in range(channels_total):
            for s in range(samp_per_channel):
                c_data[ch][s] = data[ch, s]


    cdef src.Complex2d _get_mutable_data(self, src.SpecFrame *frame):
        return self.c_writer.get_mutable_data(frame)

    def _clear(self, SpecFrame frame):
        cdef src.SpecFrame *c_frame = self._get_c_frame(frame)
        self.c_writer.clear(c_frame)

cdef class SpecFrame(SpecFrameWriter):
    cdef src.SpecFrame *c_frame

    def __cinit__(self, **kw):

        self.c_frame = new src.SpecFrame(0, 0)

        if "f0" in kw:
            self._write_central_frequency(self, kw['f0'])

        if "fs" in kw:
            self._write_sampling_frequency(self, kw['fs'])

        if 'f_res' in kw:
            self._write_frequency_resolution(self, kw['f_res'])

        if 'post_id' in kw:
            self._write_post_id(self, kw['post_id'])

        if 'serial' in kw:
            self._write_serial(self, kw['serial'])

        if 'sig' in kw:
            sig = kw['sig']
            self._write_data(self, sig)


    def __dealoc__(self):
        if self.c_frame is not NULL:
            del self.c_frame


    cdef np.ndarray[np.complex128_t, ndim=2] get_python_data(self):
        cdef int channels_total =  self.c_frame.get_channels_total()
        cdef int samp_per_channel = self.c_frame.get_length()
        cdef src.Complex2d data = self._get_mutable_data(self.c_frame)

        output = np.zeros(shape=(channels_total, samp_per_channel), dtype=np.complex128)
        for i in range(channels_total):
            for j in range(samp_per_channel):
                output[i, j] = data[i][j]

        return output


    def get_data(self):
        return self.get_python_data()

    def set_data(self, data):
        self._write_data(self, data)

    def get_central_frequency(self):
        return self.c_frame.get_central_frequency()

    def set_central_frequency(self, f0):
        self._write_central_frequency(self, f0)

    def get_sampling_frequency(self):
        return self.c_frame.get_sampling_frequency()

    def set_sampling_frequency(self, fs):
        self._write_sampling_frequency(self, fs)

    def get_frequency_resolution(self):
        return self.c_frame.get_frequency_resolution()

    def set_frequency_resolution(self, f_res):
        self._write_frequency_resolution(self, f_res)

    def get_post_id(self):
        return self.c_frame.get_post_id()

    def set_post_id(self, post_id):
        self._write_post_id(self, post_id)

    def get_serial(self):
        return self.c_frame.get_serial()

    def set_serial(self, serial):
        return self._write_serial(self, serial)

    def _clear(self):
        super()._clear(self)


cdef src.SpecFrame *spec_frame_to_c(SpecFrame frame):
    cdef src.SpecFrame *f = frame.c_frame
    return f

cdef SpecFrame c_to_spec_frame(src.SpecFrame *c_frame):
    cdef SpecFrame frame = SpecFrame()
    frame.c_frame = c_frame
    return  frame

cdef class SpecFrameSaver:
    cdef src.SpecFrameSaver *c_saver

    def __cinit__(self, int post_id = -1):
        self.c_saver = new src.SpecFrameSaver(post_id)

    def open(self, filename):
        cdef string c_str = filename.encode("utf-8")
        return self.c_saver.open(c_str)

    def save_title(self, start_period):
        self.c_saver.save_title(start_period)

    def save(self, frame):
        self.c_saver.save(spec_frame_to_c(frame))

    def close(self):
        self.c_saver.close()


cdef class SpecFrameLoader:
    cdef src.SpecFrameLoader *c_loader

    def __cinit__(self, int post_id = -1):
        self.c_loader = new src.SpecFrameLoader(post_id)

    def open(self, filename):
        cdef string c_str = filename.encode("utf-8")
        return self.c_loader.open(c_str)

    def read_title(self):
        return self.c_loader.read_title()

    def load(self, frame):
        self.c_loader.load(spec_frame_to_c(frame))

    def close(self):
        self.c_loader.close()
#
# class SpecFrameFactory(SpecFrameWriter):
#
#     def __init__(self):
#         self.frame = SpecFrame()
#
#     def write_sampling_frequency(self, double fs):
#         super().write_sampling_frequency(self.frame, fs)
#
#     def write_central_frequency(self, double f0):
#         super().write_central_frequency(self.frame, f0)
#
#     def write_frequency_resolution(self, double f_res):
#         super().write_frequency_resolution(self.frame, f_res)
#
#     def write_post_id(self, double post_id):
#         super().write_post_id(self.frame, post_id)
#
#     def write_serial(self, int serial):
#         super().write_serial(self.frame, serial)
#
#     def write_bound(self, int bound):
#         super().write_bound(self.frame, bound)
#
#     def write_data(self,  np.ndarray[:,:] data):
#         super().write_data(self.frame, data)
#
#     def clear(self):
#         super().clear(self.frame)
#
#     def get_frame(self):
#         print("Start deep copying")
#         frame = copy.deepcopy(self.frame)
#         print("Stop depp copying")
#         return  self.frame



cdef class LhPel:
    cdef src.LhPel *lh
    def __cinit__(self, Antenna ant, SpecFrame frame, samp_start, samp_stop, verbose = False):
        cdef src.Antenna *a = antenna_factory(ant)
        cdef src.SpecFrame *s = spec_frame_to_c(frame)
        self.lh = new src.LhPel(a, s, samp_start, samp_stop, verbose)

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

cdef class Pelengator:
    cdef src.Pelengator *pelengator
    def __init__(self, Antenna py_ant, frequency_resolution, sampling_frequency):
        cdef src.Antenna *c_ant = antenna_factory(py_ant)
        self.pelengator = new src.Pelengator(c_ant[0], frequency_resolution, sampling_frequency)

    def __dealoc__(self):
        if self.pelengator is not NULL:
            del self.pelengator

    def set_signal_parameters(self, double f0, int samp_start, int samp_stop):
        self.pelengator.set_signal_param(f0, samp_start, samp_stop)

    def estimate(self, SpecFrame py_frame):
        cdef src.SpecFrame *c_frame = spec_frame_to_c(py_frame)
        cdef src.Estimation estimation = self.pelengator.estimate(c_frame)
        return np.asarray(estimation)

    def turn_on_interpolation(self, bool turn_on):
        self.pelengator.turn_on_interpolation(turn_on)

    def set_verbose(self, arg):
        self.pelengator.set_verbose(arg)


cdef class Triangulator:
    cdef src.Triangulator c_triangulator
    def __cinit__(self):
        self.c_triangulator = src.Triangulator()

    def estimate(self, pelengs):
        cdef vector[src.Peleng] c_pelengs
        cdef vector[vector[float]] target

        for i in range(len(pelengs)):
            c_pelengs.push_back(peleng_factory(pelengs[i]))

        target = self.c_triangulator.estimate(c_pelengs)
        return np.asarray(target)

cdef class Peleng:
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


cdef src.Peleng peleng_factory(Peleng pel):
    cdef src.Peleng c_pel = src.Peleng()
    c_pel.azimuth = pel.azimuth
    c_pel.elevation = pel.elevation
    c_pel.phase_center = pel.phase_center.tolist()
    c_pel.time = pel.time
    return c_pel

cdef Peleng py_peleng_factory(src.Peleng c_pel):
    cdef Peleng pel = Peleng()
    pel.azimuth = c_pel.azimuth
    pel.elevation = c_pel.elevation
    pel.phase_center = c_pel.phase_center
    pel.probability = c_pel.probability
    pel.time = c_pel.time


cdef class PseudoPhaseEstimator:
    cdef src.PseudoPhaseEstimator c_pp_estimator

    def set_signal_parameters(self, ch_ref, samp_start, samp_stop):
        self.c_pp_estimator.set_signal_parameters(ch_ref, samp_start, samp_stop)


    def estimate(self, frame):
        cdef src.SpecFrame *c_frame = spec_frame_to_c(frame)
        estimation = self.c_pp_estimator.estimate(c_frame)
        return np.asarray(estimation)


cdef class DelayEstimator:
    cdef src.DelayEstimator c_d_estimator

    def set_signal_parameters(self, ch_ref, samp_start, samp_stop):
        self.c_d_estimator.set_signal_parameters(ch_ref, samp_start, samp_stop)


    def estimate(self, frame):
        cdef src.SpecFrame * c_frame = spec_frame_to_c(frame)
        estimation = self.c_d_estimator.estimate(c_frame)
        return np.asarray(estimation)