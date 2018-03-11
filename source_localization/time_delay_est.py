import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
from scipy import signal as sg
length = 2**15 + 100000
fs = 100e6
t_pulse_width = 100e-6
t_period = 200e-6
snr = 10  # dB
t_delay = np.array([0, 1e-6])

pulse_width = int(t_pulse_width * fs)
period = int(t_period * fs)
delay = (t_delay * fs).astype(np.int)

x = np.arange(0, length/fs, 1/fs)
y = st.norm.rvs(size=(2, length), scale=10**(-snr/20))


for ch in range(2):
    for p in range(int(np.ceil(length/period))):
        for i in range(int(np.floor(pulse_width))):
            if (delay[ch] + i + p*period) >= length:
                break
            y[ch, delay[ch] + i + p * period] += 10


acf = sg.convolve(y[0, :], y[0, :])
vcf = sg.convolve(y[0, :], y[1, :])
delay_est = np.argmax(vcf) - np.argmax(acf)
t_delay_est = delay_est / fs

print("Estimated delay is: ", t_delay_est)
print("True delay is: ", t_delay[1] - t_delay[0])
print("Error: ", abs(t_delay_est - (t_delay[1] - t_delay[0])))
#
# data = [go.Scatter(
#                    y=vcf,
#                    mode="lines")]
#         go.Scatter(x=x,
#                    y=y[1, :],
#                    mode="lines")]
#
#
#
# ply.plot(data)