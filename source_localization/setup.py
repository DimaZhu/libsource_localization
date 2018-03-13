from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import numpy


source_localization = Extension(
   "source_localization",
   sources=["source_localization.pyx", "pelengator.cpp", "interpolator.cpp", "lhpel.cpp", "lh.cpp", "specframe.cpp",
            "antenna.cpp", "specframesaver.cpp", "specframeloader.cpp", "gridpel.cpp", "triangulator.cpp",
            "peleng.cpp"],
   language="c++",
   extra_compile_args=['-std=gnu++11'],
   include_dirs=[numpy.get_include(), './dlib/', './'],
)


setup(ext_modules=cythonize([source_localization]))
