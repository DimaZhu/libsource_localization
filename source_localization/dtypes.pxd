
from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.deque cimport deque

import numpy as np
cimport numpy as np

ctypedef  complex **complex2d;
ctypedef  complex* complex1d;

cdef extern from "antenna.h" nogil:
    cdef cppclass Antenna:
        Antenna() except +
        Antenna(char* ) except +
        Antenna(const Antenna &antenna)

        # seters
        void set_model(vector[ vector[float]] coordinates)
        void set_base(vector[float]  coordinates)
        void set_orientation(float i_orientation)

        # geters
        vector[vector[float]] get_model() const
        vector[vector[float]] get_elements() const
        vector[float] get_base() const

        vector[float] get_elements_on_stages() const
        float get_orientation() const
        int get_channels_total() const


cdef class PyAntenna:
    cdef Antenna *c_ant

cdef object PyAntenna_factory(Antenna *ptr)

cdef Antenna *Antenna_factory(PyAntenna ant)

# cdef extern from "complex.h" namespace "std":


cdef extern from "profiler.h" nogil:
    cdef cppclass Profiler:
        Profiler()
        # void start(QString name)
        # void stop(QString name)
        void clear()
        Profile get_profile()

cdef extern from "profile.h" nogil:
    cdef cppclass Profile:
        Profile_entity()
        int time_total
        int time_aver
        int count
        float time_percentage


cdef extern from "specframe.h" nogil:
    cdef cppclass SpecFrame:
        SpecFrame(int i_channels_total, int i_samp_per_ch)
        SpecFrame(SpecFrame &frame)

        const SpecFrame &operator=(SpecFrame& frame)
        complex2d *get_data(int &channels, int &samp_per_ch)
        complex2d *get_data()

        void set_data(complex2d *new_data_ptr, int i_channels_total, int i_samp_per_ch)
        void filter(complex2d *freq_response)

        void clear()
        void erase()

        int ind(int samp) const

        bool is_band_limited() const

        void set_sampling_frequency(double fs)
        double get_sampling_frequency() const

        void set_carrier(double f0)
        double get_carrier() const

        void set_frequency_resolution(double f_res)
        double get_frequency_resolution() const

        double get_band_width() const

        int get_channels_total() const

        int get_length() const

        int get_full_frame_length() const

        void set_post_id(int id)
        int get_post_id() const

        void set_count(int )
        int get_count() const

        void set_bound(int bound)
        int get_bound() const

        Profiler profile

cdef class PySpecFrame:
    cdef SpecFrame *c_frame
    cdef void set_python_data(self, np.ndarray[np.complex128_t, ndim=2] data)
    cdef np.ndarray[np.complex128_t, ndim=2] get_python_data(self)

cdef SpecFrame *SpecFrame_factory(PySpecFrame frame)