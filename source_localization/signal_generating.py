import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
import source_localization as src
from scipy.fftpack import fft, ifft
from scipy.signal import convolve

file_name = "/home/dima/Projects/Touchdown/Data/train_imp.bin"
ch_total = 6
length = 2**15
fs = 100e6
t_pulse_width = 100e-6
t_period = 200e-6
snr = 1000000  # dB
start_period = 1
f0 = 50e6
post_id = 0
serial = 0

t_delay = np.zeros(ch_total)
t_delay[0] = 0
t_delay[1] = 1e-6
t_delay[2] = 2e-6
t_delay[3] = 3e-6
t_delay[4] = 3e-6
t_delay[5] = 4e-6

pulse_width = int(t_pulse_width * fs)
period = int(t_period * fs)
delay = (t_delay * fs).astype(np.int)

x = np.arange(0, length/fs, 1/fs)
y = st.norm.rvs(size=(ch_total, length), scale=10**(-snr/20)) +\
    1j * st.norm.rvs(size=(ch_total, length), scale=10**(-snr/20))


for ch in range(ch_total):
    for p in range(int(np.ceil(length/period))):
        for i in range(int(np.floor(pulse_width))):
            if (delay[ch] + i + p*period) >= length:
                break
            y[ch, delay[ch] + i + p * period] += 1


z = ifft(fft(np.real(y[0, :])) * np.real(fft(y[1, :])))
p = convolve(np.real(y[0, :]), np.real(fft(y[1, :])), mode="same")
data = [go.Scatter(x=x,
                   y=np.real(z),
                   mode="lines"),
        go.Scatter(x=x,
                   y=p,
                   mode="lines")]
ply.plot(data)

frame = src.SpecFrame(post_id=post_id, serial=serial, f0=f0, fs=fs, f_res=fs/length, sig=y)

writer = src.SpecFrameSaver(post_id)
writer.open(file_name)
writer.save_title(start_period)
writer.save(frame)
writer.close()

print("Successfully saved frame")
