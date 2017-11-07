from libcpp.vector cimport vector
import numpy as np
cimport numpy as np

cdef extern from "./datatypes/antenna.h":
    cdef cppclass Antenna:
        Antenna() except +
        Antenna(char* ) except +
        Antenna(const Antenna &antenna)

        # seters
        void set_model_coordinates(vector[ vector[float]] coordinates)
        void set_base_coordinates(vector[float]  coordinates)
        void set_orientation(float i_orientation)

        # geters
        vector[vector[float]] get_model_coordinates() const
        vector[vector[float]] get_elements_coordinates() const
        vector[float] get_base_coordinates() const

        vector[float] get_elements_on_stages() const
        float get_orientation() const
        int get_channels_total() const


cdef object PyAntenna_factory(Antenna *ptr)

cdef extern from ".datatypes/specframe.h":
    ctypedef  vector[vector[complex[double]]] complex2d;
    cdef cppclass SpecFrame:
        SpecFrame() except +
        SpecFrame(const SpecFrame &frame) except +

        const SpecFrame &operator=(const SpecFrame& frame)
        complex2d *get_data()
        complex2d get_data(int samp_start, int samp_stop) const
        complex2d get_data(int ch_start, int ch_stop, int samp_start, int samp_stop) const

        void set_data(complex2d data)
        void push_back(int ch, int length, complex[double] * data)

        void clear()

        int ind(int samp) const

        bool is_band_limited() const
        bool is_in_band(int samp) const

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

cdef object PySpecFrame_factory(SpecFrame *ptr)
