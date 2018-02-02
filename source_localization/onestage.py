import numpy as np

def likelihood_nb(signal, ant_coord, peleng, f0):
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
                delta_phz = np.angle(signal[l, k]) - np.angle(signal[m, k])
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

class Lh_Wb:
    def __init__(self, antenna_system, specframe, samp_start, samp_stop):
        self.antenna_system = antenna_system
        self.specframe = specframe
        self.samp_start = samp_start
        self.samp_stop = samp_stop
        self.data = specframe.get_python_data


    def calc(self, r):

        lh = 0

        for k in range(self.antenna_system.shape[1] - 1):
            for n in range(k+1, self.antenna_system.shape[1]):
                for l in range(self.antenna_system[k].get_channels_total()):
                    for m in range(self.antenna_system[n].get_channels_total()):

                        mutual_amp = 0
                        base_n = self.antenna_system[n].get_base()
                        tau_n = np.sqrt((r[0] - base_n[0])**2 + (r[1] - base_n[1])**2 + (r[2] - base_n[2])**2)

                        base_k = self.antenna_system[k].get_base()
                        tau_k = np.sqrt((r[0] - base_k[0])**2 + (r[1] - base_k[1])**2 + (r[2] - base_k[2])**2)

                        for samp in range(self.samp_start, self.samp_stop):
                            mutual_amp += self.data[n, m, samp] * np.conj(self.data[k, l, samp])\
                                          * np.exp(1j * 2 * np.pi * self.specframe.get_carrier\
                                                   / self.specframe.get_frequency_resolution()* (tau_n - tau_k))

                        lh += np.abs(mutual_amp)

        return lh




# def likelihood_wb():
    # likelihood = 0
    # wave_length = 3e8 / f0
    # freq_2pi_c = 2 * np.pi / wave_length
    # r = np.array([np.cos(peleng[1]) * np.cos(peleng[0]), np.cos(peleng[1]) * np.sin(peleng[0]), np.sin(peleng[1])])
    # l_max = ant_coord.shape[1] - 1
    # m_max = l_max + 1
    # k_max =
    # samp_max = signal.shape[1]
    # ant_delta = np.zeros(3)
    #
    # for k in range
    # for l in range(l_max):
    #     for m in range(l + 1, m_max):
    #         sum_sin = 0
    #         sum_cos = 0
    #         sum_amp = 0
    #         for samp in range(samp_max):
    #             amp = np.abs(signal[m, samp]) * np.abs(signal[l, samp])
    #             delta_phz = np.angle(signal[l, samp]) - np.angle(signal[m, samp])
    #             sum_sin += amp * np.sin(delta_phz)
    #             sum_cos += amp * np.cos(delta_phz)
    #             sum_amp += signal[m, samp] * np.conj(signal[l, samp])
    #
    #         ant_delta[0] = ant_coord[0, m] - ant_coord[0, l]
    #         ant_delta[1] = ant_coord[1, m] - ant_coord[1, l]
    #         ant_delta[2] = ant_coord[2, m] - ant_coord[2, l]
    #         coord_delta = ant_delta[0] * r[0] + ant_delta[1] * r[1] + ant_delta[2] * r[2]
    #         pseudophase = np.angle(sum_cos + 1j * sum_sin)
    #         full_phase = freq_2pi_c * coord_delta + pseudophase
    #         likelihood += np.abs(sum_amp) * np.cos(full_phase)
    #
    # return likelihood