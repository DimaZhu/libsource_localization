import siggen as sig
import estimator as est
import dtypes as dt
import numpy as np

#input parameters
a = dt.PyAntenna()
a.load("/home/dima/.landing/AFS4.ini")
el = a.get_elements()
f0 = 120e6
N = 2 ** 16
fs = f0 * N / (N/2 - 2)
f_res = fs / N
df = f_res


target_pel = np.array([10, 20])
R = 1e4
target_pos = np.array([R * np.cos(np.radians(target_pel[0])) * np.cos(np.radians(target_pel[1])),
                       R * np.sin(np.radians(target_pel[0])) * np.cos(np.radians(target_pel[1])),
                       R * np.sin(np.radians(target_pel[1]))])

sig_length = int(fs/f_res)
signal = sig.wgn_baseband(a, target_pos, sig_length, fs)
n_stop = int(np.ceil((f0 + df / 2) / f_res))
n_start = int(np.ceil((f0 - df / 2) / f_res))

frame = dt.PySpecFrame()
frame.set_data(signal[:, n_start:n_stop])
frame.set_carrier(f0)
frame.set_sampling_frequency(fs)
frame.set_frequency_resolution(f_res)

pelengator = est.PyPelengator(a)
pelengator.set_signal_parameters(f0, df, f_res, fs)
estimation = pelengator.estimate(frame)

print("Target estimation: ", np.rad2deg(estimation))



