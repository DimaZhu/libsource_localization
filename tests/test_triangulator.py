import pyestimator
import numpy as np

pel1 = pyestimator.PyPeleng()
pel1.set_azimuth(np.radians(30))
pel1.set_elevation(0)
pel1.set_phase_center(np.array([0, 0, 0]))

pel2 = pyestimator.PyPeleng()
pel2.set_azimuth(np.radians(-45))
pel2.set_elevation(0)
pel2.set_phase_center(np.array([0, 1, 1]))

pel3 = pyestimator.PyPeleng()
pel3.set_azimuth(np.radians(260))
pel3.set_elevation(np.radians(-10))
pel3.set_phase_center(np.array([1, 1, 1]))

pelengs = [pel1, pel2, pel3]

tr = pyestimator.PyTriangulator()
print(tr.estimate(pelengs))
