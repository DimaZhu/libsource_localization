""" This module provides various signal for spatial net of antenna sensors.

Currently it can generate:
    base band white noise
"""

__version__ = '0.1'
__author__ = 'Dmitry Zhukov'

import numpy as np
import configparser


class Antenna:
    """Class for creating afs.ini files and modeling antennas"""

    base = np.zeros((3, 1))

    def save(self, filename):
        """Save antenna parameters
        """

        config = configparser.ConfigParser()

        config["DEFAULT"]['channels number'] = str(self.channelsTotal);

        for i in range(self.coordinates.shape[1]):
            config['DEFAULT']['alpha' + str(i)] = str(self.coordinates[0, i])

        for i in range(self.coordinates.shape[1]):
            config['DEFAULT']['r' + str(i)] = str(self.coordinates[1, i])

        for i in range(self.coordinates.shape[1]):
            config['DEFAULT']['z' + str(i)] = str(self.coordinates[2, i])

        with open(filename, 'w') as configfile:
            config.write(configfile)

    def load(self, filename):
        """ Load antenna parameters
        """

        config = configparser.ConfigParser()

        config.read(filename)
        self.channelsTotal = int(config['DEFAULT']['channels number'])
        self.coordinates = np.zeros((3, self.channelsTotal))

        for i in range(self.channelsTotal):
            self.coordinates[0, i] = float(config['DEFAULT']['alpha' + str(i)])
            self.coordinates[1, i] = float(config['DEFAULT']['r' + str(i)])
            self.coordinates[2, i] = float(config['DEFAULT']['z' + str(i)])

    def calculate(self, radius, height, phi, step):
        """Calculate antennas coordinates
        """
        self.channelsTotal = 0
        for i in range(height.size):
            self.channelsTotal += int(np.abs(360 / step[i]))

        element_counter = 0
        self.coordinates = np.zeros((3, self.channelsTotal))
        for i in range(height.size):
            circle_counter = 0
            while np.abs(circle_counter * step[i]) < 360:
                self.coordinates[0, element_counter] = (phi[i] + circle_counter * step[i]) % 360
                self.coordinates[1, element_counter] = radius[i]
                self.coordinates[2, element_counter] = height[i]
                circle_counter += 1
                element_counter += 1

    def to_cartesian(self):
        """Antenna transform antenna coordinates to cartesian"""

        cart = np.zeros(self.coordinates.shape)
        cart[0, :] = self.coordinates[1, :] * np.cos(np.radians(self.coordinates[0, :])) + self.base[0, 0]
        cart[1, :] = self.coordinates[1, :] * np.sin(np.radians(self.coordinates[0, :])) + self.base[1, 0]
        cart[2, :] = self.coordinates[2, :] + self.base[2, 0]
        return cart

    def rotate(self, angle):
        """Rotate antenna"""
        self.coordinates[0, :] = (self.coordinates[0, :] - angle) % 360

    def show(self):
        """Antenna visualization via plotly
        """
        import plotly.offline as plt
        import plotly.graph_objs as go

        cart_coord = self.to_cartesian()
        color = 'green'
        elements = go.Scatter3d(x=cart_coord[0, :],
                                y=cart_coord[1, :],
                                z=cart_coord[2, :],
                                mode='markers',
                                marker=go.Marker(symbol='square', color=color))

        data = [elements]

        for i in range(self.channelsTotal):
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


def wgn_baseband(antenna, target, length, fs, **keywords):
    from scipy.stats import norm
    import scipy.fftpack as fft

    if len(target.shape) is 1:
        target = np.array([target])
        target = target.transpose()

    if target.shape[1] is not 1 and target.shape[0] is 1:
        target = target.transpose()

    N = antenna.get_channels_total()
    A = antenna.get_elements().transpose()
    T = np.matmul(target, np.ones((1, N)))
    r = np.sqrt(np.matmul(np.ones((1, 3)), (A - T) ** 2))
    tau = r.T / 3e8

    if 'print_delay' in keywords:
        print(tau)

    s0 = norm.rvs(size=length) + 1j * norm.rvs(size=length)
    s0 = np.array([s0.T])
    s = np.matmul(np.ones((N, 1)), s0)

    s_f = delay(s, tau, fs)

    if 'snr' in keywords:
        scale = 1 / (10**(keywords['snr'] / 20))
        s = norm.rvs(size=(N, length), scale=scale) + 1j*norm.rvs(size=(N, length), scale=scale)

    return s_f


def delay(signal, tau, fs):
    """ delays signal by tau seconds

        signal can be 2d Matrix, where 1 axis is channels and tau is a vector with (j,1) element
        corresponds to j channel.
    """
    import scipy.fftpack as fft

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
    return output_f

