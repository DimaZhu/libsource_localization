import dtypes
import plotly.offline as ply
import plotly.graph_objs as go
import numpy as np

reader = dtypes.PyFrame_reader()
reader.open("/home/dima/Documents/1.bin")
frame = reader.read()

samples = frame.get_data()
data = []
print(samples.shape[0])


x = np.arange(0, samples.shape[1]/frame.get_sampling_frequency(),  1/frame.get_sampling_frequency())

for ch in range(1, int(samples.shape[0]/2)):
    data.append(go.Scatter(x=x,
                           y=np.abs(samples[0, :]),
                           mode='lines',
                           name="channel" + str(0)))

    data.append(go.Scatter(x=x,
                           y=np.abs(samples[ch * 2, :]),
                           mode='lines',
                           name="channel" + str(ch)))
    ply.plot(data)
    data.clear()





