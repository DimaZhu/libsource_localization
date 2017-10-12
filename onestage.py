"""   This module provides onestage algorithms to estimate
    position of the electromagnetic source"""

__version__ = '0.0'
__author__ = 'Dmitry Zhukov'


import numpy as np
from onestage_C import aoa_likelihood_c
import siggen


# def aoa_likelihood(signal, ant_coord, peleng, f0):
#     """Maximum likelihood function for angle of arrival"""
#
#     likelihood = 0
#     wave_length = 3e8 / f0
#     freq_2pi_c = 2 * np.pi / wave_length
#     r = np.array([np.cos(peleng[1]) * np.cos(peleng[0]), np.cos(peleng[1]) * np.sin(peleng[0]), np.sin(peleng[1])])
#     l_max = ant_coord.shape[1] - 1
#     m_max = l_max + 1
#     k_max = signal.shape[1]
#     ant_delta = np.zeros(3, np.double)
#
#     for l in range(l_max):
#         for m in range(l + 1, m_max):
#             sum_sin = 0
#             sum_cos = 0
#             sum_amp = 0
#             for k in range(k_max):
#                 amp = abs(signal[l, k]) * abs(signal[m, k])
#                 delta_phz = np.angle(signal[m, k]) - np.angle(signal[l, k])
#                 sum_sin += amp * np.sin(delta_phz)
#                 sum_cos += amp * np.cos(delta_phz)
#                 sum_amp += amp
#
#             ant_delta[0] = ant_coord[0, l] - ant_coord[0, m]
#             ant_delta[1] = ant_coord[1, l] - ant_coord[1, m]
#             ant_delta[2] = ant_coord[2, l] - ant_coord[2, m]
#             coord_delta = ant_delta[0] * r[0] + ant_delta[1] * r[1] + ant_delta[2] * r[2]
#             full_phase = freq_2pi_c * coord_delta + np.angle(sum_cos + 1j * sum_sin)
#             likelihood += sum_amp ** np.cos(full_phase)
#
#     return likelihood
#



def peleng(antenna, target):
    """ calculate pelengs from antenna to target"""
    import numpy as np
    dr = target - antenna.base;
    alpha = np.arctan( dr[1,0] / dr[0, 0])

    rxy = np.sqrt((target[0,0] - antenna.base[0,0])**2 + (target[1,0] - antenna.base[1,0])**2)
    thetta = np.angle(rxy +  1j * (target[2,0] - antenna.base[2,0]))
    peleng = np.array([alpha, thetta])
    return peleng


def aoa_likelihood(signal, ant_coord, peleng, f0):
    return aoa_likelihood_c(signal, ant_coord, peleng, f0)
    # likelihood = 0
    # wave_length = 3e8 / f0
    # freq_2pi_c = 2 * np.pi / wave_length
    # r = np.array([np.cos(peleng[1]) * np.cos(peleng[0]), np.cos(peleng[1]) * np.sin(peleng[0]), np.sin(peleng[1])])
    # l_max = ant_coord.shape[1] - 1
    # m_max = l_max + 1
    # k_max = signal.shape[1]
    # ant_delta = np.zeros(3)
    #
    # for l in range(l_max):
    #     for m in range(l + 1, m_max):
    #         sum_sin = 0
    #         sum_cos = 0
    #         sum_amp = 0
    #         for k in range(k_max):
    #             amp = np.abs(signal[m, k]) * np.abs(signal[l, k])
    #             delta_phz = np.angle(signal[l, k]) - np.angle(signal[m, k]) # !NOTE! похоже здесь ошибка в теории!
    #             sum_sin += amp * np.sin(delta_phz)
    #             sum_cos += amp * np.cos(delta_phz)
    #             sum_amp += signal[m, k] * np.conj(signal[l, k])
    #
    #         ant_delta[0] = ant_coord[0, m] - ant_coord[0, l]
    #         ant_delta[1] = ant_coord[1, m] - ant_coord[1, l]
    #         ant_delta[2] = ant_coord[2, m] - ant_coord[2, l]
    #         coord_delta = ant_delta[0] * r[0] + ant_delta[1] * r[1] + ant_delta[2] * r[2]
    #         pseudophase = np.angle(sum_cos + 1j * sum_sin)
    #         full_phase = freq_2pi_c * coord_delta + pseudophase
    #         likelihood += np.abs(sum_amp) ** np.cos(full_phase)
    #
    # return likelihood


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

    alpha_line = np.array([np.linspace(0, 2 * np.pi, size[1], endpoint=False)])
    alpha_mat = np.matmul(np.ones((1, size[0], 1)), alpha_line)

    betta_line = np.transpose(np.array([np.linspace(-np.pi / 2, np.pi / 2, size[0])]))
    betta_mat = np.matmul(betta_line, np.ones((1, 1, size[1])))

    peleng = np.concatenate((alpha_mat, betta_mat), axis=0)
    likelihood = np.zeros(size)
    for i in range(size[0]):
        for j in range(size[1]):
            likelihood[i, j] = aoa_likelihood(s[:, n_start:n_stop], antenna.to_cartesian(), peleng[:, i, j], f0)

    return likelihood

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
    import numpy as np
    bettaMin = np.radians(-85)
    bettaMax = np.radians(85)
    pointsTotal = 0

    def prepare(self, antenna, bettaStep):
        """Do advanced calculations"""
        import numpy as np

        betta = self.bettaMin
        alpha = 0
        self.pointsTotal = 0
        self.antenna = antenna

        while betta < self.bettaMax:
            if abs(betta + np.pi / 2) < 1e-2:
                betta += 0.01

            if abs(betta - np.pi / 2 ) < 1e-2:
                betta = self.bettaMax

            self.pointsTotal += 1
            alpha += bettaStep / np.cos(betta)

            if alpha >= 2 * np.pi:
                betta += bettaStep
                alpha = 0


        print(self.pointsTotal)

        self.alphas = np.zeros(self.pointsTotal)
        self.bettas = np.zeros( self.pointsTotal)

        self.pairsTotal = int(antenna.channelsTotal * (antenna.channelsTotal - 1) / 2)

        self.coordDeltas = np.zeros((self.pairsTotal, self.pointsTotal))
        antcoord = antenna.ToCartesian()
        betta = self.bettaMin
        alpha = 0
        pointInd = 0

        while betta < self.bettaMax:
            if abs(betta + np.pi / 2) < 1e-2:
                betta += 0.01

            if abs(betta - np.pi / 2 ) < 1e-2:
                betta = self.bettaMax

            self.alphas[pointInd] = alpha
            self.bettas[pointInd] = betta

            pairInd = 0

            r = np.array([[np.cos(betta) * np.cos(alpha)], [np.cos(betta) * np.sin(alpha)], [np.sin(betta)]])
            for i in range(antenna.channelsTotal - 1):
                for j in range(i + 1, antenna.channelsTotal):
                    antDelta = np.array([antcoord[:,i] - antcoord[:,j]]).T
                    self.coordDeltas[pairInd, pointInd] = np.matmul(antDelta.T, r)
                    pairInd += 1

            alpha += bettaStep / np.cos(betta)

            if alpha >= 2 * np.pi:
                betta += bettaStep
                alpha = 0

            pointInd += 1



    def estimate(self, signal, f0):

        import numpy as np

        pseudoPhase = np.zeros((self.pairsTotal))
        mutualEnergyAmp = np.zeros((self.pairsTotal))

        pairInd = 0
        for i in range(self.antenna.channelsTotal):
            for j in range(i + 1, self.antenna.channelsTotal):
                sum_sin = 0
                sum_cos = 0
                mutual_amp = 0
                for k in range(signal.shape[1]):
                    amp = np.abs(signal[i, k]) * np.abs(signal[j,k])
                    delta_phz = np.angle(signal[i,k]) - np.angle(signal[j,k])
                    sum_sin += amp * np.sin(delta_phz)
                    sum_cos += amp * np.cos(delta_phz)
                    mutual_amp += amp

                pseudoPhase[pairInd] = np.angle(sum_cos + 1j * sum_sin)
                mutualEnergyAmp[pairInd] = mutual_amp
                pairInd += 1



        L1_max = 0
        pointMax = 0
        L1_min = 0
        waveLength = 3e8 / f0
        freq_2pi_c = 2 * np.pi / waveLength

        for point in range(self.pointsTotal):
            L1 = 0
            for pair in range(self.pairsTotal):
                fullPhase = freq_2pi_c * self.coordDeltas[pair, point] + pseudoPhase[pair]
                L1 += mutualEnergyAmp[pair] * np.cos(fullPhase)

            if L1 > L1_max:
                L1_max = L1
                pointMax = point


        peleng = np.array([self.alphas[pointMax], self.bettas[pointMax]])
        return peleng