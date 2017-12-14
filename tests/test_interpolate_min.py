import numpy as np
from inter import interpolate_min
import onestage
import plotly.plotly as pl
import plotly.graph_objs as go
from scipy import optimize

def mypoly(x):
    shift = np.array([3, 3])
    width = np.array([1, 2])
    angle = np.radians(0)

    x_rot = np.cos(angle) * x[0] - np.sin(angle) * x[1]
    y_rot = np.sin(angle) * x[0] + np.cos(angle) * x[1]

    z = (x_rot - shift[0]) ** 2 / width[0] + (y_rot - shift[1]) ** 2 / width[1]
    return z


def mysinc(x):
    shift = np.array([3, 4])
    width = np.array([10, 10])
    return -np.sinc((x[0] - shift[0]) / width[0]) * np.sinc((x[1] - shift[1]) / width[1])

n = np.arange(0, 10)
m = np.arange(0, 10)
x = onestage.meshgrid(m, n)

y = mysinc(x)
print(interpolate_min(x, y, mysinc))

data = [go.Surface(x=x[0],
                   y=x[1],
                   z=y)]
pl.plot(data, filename="Poly")

# x = np.array([0, 0])
# output = optimize.minimize(mypoly, x, method='Nelder-Mead')
# print(output.x)

