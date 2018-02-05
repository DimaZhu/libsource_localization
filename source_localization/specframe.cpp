#include "specframe.h"

SpecFrame::SpecFrame(int i_channels_total, int i_samp_per_ch, SpecFrame_parent *i_parent):
    parent(i_parent),
    channels_total(i_channels_total),
    samp_per_channel(i_samp_per_ch),
    f0(NAN),
    sampling_frequency(NAN),
    freq_resolution(NAN),
    postId(-1),
    count(-1),
    boundInd(0)
{
    if ((i_channels_total == 0) || (i_samp_per_ch == 0))
        data = nullptr;
    else
        allocate_memory();
}

SpecFrame::SpecFrame(SpecFrame &frame):
    parent(frame.get_parent()),
    channels_total(frame.get_channels_total()),
    samp_per_channel(frame.get_length()),
    f0(frame.get_carrier()),
    sampling_frequency(frame.get_sampling_frequency()),
    freq_resolution(frame.get_frequency_resolution()),
    postId(frame.get_post_id()),
    count(frame.get_count()),
    boundInd(frame.get_bound())
{
    free_memory();
    allocate_memory();

    complex2d * new_data_ptr = frame.get_data();
    complex2d new_data = *new_data_ptr;

    for (int ch = 0; ch < channels_total; ++ch)
        for (int s = 0; s < samp_per_channel; ++s)
            data[ch][s] = new_data[ch][s];

}

SpecFrame::~SpecFrame()
{
    free_memory();
}

const SpecFrame &SpecFrame::operator =(SpecFrame &frame)
{
    // Избегать самоприсваивания
    if (&frame != this) {

        parent=frame.get_parent();
        channels_total = frame.get_channels_total();
        samp_per_channel = frame.get_length();
        f0 = frame.get_carrier();
        sampling_frequency = frame.get_sampling_frequency();
        freq_resolution = frame.get_frequency_resolution();
        postId = frame.get_post_id();
        count = frame.get_count();
        boundInd = frame.get_bound();

        free_memory();
        allocate_memory();

        complex2d * new_data_ptr = frame.get_data();
        complex2d new_data = *new_data_ptr;

        for (int ch = 0; ch < channels_total; ++ch)
            for (int s = 0; s < samp_per_channel; ++s)
                data[ch][s] = new_data[ch][s];
    }
    return *this;
}

void SpecFrame::allocate_memory()
{
    data = (complex2d ) malloc( channels_total * sizeof(complex1d));
    for (int ch = 0; ch < channels_total; ++ch)
        data[ch] = (complex1d) malloc(samp_per_channel * sizeof(complex<double>));

    clear();
}

void SpecFrame::free_memory()
{
    if(data != nullptr) {
        for (int ch = 0; ch < channels_total; ++ch)
            free(data[ch]);
        free(data);
    }
}

void SpecFrame::resize(int i_channels_total, int i_samp_per_ch)
{
    channels_total = i_channels_total;
    samp_per_channel = i_samp_per_ch;

    free_memory();
    allocate_memory();

}

complex2d *SpecFrame::get_data(int &channels, int &samp_per_ch)
{
    channels = channels_total;
    samp_per_ch = samp_per_channel;
    return &data;
}

complex2d *SpecFrame::get_data()
{
    return &data;
}

void SpecFrame::set_data(complex2d *new_data_ptr, int i_channels_total, int i_samp_per_ch)
{
    channels_total = i_channels_total;
    samp_per_channel = i_samp_per_ch;

    free_memory();
    allocate_memory();

    complex2d new_data = *new_data_ptr;

    for (int ch = 0; ch < channels_total; ++ch)
      for (int s = 0; s < samp_per_channel; ++s)
          data[ch][s] = new_data[ch][s];
}

void SpecFrame::filter(complex2d freq_response)
{

    for (int ch = 0; ch < channels_total; ++ch)
      for (int s = 0; s < samp_per_channel; ++s)
          data[ch][s] = data[ch][s] * freq_response[ch][s + boundInd];

}

void SpecFrame::filter(complex1d freq_response, int ch_ind)
{
    for (int s = 0; s < samp_per_channel; ++s)
        data[ch_ind][s] = data[ch_ind][s] * freq_response[s + boundInd];
}

void SpecFrame::clear()
{

    f0 = NAN;
    count = -1;

    profiler.clear();

    for (int ch = 0; ch < channels_total; ++ch)
      for (int s = 0; s < samp_per_channel; ++s)
          data[ch][s] = 0;

}

int SpecFrame::ind(int samp) const
{
    assert(samp >= 0);
    samp -= boundInd;
    return samp;
}

bool SpecFrame::is_band_limited() const
{
    if (boundInd > 0)
        return true;
    else
        return false;
}

bool SpecFrame::is_in_band(int samp) const
{
    if (samp_per_channel <= 0)
        return false;

    if ((samp >= boundInd) && (samp < (boundInd + samp_per_channel)))
        return true;
    else
        return false;

}

double SpecFrame::get_carrier() const
{
    return f0;
}

void SpecFrame::set_carrier(double f0)
{
    assert(f0 > 0);
    this->f0 = f0;
}

double SpecFrame::get_frequency_resolution() const
{
    return freq_resolution;
}

void SpecFrame::set_frequency_resolution(double f_res)
{
    assert(f_res > 0);
    freq_resolution = f_res;
}

double SpecFrame::get_sampling_frequency() const
{
    return sampling_frequency;
}

void SpecFrame::set_sampling_frequency(double fs)
{
    assert(fs > 0);
    sampling_frequency = fs;
}

double SpecFrame::get_band_width() const
{
    double band_width;
    if (channels_total > 0)
        band_width = samp_per_channel * freq_resolution;
    else
        band_width = NAN;

    return band_width;
}

int SpecFrame::get_channels_total() const
{
    return channels_total;
}

int SpecFrame::get_length() const
{
    return samp_per_channel;
}

int SpecFrame::get_full_frame_length() const
{
    return round( sampling_frequency / freq_resolution);
}

void SpecFrame::set_post_id(int id)
{
    assert(id >= 0);
    postId = id;
}

int SpecFrame::get_post_id() const
{
    return postId;
}

void SpecFrame::set_count(int val)
{
    assert(val >= 0);
    count = val;
}

int SpecFrame::get_count() const
{
    return count;
}

void SpecFrame::set_bound(int bound)
{
    assert(bound >= 0);
    boundInd = bound;
}

int SpecFrame::get_bound() const
{
    return boundInd;
}

void SpecFrame::erase()
{
    parent->erase_frame(count);
}

SpecFrame_parent *SpecFrame::get_parent()
{
    return parent;

}

void SpecFrame::set_parent(SpecFrame_parent *i_parent)
{
    parent = i_parent;
}

SpecFrame_parent::SpecFrame_parent()
{

}
