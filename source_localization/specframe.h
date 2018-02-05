#ifndef SPECFRAME_H
#define SPECFRAME_H

#include <cstring>
#include <complex>
#include <vector>
#include <profiler.h>
#include <deque>
#include <cassert>

using namespace std;
typedef  complex<double> ** complex2d;
typedef  complex<double> *complex1d;


class SpecFrame_parent
{
public:
    SpecFrame_parent();

    virtual void erase_frame(int frame_number) = 0;
};

class SpecFrame
{
public:
    SpecFrame(int i_channels_total = 0, int i_samp_per_ch = 0, SpecFrame_parent *i_parent = 0);
    SpecFrame(SpecFrame &frame);
    ~SpecFrame();

    const SpecFrame &operator=(SpecFrame& frame);
    complex2d *get_data(int &channels, int &samp_per_ch);
    complex2d *get_data();

    void set_data(complex2d *new_data_ptr, int i_channels_total, int i_samp_per_ch);
    void filter(complex2d freq_response);
    void filter(complex1d freq_response, int ch_ind);

    void clear();
    void erase();
    void resize(int i_channels_total, int i_samp_per_ch);

    //Пересчитвает нужный отсчет в индекс кадра
    int ind(int samp) const;

    bool is_band_limited() const; //Включен ли режим ограниченной полосы
    bool is_in_band(int samp) const;

    void set_sampling_frequency(double fs);
    double get_sampling_frequency() const;

    void set_carrier(double f0);
    double get_carrier() const;

    void set_frequency_resolution(double f_res);
    double get_frequency_resolution() const;

    double get_band_width() const;

    int get_channels_total() const;

    int get_length() const;

    int get_full_frame_length() const;

    void set_post_id(int id);
    int get_post_id() const;

    void set_count(int );
    int get_count() const;

    void set_bound(int bound);
    int get_bound() const;

    SpecFrame_parent * get_parent();
    void set_parent(SpecFrame_parent *i_parent);


    Profiler profiler;
private:

    SpecFrame_parent *parent;
    void allocate_memory();
    void free_memory();

    complex2d data;
    int channels_total;
    int samp_per_channel;

    double f0;
    double sampling_frequency;
    double freq_resolution;

    int postId;
    int count;

    int boundInd; // В режиме ограниченной полосы, показывает какому отсчету
                  // целого кадра соответсвует первый отсчет

};
#endif // SPECFRAME_H
