import numpy as np
import onestage

from source_localization import siggen

#input parametrs
a = siggen.Antenna()
a.load("/home/dima/.landing/AFS4.ini")
snr_db = np.arange(-10, 15, 5)
snr = 10 ** (snr_db / 20)
f0 = 100e6;
N = 2 ** 16
fs = f0 * N / (N/2 -2);
f_res = 1/2e-3 #fs / N
df = 16e3
target = np.array([0, 0])
#Theoretical curve
var_th = np.zeros((2, snr.shape[0]))
for q in range(snr.shape[0]):
    var_th[:, q] = onestage.crb_phase(a, f0, df, f_res, snr[q], target)