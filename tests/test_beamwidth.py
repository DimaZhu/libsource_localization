import siggen as sig
import dtypes as dt
import numpy as np
import onestage
import plotly.offline as pl
import plotly.graph_objs as go

#input parameters
a = dt.PyAntenna()
a.load("/home/dima/.landing/AFS32.ini")
el = a.get_elements()
f0 = 120e6
N = 2 ** 16
fs = 250e6
f_res = 10e3
df = f_res

width = onestage.beam_width_est(antenna=a, f0=f0, df=df, fs=fs, f_res=f_res)
print(np.rad2deg(width))