from ctypes import cdll
from numpy.ctypeslib import ndpointer
import ctypes
import numpy as np
lib = cdll.LoadLibrary('/home/dima/Projects/source_localization/build/libestimators.so')
double_pp = ndpointer(dtype=np.uintp, ndim=1, flags='C')


class Pel_Lh(object):

    def __init__(self, signal, antenna, f0):

        lib.Pel_Lh_new.argtypes= [double_pp, double_pp, ctypes.c_int, double_pp, ctypes.c_int, ctypes.c_double]
        lib.Pel_Lh_new.restype = ctypes.c_void_p

        lib.Pel_Lh_calc.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double]
        lib.Pel_Lh_calc.restype = ctypes.c_double

        amp = np.abs(signal)
        phase = np.angle(signal)
        sig_length = signal.shape[1]
        elements_total = signal.shape[0]
        antenna_coordinates = antenna.to_cartesian()

        amp_c = (amp.__array_interface__['data'][0] + np.arange(amp.shape[0]) * amp.strides[0]).astype(np.uintp)
        phase_c = (phase.__array_interface__['data'][0] + np.arange(phase.shape[0]) * phase.strides[0]).astype(np.uintp)
        antenna_coordinates_c = (antenna_coordinates.__array_interface__['data'][0] + np.arange(antenna_coordinates.shape[0]) * antenna_coordinates.strides[0]).astype(np.uintp)

        self.obj = lib.Pel_Lh_new(amp_c, phase_c, sig_length, antenna_coordinates_c, elements_total, f0)

    def calc(self, pel):
        return lib.Pel_Lh_calc(self.obj, ctypes.c_double(pel[0]), ctypes.c_double(pel[1]))


