import numpy as np
import plotly.offline as ply
import plotly.graph_objs as go
from scipy import stats as st
import source_localization as src
from scipy.fftpack import fft, ifft
from scipy.signal import convolve

file_name = "/home/dima/Projects/Touchdown/Data/26.03.18_2/p0.bin"

frame = src.SpecFrame()

loader = src.SpecFrameLoader()
loader.open(file_name)
loader.read_title()
loader.load(frame)
loader.close()

data = frame.get_data()
data = [go.Scatter(y=np.real(data[0, :]),
                   mode="lines")]

ply.plot(data)