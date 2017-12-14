import numpy as np
import dtypes as dt
import siggen as sig

#input parameters
a = dt.PyAntenna()
a.load("/home/dima/.landing/AFS32.ini")
el = a.get_elements()
f0 = 120e6
N = 2 ** 16
fs = f0 * N / (N/2 - 2)
f_res = fs / N
df = f_res
alpha = 0
betta = 0
R = 1e4

target_pos = np.array([R * np.cos(np.radians(alpha)) * np.cos(np.radians(betta)),
                       R * np.sin(np.radians(alpha)) * np.cos(np.radians(betta)),
                       R * np.sin(np.radians(betta))])

sig_length = int(fs / f_res)
signal = sig.wgn_baseband(a, target_pos, sig_length, fs)
n_stop = int(np.ceil((f0 + df / 2) / f_res))
n_start = int(np.ceil((f0 - df / 2) / f_res))

frame = dt.PySpecFrame()
frame.set_data(signal[:, n_start:n_stop])
frame.set_carrier(f0)
frame.set_sampling_frequency(fs)
frame.set_frequency_resolution(f_res)