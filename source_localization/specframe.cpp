#include "specframe.h"

SpecFrame::SpecFrame():
    QObject(),
    f0(NAN),
    sampling_frequency(NAN),
    freq_resolution(NAN),
    postId(-1),
    count(-1),
    boundInd(0)
{

}

SpecFrame::SpecFrame(const SpecFrame &frame):
    QObject(),
    f0(frame.get_carrier()),
    sampling_frequency(frame.get_sampling_frequency()),
    freq_resolution(frame.get_frequency_resolution()),
    postId(frame.get_post_id()),
    count(frame.get_count()),
    boundInd(frame.get_bound())
{
    data = frame.get_data(0, frame.get_channels_total() - 1, boundInd, boundInd + frame.get_length());
}

const SpecFrame &SpecFrame::operator =(const SpecFrame &frame)
{
    // Избегать самоприсваивания
    if (&frame != this) {

        f0 = frame.get_carrier();
        sampling_frequency = frame.get_sampling_frequency();
        freq_resolution = frame.get_frequency_resolution();
        postId = frame.get_post_id();
        count = frame.get_count();
        boundInd = frame.get_bound();
        data = frame.get_data(0, frame.get_channels_total(), 0, frame.get_length());
    }
    return *this;
}

complex2d *SpecFrame::get_data()
{
    return &data;
}

complex2d SpecFrame::get_data(int samp_start, int samp_stop) const
{
   return get_data(0, data.size(), samp_start, samp_stop);
}

complex2d SpecFrame::get_data(int ch_start, int ch_stop, int samp_start, int samp_stop) const
{

    samp_start -= boundInd;
    if (samp_start < 0)
        throw std::invalid_argument("index samp_start is out of SpecFrame bounds");

    samp_stop -= boundInd;
    if (samp_stop < 0)
        throw std::invalid_argument("index samp_stop is out of SpecFrame bounds");

    if (ch_start < 0)
        throw std::invalid_argument("index ch_start is out of SpecFrame bounds");

    if (ch_stop >= (int)data.size())
        throw std::invalid_argument("index ch_stop is out of SpecFrame bounds");


    complex2d output;
    for (int ch = ch_start; ch <= ch_stop; ++ch)
    {
        vector<complex<double>>::const_iterator first = data[ch].begin() + samp_start;
        vector<complex<double>>::const_iterator last = data[ch].begin() + samp_stop;
        vector<complex<double>> buffer(first, last);
        output.push_back(buffer);
    }

    return output;

}

void SpecFrame::push_back(int channels_total, int samp_in_channel, complex<double> **extra_data)
{
    for (int ch = 0; ch < channels_total; ++ch)
    {
        vector<complex<double>> channel;
        for (int samp = 0; samp < samp_in_channel; ++samp)
        {
            channel.push_back(extra_data[ch][samp]);
        }
        data.push_back(channel);
    }
}

void SpecFrame::set_data(complex2d data)
{
    this->data = data;
}

void SpecFrame::clear()
{

    f0 = NAN;
    sampling_frequency = NAN;
    freq_resolution = NAN;
    postId = -1;
    count = -1;
    boundInd = 0;
    data.clear();

}

int SpecFrame::ind(int samp) const
{
    Q_ASSERT(samp > 0);
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
    if (data.size() <= 0)
        return false;

    if (((boundInd - samp) >= 0) && ((int)data.size() - samp >= 0))
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
    Q_ASSERT(f0 > 0);
    this->f0 = f0;
}

double SpecFrame::get_frequency_resolution() const
{
    return freq_resolution;
}

void SpecFrame::set_frequency_resolution(double f_res)
{
    Q_ASSERT(f_res > 0);
    freq_resolution = f_res;
}

double SpecFrame::get_sampling_frequency() const
{
    return sampling_frequency;
}

void SpecFrame::set_sampling_frequency(double fs)
{
    Q_ASSERT(fs > 0);
    sampling_frequency = fs;
}

double SpecFrame::get_band_width() const
{
    double band_width;
    if (data.size() > 0)
        band_width = data[0].size() * freq_resolution;
    else
        band_width = NAN;

    return band_width;
}

int SpecFrame::get_channels_total() const
{
    return data.size();
}

int SpecFrame::get_length() const
{
    if (data.size() > 0)
        return data[0].size();
    else
        return -1;
}

int SpecFrame::get_full_frame_length() const
{
    return round( sampling_frequency / freq_resolution);
}

void SpecFrame::set_post_id(int id)
{
    Q_ASSERT(id > 0);
    postId = id;
}

int SpecFrame::get_post_id() const
{
    return postId;
}

void SpecFrame::set_count(int val)
{
    Q_ASSERT(val >= 0);
    count = val;
}

int SpecFrame::get_count() const
{
    return count;
}

void SpecFrame::set_bound(int bound)
{
    Q_ASSERT(bound >= 0);
    boundInd = bound;
}

int SpecFrame::get_bound() const
{
    return boundInd;
}

void SpecFrame::erase()
{
    emit erased(count);
}


