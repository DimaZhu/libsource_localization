from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import numpy

pydatatypes = Extension(
    "pydatatypes",
    sources=["pydatatypes.pyx", "datatypes/antenna.cpp"],
    language="c++",
    extra_compile_args=['-std=c++11'],
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=['/usr/include', '/usr/include/QtCore/', numpy.get_include()],
    package_dir={'': 'pydatatypes'}
)

grid_synth = Extension(
    "grid_synth",
    sources=["grid_synth.pyx", "pydatatypes.pyx", "datatypes/antenna.cpp"],
    extra_compile_args=['-std=c++11'],
    language="c++",
    library_dirs=['/usr/lib64/'],
    libraries=["QtCore"],
    include_dirs=["/usr/include", '/usr/include/QtCore/', numpy.get_include()],
    package_dir={'': 'grid_synth'}
)

setup(ext_modules=cythonize(pydatatypes))
setup(ext_modules=cythonize(grid_synth))
