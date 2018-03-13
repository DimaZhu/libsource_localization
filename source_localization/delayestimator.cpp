#include "delayestimator.h"

DelayEstimator::DelayEstimator():
    ref_ch(0)
{
}

void DelayEstimator::set_signal_parameters(int i_ref_ch)
{
    ref_ch = i_ref_ch;
}

Estimation DelayEstimator::estimate(const SpecFrame * const frame)
{

    Estimation estimation;
    FrameData data = frame->get_data();
    size_t sig_len = frame->get_length();
    size_t vcf_len = 2*sig_len - 1;
    double fs = frame->get_sampling_frequency();

    Complex1d vcf = (Complex1d) malloc(vcf_len * sizeof(Complex));
    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        convolve(data[ref_ch], sig_len,
                 data[ch], sig_len,
                 vcf);
        int ind = argmax(vcf, vcf_len);
        int delay_s = ind - (sig_len - 1);
        float delay_t = delay_s * fs;
        estimation.push_back(delay_t);
    }

    return estimation;
}


void DelayEstimator::convolve(FrameChannel in1, size_t len_in1,
                              FrameChannel in2, size_t len_in2,
                              Complex1d out)
{
  size_t n;

  for (n = 0; n < len_in1 + len_in2 - 1; n++)
  {
    size_t kmin, kmax, k;

    kmin = (n >= len_in1 - 1) ? n - (len_in2 - 1) : 0;
    kmax = (n < len_in1 - 1) ? n : len_in1 - 1;

    float real = 0;
    float imag = 0;

    for (k = kmin; k <= kmax; k++)
    {
      real += std::real(in1[k]) * std::real(in2[n-k]);
      imag += std::imag(in1[k]) * std::imag(in2[n-k]);
      out[n] += in1[k] * in2[n - k];
    }

    out[n] = real + 1i * imag;
  }
}

int DelayEstimator::argmax(Complex1d sig, size_t length)
{
    float max_real = std::real(sig[0]);
    int max_ind_real = 0;
    float max_imag = std::real(sig[0]);
    int max_ind_imag = 0;

    for (size_t i = 0; i < length; ++i)
    {

        if(std::real(sig[i]) > max_real){
            max_real = std::real(sig[i]);
            max_ind_real = i;
        }

        if(std::imag(sig[i]) > max_imag) {
            max_imag = std::imag(sig[i]);
            max_ind_imag = i;
        }
    }

    if (max_real > max_imag)
        return max_ind_real;
    else
        return max_ind_imag;
}
