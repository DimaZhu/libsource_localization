import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
from scipy import signal as sg
import source_localization as src

file_name = "/home/dima/Projects/Touchdown/Data/test.bin"
ch_total = 6
length = 2**15 + 100000
fs = 100e6
t_pulse_width = 100e-6
t_period = 200e-6
snr = 1000000  # dB
start_period = 1
f0 = 50e6
post_id = 0
serial = 0


y = st.norm.rvs(size=(ch_total, length), scale=10**(-snr/20)) +\
    1j * st.norm.rvs(size=(ch_total, length), scale=10**(-snr/20))

frame = src.SpecFrame(post_id=post_id, serial=serial, f0=f0, fs=fs, f_res=fs/length, sig=y)

writer = src.SpecFrameSaver(post_id)
writer.open(file_name)
writer.save_title(start_period)
writer.save(frame)
writer.close()

print("Successfully saved frame")

loader = src.SpecFrameLoader(post_id)
success = loader.open(file_name)

if not success:
    print("Enable to open file")
    exit(1)

i_start_period = loader.read_title()
i_frame = src.SpecFrame()
loader.load(i_frame)
loader.close()


error = 1e-7
failed = False
if abs(start_period - i_start_period) > error:
    print("\n\n\nTest failed. \nSaved start period was: ", start_period, "\nLoaded start period is: ", i_start_period)
    failed = True

if post_id is not i_frame.get_post_id():
    print("\n\n\nTest failed. \nSaved post id was: ", post_id, "\nLoaded post id is: ", i_frame.get_post_id())
    failed = True

if serial is not i_frame.get_serial():
    print("\n\n\nTest failed. \nSaved serial was: ", serial, "\nLoaded serial is: ", i_frame.get_serial())
    failed = True

if abs(f0 - i_frame.get_central_frequency()) > error:
    print("\n\n\nTest failed. \nSaved central frequency was: ", f0, "\nLoaded central frequency is: ",
          i_frame.get_central_frequency())
    failed = True

if abs(fs - i_frame.get_sampling_frequency()) > error:
    print("\n\n\nTest failed. \nSaved sampling frequency was: ", fs, "\nLoaded sampling frequency is: ",
          i_frame.get_sampling_frequency())
    failed = True

if abs(fs/length - i_frame.get_frequency_resolution()) > error:
    print("\n\n\nTest failed. \nSaved frequency resolution was: ", fs/length, "\nLoaded frequency resolution is: ",
          i_frame.get_frequency_resolution())
    failed = True

data = i_frame.get_data()

if ch_total is not data.shape[0]:
    print("\n\n\nTest failed. \nSaved channels total was: ", ch_total, "\nLoaded channels total is: ",
         data.shape[0])
    failed = True

if abs(length  - data.shape[1]) > error:
    print("\n\n\nTest failed. \nSaved samples per channel was: ", length, "\nLoaded samples per channel is: ",
         data.shape[1])
    failed = True


for ch in range(ch_total):
    for s in range(length):
        if abs((data[ch, s]) - y[ch, s]) > error:
            print("\n\n\nTest failed. \nData mismatch. \nChannel: ", ch, "\nSample: ", s)
            failed = True


if not failed:
    print("Test passed successfully")



