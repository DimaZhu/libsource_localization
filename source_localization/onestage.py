"""   This module provides onestage algorithms to estimate
    position of the electromagnetic source"""

__version__ = '0.0'
__author__ = 'Dmitry Zhukov'


import numpy as np
import scipy as sp
import siggen
from lh import PyLh_Pel
from dtypes import PySpecFrame



def peleng(antenna, target):
    """ calculate pelengs from antenna to target"""
    import numpy as np
    dr = target - antenna.base
    alpha = np.arctan(dr[1, 0] / dr[0, 0])

    rxy = np.sqrt((target[0, 0] - antenna.base[0, 0])**2 + (target[1, 0] - antenna.base[1, 0])**2)
    thetta = np.angle(rxy + 1j * (target[2, 0] - antenna.base[2, 0]))
    peleng = np.array([alpha, thetta])
    return peleng


def pel_likelihood(signal, ant_coord, peleng, f0):
    likelihood = 0
    wave_length = 3e8 / f0
    freq_2pi_c = 2 * np.pi / wave_length
    r = np.array([np.cos(peleng[1]) * np.cos(peleng[0]), np.cos(peleng[1]) * np.sin(peleng[0]), np.sin(peleng[1])])
    l_max = ant_coord.shape[1] - 1
    m_max = l_max + 1
    k_max = signal.shape[1]
    ant_delta = np.zeros(3)

    for l in range(l_max):
        for m in range(l + 1, m_max):
            sum_sin = 0
            sum_cos = 0
            sum_amp = 0
            for k in range(k_max):
                amp = np.abs(signal[m, k]) * np.abs(signal[l, k])
                delta_phz = np.angle(signal[l, k]) - np.angle(signal[m, k]) # !NOTE! похоже здесь ошибка в теории!
                sum_sin += amp * np.sin(delta_phz)
                sum_cos += amp * np.cos(delta_phz)
                sum_amp += signal[m, k] * np.conj(signal[l, k])

            ant_delta[0] = ant_coord[0, m] - ant_coord[0, l]
            ant_delta[1] = ant_coord[1, m] - ant_coord[1, l]
            ant_delta[2] = ant_coord[2, m] - ant_coord[2, l]
            coord_delta = ant_delta[0] * r[0] + ant_delta[1] * r[1] + ant_delta[2] * r[2]
            pseudophase = np.angle(sum_cos + 1j * sum_sin)
            full_phase = freq_2pi_c * coord_delta + pseudophase
            likelihood += np.abs(sum_amp) * np.cos(full_phase)

    return likelihood


def arp(antenna, f0, df, fs, f_res, size, **keywords):
    """Calculates antenna radiation pattern

        antenna - object of class Antenna
        peleng - target direction
        f0 - carrier
        snr - signal to noise ratio in decibels"""

    R = 1000
    if 'peleng' in keywords:
        target = np.array([[R*np.cos(keywords['peleng'][1])*np.cos(keywords['peleng'][0])],
                           [R*np.cos(keywords['peleng'][1])*np.sin(keywords['peleng'][0])],
                           [R*np.sin(keywords['peleng'][1])]])
    else:
        target = np.array([[-R], [0], [0]])

    N = int(fs/f_res)
    n_stop = int(np.ceil((f0 + df/2) / f_res))
    n_start = int(np.ceil((f0 - df/2) / f_res))

    if n_stop == n_start:
        n_stop +=1

    if 'snr' in keywords:
        s = siggen.wgn_baseband(antenna, target, N, fs, snr=keywords['snr'])
    else:
        s = siggen.wgn_baseband(antenna, target, N, fs)

    if n_stop > s.shape[1] or n_stop > s.shape[1] / 2:
        raise RuntimeError("signal band is more than fs / 2 ")

    alpha_line = np.linspace(0, 2 * np.pi, size[1], endpoint=False)
    betta_line = np.linspace(-np.pi / 2, np.pi / 2, size[0])
    peleng = meshgrid(alpha_line, betta_line)

    likelihood = np.zeros(size)

    frame = PySpecFrame()
    frame.set_data(s[:, n_start:n_stop])
    frame.set_carrier(f0)
    lh = PyLh_Pel(antenna, frame)
    for i in range(size[0]):
        for j in range(size[1]):
            likelihood[i, j] = -lh.calc(peleng[:, i, j])

    return likelihood

def meshgrid(x1, x2):

    if len(x1.shape) is not 1:
        print("Wrong size of x1")
        print(x1.shape)

    if len(x2.shape) is not 1:
        print("Wrong size of x2")
        print(x1.shape)

    x1 = np.array([x1])
    x2 = np.array([x2])
    x2 = x2.transpose()
    x1_mat = np.matmul(np.ones((1, x2.shape[0], 1)), x1)
    x2_mat = np.matmul(x2, np.ones((1, 1, x1.shape[1])))
    grid = np.concatenate((x1_mat, x2_mat), axis=0)

    return grid

def beam_width_est(**kwargs):

    if len(kwargs) != 5 and len(kwargs) != 1:
        print("Not valid number of arguments")
        return

    if len(kwargs) == 5:
        size = (200, 600)
        lh = arp(kwargs['antenna'], kwargs['f0'], kwargs['df'], kwargs['fs'], kwargs['f_res'], size)

    if len(kwargs) == 1:
        lh = kwargs['lh']
        size = lh.shape

    max_ind = np.argmax(lh)
    betta_max = max_ind % lh.shape[0]
    alpha_max = max_ind % lh.shape[1]
    max_val = lh[betta_max, alpha_max]

    # alpha width
    stop = alpha_max
    while lh[betta_max, stop] > max_val/2:
        stop += 1
        if stop == size[1]:  # overlapping
            stop = 0

    stop -= 1

    start = alpha_max
    while lh[betta_max, start] > max_val/2:
        start -= 1

    start += 1

    if start < 0:
        start += size[1] - 1

    if stop >= start:
        alpha_width = (stop - start + 1) * (2 * np.pi) / size[1]
    else:
        alpha_width = (stop - start + size[1]) * (2 * np.pi) / size[1]

    # betta width
    stop = betta_max
    while lh[stop, alpha_max] > max_val/2:
        stop += 1
        if stop == size[1]:  # overlapping
            print("It seems betta is near pi/2. These algorithm doesn't work for such angles")
            return -1

    stop -= 1

    start = betta_max
    while lh[start, alpha_max] > max_val/2:
        start -= 1
        if start < 0:
            print("It seems betta is near -pi/2. These algorithm doesn't work for such angles")
            return -1

    start += 1

    betta_width = (stop - start + 1) * np.pi / size[0]

    return np.array([alpha_width, betta_width])



class PelengEstimator:
    """ This class estimates pelengs"""

    def __init__(self, antenna, f0, df, fs, f_res):
        beam_width = beam_width_est(antenna=antenna, f0=f0, df=df, fs=fs, f_res=f_res)
        self.lh_size = np.array([np.ceil(np.pi / beam_width[1]) * 3, np.ceil(2 * np.pi / beam_width[0]) * 3], np.int)
        self.f0 = f0
        self.antenna = antenna

    def estimate(self, signal):
        # 1) Грубая оценка
        alpha_line = np.array([np.linspace(0, 2 * np.pi, self.lh_size[1], endpoint=False)])
        alpha_mat = np.matmul(np.ones((1, self.lh_size[0], 1)), alpha_line)

        betta_line = np.transpose(np.array([np.linspace(-np.pi / 2, np.pi / 2, self.lh_size[0])]))
        betta_mat = np.matmul(betta_line, np.ones((1, 1, self.lh_size[1])))

        peleng = np.concatenate((alpha_mat, betta_mat), axis=0)
        likelihood = np.zeros(self.lh_size)

        lh_fun = Lh_Pel(signal, self.antenna, self.f0)

        for i in range(self.lh_size[0]):
            for j in range(self.lh_size[1]):
                likelihood[i, j] = -lh_fun.calc(peleng[:, i, j])

        max_ind = np.argmax(likelihood)
        betta_max = max_ind % likelihood.shape[0]
        alpha_max = max_ind % likelihood.shape[1]
        pel_max = np.array([alpha_max, betta_max])

        # 2) Интерполяция

        # 3) Доуточнение
        output = sp.optimize.minimize(lh_fun.calc, pel_max, method='Nelder-Mead')
        return output.x


def crb_phase(antenna, f0, df, f_res, snr, target):
    """ Cramer Rao Lower bound for pelengation algorithm
    NOTE! Assumptions snr in every channel is equal. Structure of every narrow base system is the same """

    c = 3e8
    dim = 2
    t = 1 / f_res
    diag = np.diag(np.ones(antenna.channelsTotal - 1))
    ones = np.ones((antenna.channelsTotal - 1, antenna.channelsTotal - 1))
    wave_length = c / f0
    p_n = 16 * np.pi**2 * t * wave_length**-2 * df * snr**2 / (1 + antenna.channelsTotal * snr)
    phi_mat = antenna.channelsTotal * ones - diag
    b_mat = np.zeros((antenna.channelsTotal - 1, dim))
    ant_coord = antenna.to_cartesian()


    for i in range(1, antenna.channelsTotal):
        dx = ant_coord[0, i] - ant_coord[0, 0]
        dy = ant_coord[1, i] - ant_coord[1, 0]
        dz = ant_coord[2, i] - ant_coord[2, 0]

        b_mat[i - 1, 0] = - dx * np.sin(target[0]) * np.cos(target[1])\
                          + dy * np.cos(target[0]) * np.cos(target[1])
        b_mat[i - 1, 1] = - dx * np.cos(target[0]) * np.sin(target[1])\
                          - dy * np.sin(target[0]) * np.sin(target[1])\
                          + dz * np.cos(target[1])

    mat1 = antenna.channelsTotal * np.matmul(np.matmul(b_mat.transpose(),ones), b_mat)
    mat2 = np.matmul(b_mat.transpose(), b_mat)
    mat3 = np.linalg.pinv(mat1 - mat2)
    # mat1 = np.matmul(b_mat.transpose(), phi_mat)
    # mat2 = np.matmul(mat1, b_mat)
    # mat3 = np.linalg.pinv(mat2)
    covar_mat = p_n**-1 * mat3
    return np.array([covar_mat[0, 0], covar_mat[1, 1]])
