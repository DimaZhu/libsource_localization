from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import numpy
import os, fnmatch


def find(pattern, path):
    result = []
    for root, dirs, files in os.walk(path):
        for name in files:
            if fnmatch.fnmatch(name, pattern):
                result.append(os.path.join(root, name))
    return result


def get_path_to_file(full_path):

    for i in range(1, len(full_path)):
        if full_path[-i] is '/':
            path_to = full_path[:-i]
            break

    return path_to



dtypes = Extension(
    "dtypes",
    sources=["dtypes.pyx", "antenna.cpp", "specframe.cpp", "profiler.cpp", "profile.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=[get_path_to_file(find("libQtCore.so", "/")[0])],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), '/usr/include', get_path_to_file(find("qstring.h", "/usr")[0]), './dlib/', './'],
)


grid = Extension(
    "grid",
    sources=["grid.pyx"],
    extra_compile_args=['-std=gnu++11'],
    language="c++",
    library_dirs=[get_path_to_file(find("libQtCore.so", "/")[0])],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), get_path_to_file(find("qstring.h", "/usr")[0])],
)


lh = Extension(
    "pylh",
    sources=["pylh.pyx", "lh_pel.cpp", "lh.cpp", "specframe.cpp", "antenna.cpp", "profiler.cpp", "profile.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=[get_path_to_file(find("libQtCore.so", "/")[0])],
    libraries=["QtCore"],
    include_dirs=[numpy.get_include(), './dlib/', get_path_to_file(find("qstring.h", "/usr")[0]), './'],
)


est = Extension(
    "pyestimator",
    sources=["pyestimator.pyx", "pelengator.cpp", "interpolator.cpp", "lh_pel.cpp", "lh.cpp", "specframe.cpp", "antenna.cpp", "grid.cpp", "profiler.cpp", "profile.cpp"],
    language="c++",
    extra_compile_args=['-std=gnu++11'],
    library_dirs=[get_path_to_file(find("libQtCore.so", "/")[0])],
    libraries=["QtCore"],
    include_dirs=['/usr/include', get_path_to_file(find("qstring.h", "/usr")[0]),  numpy.get_include(), './dlib/', './'],
)


setup(ext_modules=cythonize([grid, dtypes, lh, est]))
