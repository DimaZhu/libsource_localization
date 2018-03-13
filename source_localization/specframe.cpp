#include "specframe.h"

SpecFrame::SpecFrame(int i_channels_total, int i_samp_per_ch):
    parent(NULL),
    channels_total(i_channels_total),
    samp_per_channel(i_samp_per_ch),
    f0(NAN),
    sampling_frequency(NAN),
    freq_resolution(NAN),
    postId(-1),
    serial(-1),
    boundInd(0),
    reference_counter(1)
{
    if ((i_channels_total == 0) || (i_samp_per_ch == 0))
        data = nullptr;
    else
        allocate_memory();
}

SpecFrame::SpecFrame(const SpecFrame &frame):
    channels_total(frame.get_channels_total()),
    samp_per_channel(frame.get_length()),
    f0(frame.get_central_frequency()),
    sampling_frequency(frame.get_sampling_frequency()),
    freq_resolution(frame.get_frequency_resolution()),
    postId(frame.get_post_id()),
    serial(frame.get_serial()),
    boundInd(frame.get_bound()),
    reference_counter(1)
{
    free_memory();
    allocate_memory();

    FrameData new_data = frame.get_data();

    for (int ch = 0; ch < channels_total; ++ch)
        for (int s = 0; s < samp_per_channel; ++s)
            data[ch][s] = new_data[ch][s];

}

SpecFrame::~SpecFrame()
{
    free_memory();
}

const SpecFrame &SpecFrame::operator =(const SpecFrame &frame)
{
    // Избегать самоприсваивания
    if (&frame != this) {

        channels_total = frame.get_channels_total();
        samp_per_channel = frame.get_length();
        f0 = frame.get_central_frequency();
        sampling_frequency = frame.get_sampling_frequency();
        freq_resolution = frame.get_frequency_resolution();
        postId = frame.get_post_id();
        serial = frame.get_serial();
        boundInd = frame.get_bound();
        reference_counter = 1;

        free_memory();
        allocate_memory();

        FrameData new_data = frame.get_data();

        for (int ch = 0; ch < channels_total; ++ch)
            for (int s = 0; s < samp_per_channel; ++s)
                data[ch][s] = new_data[ch][s];
    }
    return *this;
}

void SpecFrame::allocate_memory()
{
    data = (Complex2d ) malloc( channels_total * sizeof(Complex1d));
    for (int ch = 0; ch < channels_total; ++ch)
        data[ch] = (Complex1d) malloc(samp_per_channel * sizeof(Complex));

    for (int ch = 0; ch < channels_total; ++ch)
        for (int s = 0; s < samp_per_channel; ++s)
        data[ch][s] = 0;
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

    free_memory();

    channels_total = i_channels_total;
    samp_per_channel = i_samp_per_ch;

    allocate_memory();

}

FrameData SpecFrame::get_data() const
{
    return data;
}

void SpecFrame::filter(FrameData freq_response)
{

    for (int ch = 0; ch < channels_total; ++ch)
      for (int s = 0; s < samp_per_channel; ++s)
          data[ch][s] = data[ch][s] * freq_response[ch][s + boundInd];

}

void SpecFrame::filter(Complex1d freq_response, int ch_ind)
{
    for (int s = 0; s < samp_per_channel; ++s)
        data[ch_ind][s] = data[ch_ind][s] * freq_response[s + boundInd];
}

void SpecFrame::clear()
{

    f0 = NAN;
    serial = -1;
    reference_counter = 1;

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

double SpecFrame::get_central_frequency() const
{
    return f0;
}


double SpecFrame::get_frequency_resolution() const
{
    return freq_resolution;
}


double SpecFrame::get_sampling_frequency() const
{
    return sampling_frequency;
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

int SpecFrame::get_post_id() const
{
    return postId;
}

int SpecFrame::get_serial() const
{
    return serial;
}


int SpecFrame::get_bound() const
{
    return boundInd;
}

void SpecFrame::erase()
{
    --reference_counter;
    if ((reference_counter == 0) && (parent != NULL))
        parent->erase_frame(serial);
}

SpecFrame* SpecFrame::copy()
{
    ++reference_counter;
    return this;
}
