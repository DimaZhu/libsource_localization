
from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.deque cimport deque
from libcpp.string cimport string

import numpy as np
cimport numpy as np

ctypedef  float complex * const * const FrameData
ctypedef  float complex * const FrameChannel
ctypedef  float complex ** Complex2d
ctypedef  float complex * Complex1d
ctypedef  vector[double] Estimation

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


# cdef extern from "complex.h" namespace "std":

cdef extern from "specframe.h" nogil:
    cdef cppclass SpecFrameParent:
        SpecFrameParent()
        void erase_frame(int serial)


cdef extern  from "specframe.h" nogil:
    cdef cppclass SpecFrame:
        SpecFrame(int i_channels_total, int i_samp_per_ch)
        SpecFrame()
        SpecFrame(SpecFrame &frame)

        const SpecFrame &operator=(SpecFrame& frame)
        const FrameData get_data() const
        void filter(Complex2d *freq_response)

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

        int get_serial() const



cdef extern from "specframewriter.h" nogil:
    cdef cppclass SpecFrameWriter:

        SpecFrameWriter()
        void write_sampling_frequency(SpecFrame *frame, double fs)
        void write_central_frequency(SpecFrame *frame, double f0)
        void write_frequency_resolution(SpecFrame *frame, double f_res)
        void write_post_id(SpecFrame *frame, double post_id)
        void write_serial(SpecFrame *frame, int serial)
        void write_bound(SpecFrame *frame, int bound)
        void write_parent(SpecFrame * frame, SpecFrameParent *parent)
        Complex2d get_mutable_data(SpecFrame *frame)
        void resize(SpecFrame *frame, int channels, int samp_per_ch)
        void clear(SpecFrame *frame)


cdef extern from "specframesaver.h" nogil:
    cdef cppclass SpecFrameSaver:
        SpecFrameSaver(int post_id);
        bool open(string);
        void save_title(float start_period);
        void save(SpecFrame*);
        void close();


cdef extern from "specframeloader.h" nogil:
    cdef cppclass SpecFrameLoader:
        SpecFrameLoader(int post_id);
        bool open(string);
        float read_title();
        void load(SpecFrame*);
        void close();

cdef extern from "lh.h":
    cdef cppclass Lh:
        Lh();
        double calculate(double alpha, double betta) const
        void set_verbose(bool arg)

cdef extern from "lhpel.h":
    cdef cppclass LhPel:
        LhPel(Antenna *ant, SpecFrame *data, int samp_start, int samp_stop, bool verbose)
        double calculate(double alpha, double betta) const
        void set_verbose(bool arg)

cdef extern  from "peleng.h" nogil:
    cdef cppclass Peleng:
        Peleng();
        double azimuth;
        double elevation;
        double probability;
        vector[vector[float]] phase_center;
        long int time;


cdef extern from "pelengator.h" nogil:
    cdef cppclass Pelengator:
        Pelengator(const Antenna & ant, double frequency_resolution, double sampling_frequency)

        void set_signal_param(double f0, int samp_start, int samp_stop)
        Estimation estimate(SpecFrame *frame)
        void turn_on_interpolation(bool turn_on)
        void set_verbose(bool)

cdef extern from "triangulator.h" nogil:
    cdef cppclass Triangulator:
        Triangulator();
        void set_active(bool arg);
        bool is_active();
        vector[vector[float]] estimate(vector[Peleng] pelengs);


cdef extern from "estimator.h" nogil:
    cdef cppclass Estimator:
        Estimator();
        Estimation estimate(SpecFrame * frame) const;


cdef extern from "pseudophaseestimator.h" nogil:
    cdef cppclass PseudoPhaseEstimator:
        PseudoPhaseEstimator();
        void set_signal_parameters(int ref_ch, int  i_samp_start, int i_samp_stop);
        Estimation estimate(SpecFrame * frame) const


cdef extern from "delayestimator.h" nogil:
    cdef cppclass DelayEstimator:
        DelayEstimator();
        void set_signal_parameters(int ref_ch, int  i_samp_start, int i_samp_stop);
        Estimation estimate(SpecFrame * frame) const

