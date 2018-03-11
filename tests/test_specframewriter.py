import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
from scipy import signal as sg
import source_localization as src

ch_total = 6
length = 2**15 + 100000
fs = 100e6
t_pulse_width = 100e-6
t_period = 200e-6
snr = 1000000  # dB
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
y = st.norm.rvs(size=(ch_total, length), scale=10**(-snr/20))


for ch in range(ch_total):
    for p in range(int(np.ceil(length/period))):
        for i in range(int(np.floor(pulse_width))):
            if (delay[ch] + i + p*period) >= length:
                break
            y[ch, delay[ch] + i + p * period] += 1


factory = src.SpecFrameFactory()
factory.write_post_id(0)
factory.write_serial(0)
factory.write_bound(0)
factory.write_central_frequency(50e6)
factory.write_sampling_frequency(fs)
factory.write_frequency_resolution(fs/length)

frame = factory.get_frame()
print(frame.get_post_id())


factory.write_post_id(1)
print(frame.get_post_id())

writer = src.SpecFrameWriter(0)
writer.save_