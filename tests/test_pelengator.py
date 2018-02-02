import siggen as sig
import dtypes as dt
import numpy as np
import onestage
import plotly.offline as pl
import plotly.graph_objs as go
import pyestimator as est

#input parameters
a = dt.PyAntenna()
a.load("/home/dima/.landing/AFS4.ini")
el = a.get_elements()
f0 = 1310e6 # 100e6 / 2**16 * 10000
N = 2 ** 16
fs = 2.1 * f0 # f0 * N / (N/2 - 2)
f_res = fs / N
df = 0 #f_res
n_stop = int(np.ceil((f0 + df / 2) / f_res))
n_start = int(np.ceil((f0 - df / 2) / f_res))


alpha = np.arange(0, 360, 10)
betta = np.arange(-85, 85, 10)
grid_alpha, grid_betta = np.meshgrid(alpha, betta)
mse = np.zeros(grid_alpha.shape)
R = 1e4

# pelengator = est.PyPelengator(a, f_res, fs)
# pelengator.set_verbose(True)
# pelengator.turn_on_interpolation(False)
# pelengator.set_signal_parameters(f0, 0, n_stop - n_start)

pelengator = onestage.PelengEstimator(a, f0, df, f_res, fs)

for i in range(betta.size):
    for j in range(alpha.size):

        target_pos = np.array([R * np.cos(np.radians(grid_alpha[i][j])) * np.cos(np.radians(grid_betta[i][j])),
                               R * np.sin(np.radians(grid_alpha[i][j])) * np.cos(np.radians(grid_betta[i][j])),
                               R * np.sin(np.radians(grid_betta[i][j]))])

        sig_length = int(fs/f_res)
        signal = sig.wgn_baseband(a, target_pos, sig_length, fs)

        frame = dt.PySpecFrame()
        frame.set_data(signal[:, n_start:n_stop+1])
        frame.set_carrier(f0)
        frame.set_sampling_frequency(fs)
        frame.set_frequency_resolution(f_res)

        estimation = np.rad2deg(pelengator.estimate(frame))

        delta_alpha = abs(estimation[0] - grid_alpha[i][j])
        if delta_alpha >= 180:
            delta_alpha -= 360

        delta_betta = abs(estimation[1] - grid_betta[i][j])
        if delta_betta >= 180:
            delta_betta -= 360

        mse[i][j] = np.sqrt(delta_alpha**2 + delta_betta**2)

        print("Target estimation: ", estimation)
        print("True target position: ", grid_alpha[i][j], grid_betta[i][j], "\n\n")


data = [go.Surface(x=grid_alpha,
                   y=grid_betta,
                   z=mse)]
pl.plot(data)



