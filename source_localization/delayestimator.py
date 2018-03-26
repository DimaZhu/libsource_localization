import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
import source_localization as src
from scipy.fftpack import fft, ifft, fftshift
import siggen


length = 2**16
fs = 100e6
t_pulse_width = 1e-6
t_period = 120.33333333e-6
snr = 0  # dB
start_period = 1
central_freq = 50e6
carrier = 10e6
post_id = 0
serial = 0
f_res = fs / length

n_start = 39320 - 400
n_stop = 39320 + 400
ref_ch = 0


sys = src.AntennaSystem()
sys.load("/home/dima/.touchdown/LandingSystem.ini")

antenna = src.Antenna()
antenna = sys[0]
target = sys[1].get_base()

s_f, t_delay = siggen.rimp(antenna, target, length, fs, t_pulse_width, t_period, carrier, get_tau=True, snr=snr)


frame = src.SpecFrame()
frame.set_data(s_f)
frame.set_central_frequency(0)
frame.set_sampling_frequency(fs)
frame.set_frequency_resolution(f_res)


estimator = src.DelayEstimator()
estimator.set_signal_parameters(ref_ch, n_start, n_stop)
time_delay_est = estimator.estimate(frame)

# amp = np.abs(s_f[0, :])
# phase = np.angle(s_f[1, :]) - np.angle(s_f[0, :])


test_passed = True
delta = np.max(abs(t_delay)) * 1e-3

for ch in range(antenna.get_channels_total()):

    print("Оценка времени запаздываня для канала ", ch, ": ",
          time_delay_est[ch])

    print("Истинная задержка", t_delay[ch] - t_delay[ref_ch])

    error = time_delay_est[ch] - (t_delay[ch] - t_delay[ref_ch])
    print("Ошибка", error)

    if np.abs(error) > delta:
        test_passed = False


# data = [go.Scatter(
#                    y=amp,
#                    mode="lines")]
#         # go.Scatter(x=x,
#         #            y=y[1, :],
#         #            mode="lines")]
#
#
# ply.plot(data)


if test_passed is True:
    print("\nTest successfully passed")
    exit(0)
else:
    print("\nTest failed")
    exit(1)






