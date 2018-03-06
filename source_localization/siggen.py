""" This module provides various signal for spatial net of antenna sensors.

Currently it can generate:
    base band white noise
"""

__version__ = '0.1'
__author__ = 'Dmitry Zhukov'

import numpy as np
from scipy.stats import norm
import scipy.fftpack as fft
import dtypes


def wgn_baseband(antenna, target, length, fs, **keywords):
    """function wgn_baseband generates white gaussian noise on
    zero carrier.
    Target can be [x, y, z] or [[x], [y], [z]]
    antenna can be dtypes.PyAntenna() object or dtypes.AntennaSystem() object.
    if there is more than one antenna than output is list of 2D nparray.
    in 2D nparray the first dimension is channels and the second is signals.
    if there is one antenna output is 2D nparray
    """

    if len(target.shape) is 1:
        target = np.array([target])
        target = target.transpose()

    if target.shape[1] is not 1 and target.shape[0] is 1:
        target = target.transpose()

    if type(antenna) is dtypes.PyAntenna:
        antenna = dtypes.AntennaSystem().append(antenna)

    output = []
    for ant_counter in range(len(antenna)):
        channels_total = antenna.get_channels_total()
        elements = antenna.get_elements()
        target_matrix = np.matmul(target, np.ones((1, channels_total)))
        r = np.sqrt(np.matmul(np.ones((1, 3)), (elements - target_matrix) ** 2))
        tau = r.T / 3e8

        s0 = norm.rvs(size=length) + 1j * norm.rvs(size=length)
        s0 = np.array([s0.T])
        s = np.matmul(np.ones((channels_total, 1)), s0)

        s_f = delay(s, tau, fs)

        if 'snr' in keywords:
            scale = 1 / (10**(keywords['snr'] / 20))
            s_f += norm.rvs(size=(channels_total, length), scale=scale) + 1j*norm.rvs(size=(channels_total, length), scale=scale)

        output.append(s_f)

    if 'get_tau' in keywords:
        if keywords['get_tau'] and len(output) is 1:
            return output[0], tau

        if keywords['get_tau'] and len(output) is not 1:
            return output, tau
    else:
        if len(output) is 1:
            return output[0]

        else:
            return output


def delay(signal, tau, fs):
    """ delays signal by tau seconds

        signal can be 2d Matrix, where 1 axis is channels and tau is a vector with (j,1) element
        corresponds to j channel.
    """


    assert signal.shape[0] is tau.shape[0]

    length = signal.shape[1]
    k1 = np.array([np.linspace(0, length/2, length/2 + 1)])
    k2 = np.array([np.linspace(-length/2, -1, length/2)])
    k = np.concatenate((k1, k2), 1)
    H = np.exp(- 1j * 2 * np.pi * fs / length * np.matmul(tau, k))

    if not (length % 2):
        ind = length / 2
        H[:, int(ind)] = 1
        H = np.delete(H, ind + 1, 1)

    signal_f = fft.fft(signal, axis=1)
    output_f = H * signal_f

    print(output_f.shape)
    return output_f

