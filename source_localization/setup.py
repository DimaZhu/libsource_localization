from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import numpy




dtypes = Extension(
    "dtypes",
    sources=["dtypes.pyx",  "antenna.cpp", "specframe.cpp", "profiler.cpp", "profile.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    include_dirs=[numpy.get_include(), './dlib/', './']
)


est = Extension(
   "pyestimator",
   sources=["pyestimator.pyx", "pelengator.cpp", "interpolator.cpp", "lh_pel.cpp", "lh.cpp", "specframe.cpp",
            "antenna.cpp", "grid.cpp", "profiler.cpp", "profile.cpp", "grid_pel.cpp"],
   language="c++",
   extra_compile_args=['-std=gnu++11'],
   include_dirs=[numpy.get_include(), './dlib/', './'],
)


setup(ext_modules=cythonize([dtypes, est]))
