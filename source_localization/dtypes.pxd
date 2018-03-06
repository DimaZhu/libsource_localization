
from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.deque cimport deque
from libcpp.string cimport string

import numpy as np
cimport numpy as np

ctypedef  complex **complex2d;
ctypedef  complex* complex1d;

cdef extern from "antenna.h" nogil:
    cdef cppclass Antenna:
        Antenna() except +
        Antenna(string ) except +
        Antenna(const Antenna &antenna)

        # seters
        void set_model(vector[ vector[float]] coordinates)
        void set_base(vector[float]  coordinates)
        void set_base(vector[vector[float]] coord)
        void set_phase_center(vector[float] coord)
        void set_phase_center(vector[vector[float]] coord)
        void set_orientation(float i_orientation)
        void set_name(string name)

        # geters
        vector[vector[float]] get_model() const
        vector[vector[float]] get_elements() const
        vector[vector[float]] get_base() const
        vector[vector[float]] get_phase_center() const

        vector[float] get_elements_on_stages() const
        float get_orientation() const
        int get_channels_total() const

    cdef cppclass coordinates


cdef class PyAntenna:
    cdef Antenna *c_ant


cdef object py_antenna_factory(Antenna *ptr)


cdef Antenna *antenna_factory(PyAntenna ant)


# cdef extern from "complex.h" namespace "std":

cdef extern from "specframe.h" nogil:
    cdef cppclass SpecFrame:
        SpecFrame(int i_channels_total, int i_samp_per_ch)
        SpecFrame(SpecFrame &frame)

        const SpecFrame &operator=(SpecFrame& frame)
        const complex2d get_data(int &channels, int &samp_per_ch) const
        const complex2d get_data() const
        void filter(complex2d *freq_response)

        void erase()

        int ind(int samp) const

        bool is_band_limited() const

        double get_sampling_frequency() const

        double get_central_frequency() const

        double get_frequency_resolution() const

        double get_band_width() const

        int get_channels_total() const

        int get_length() const

        int get_full_frame_length() const

        int get_post_id() const

        int get_count() const

        int get_bound() const


cdef class PySpecFrame:
    cdef SpecFrame *c_frame
    # cdef void set_python_data(self, np.ndarray[np.complex128_t, ndim=2] data)
    cdef np.ndarray[np.complex128_t, ndim=2] get_python_data(self)

cdef SpecFrame *spec_frame_factory(PySpecFrame frame)
cdef PySpecFrame py_spec_frame_factory(SpecFrame *frame)

cdef extern from "specframereader.h" nogil:
    cdef cppclass SpecFrameReader:
        SpecFrameReader(int post_id = -1);
        bool open(string);
        void read(SpecFrame*);
        void close();

cdef class PyFrameReader:
    cdef SpecFrameReader c_reader
    cdef SpecFrame *c_frame