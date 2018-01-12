from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import numpy

dtypes = Extension(
    "dtypes",
    sources=["dtypes.pyx", "antenna.cpp", "specframe.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), '/usr/include', '/usr/include/QtCore/', './dlib/', './'],
)


grid = Extension(
    "grid",
    sources=["grid.pyx"],
    extra_compile_args=['-std=gnu++11'],
    language="c++",
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), '/usr/include/QtCore/'],
)

lh = Extension(
    "pylh",
    sources=["pylh.pyx", "lh_pel.cpp", "lh.cpp", "specframe.cpp", "antenna.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), './dlib/', '/usr/include/QtCore/', './'],
)


est = Extension(
    "pyestimator",
    sources=["pyestimator.pyx", "pelengator.cpp", "interpolator.cpp", "lh_pel.cpp", "lh.cpp", "specframe.cpp", "antenna.cpp", "grid.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=['/usr/include', '/usr/include/QtCore/',  numpy.get_include(), './dlib/', './'],
)


setup(ext_modules=cythonize([grid, dtypes, lh, est]))
