#include "delayestimator.h"

DelayEstimator::DelayEstimator():
    ref_ch(0)
{
}

void DelayEstimator::set_signal_parameters(int i_ref_ch)
{
    ref_ch = i_ref_ch;
}

Estimation DelayEstimator::estimate(SpecFrame const * const frame) const
{

    Estimation estimation;
    FrameData data = frame->get_data();
    size_t sig_len = frame->get_length();
    size_t vcf_len = 2*sig_len - 1;
    double fs = frame->get_sampling_frequency();

    Complex1d vcf = (Complex1d) malloc(vcf_len * sizeof(Complex));
    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        dlib::matrix<complex<double>> vcf = convolve(data[ref_ch], data[ch], sig_len);
        int ind = argmax(vcf, vcf_len);
        int delay_s = ind - (sig_len - 1);
        float delay_t = delay_s / fs;
        estimation.push_back(delay_t);
    }

    return estimation;
}


dlib::matrix<double> DelayEstimator::convolve(FrameChannel in1,
                              FrameChannel in2, size_t len) const
{
    dlib::matrix<double> in1_mat;
    dlib::matrix<double> in2_mat;
    dlib::matrix<double> out;

    in1_mat.set_size(1, len);
    in2_mat.set_size(1, len);
    out.set_size(1, len);


    for (int i = 0; i < len; ++i)
    {
        in1_mat(0, i) = std::real(in1[i]);
        in2_mat(0, i) = std::real(in2[i]);
    }

    out = dlib::xcorr(in1_mat, in2_mat);
    return out;
}

int DelayEstimator::argmax(dlib::matrix<double> sig, size_t length) const
{
    float max_real = std::real(sig(0));
    int max_ind_real = 0;

    for (size_t i = 0; i < length; ++i)
    {

        if(std::real(sig(i)) > max_real){
            max_real = std::real(sig(i));
            max_ind_real = i;
        }

    }

    return max_ind_real;

}
