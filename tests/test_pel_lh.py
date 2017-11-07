import numpy as np
import plotly.plotly as ply
import plotly.graph_objs as go
import onestage
import siggen
from pydatatypes import PyAntenna as Antenna
# import pel_lh
#
# ans = pel_lh.Pel_Lh.fun()
# print(ans)



#input parameters
a = Antenna("/home/dima/.landing/AFS4.ini")
el = a.get_elements()
f0 = 120e6
N = 2 ** 16
fs = f0 * N / (N/2 - 2)
f_res = fs / N
df = f_res

lh_size = (200, 200)
alpha = np.linspace(0, 2 * np.pi, lh_size[1])
betta = np.linspace(-np.pi / 2, np.pi / 2, lh_size[0])
alpha_grid, betta_grid = np.meshgrid(alpha, betta)


lh_size = (200, 200)
alpha = np.linspace(0, 2 * np.pi, lh_size[1])
betta = np.linspace(-np.pi / 2, np.pi / 2, lh_size[0])
alpha_grid, betta_grid = np.meshgrid(alpha, betta)


target_size = (1, 1)
alpha_target = np.radians(180)
betta_target = np.radians(0)
pel = np.array([alpha_target, betta_target])

lh = onestage.arp(a, f0, df, fs, f_res, lh_size, peleng=pel)
x = lh * np.cos(betta_grid) *np.cos(alpha_grid)
y = lh * np.cos(betta_grid) * np.sin(alpha_grid)
z = lh * np.sin(betta_grid)
trace = go.Surface(    name = np.str(np.rad2deg(pel)),
                       x=x,
                       y=y,
                       z=z)
data = [trace]
ply.plot(data, filename='Antenna radiaton pattern Sphere')
