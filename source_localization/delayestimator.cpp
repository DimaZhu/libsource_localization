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
    Estimation indexes;

    FrameData data = frame->get_data();
    size_t sig_len = frame->get_length();
    size_t vcf_len = sig_len;
    double fs = frame->get_sampling_frequency();

    //Complex1d vcf = (Complex1d) malloc(vcf_len * sizeof(Complex));
    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        dlib::matrix<complex<double>> vcf = convolve(data[ref_ch], data[ch], sig_len);
        int ind = argmax(vcf, vcf_len);
        indexes.push_back(ind);

        printf("Calculating... %f", (float)ch/ (float)frame->get_channels_total());
    }

    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        int delay_s = indexes[ch] - indexes[ref_ch];
        double delay_t = delay_s / fs;
        estimation.push_back(delay_t);
    }


    return estimation;
}


dlib::matrix<complex<double>> DelayEstimator::convolve(FrameChannel in1,
                              FrameChannel in2, size_t len) const
{
    dlib::matrix<complex<double>> in1_mat;
    dlib::matrix<double> in1_imag_mat;
    dlib::matrix<complex<double>> in2_mat;
    dlib::matrix<double> in2_imag_mat;
    dlib::matrix<complex<double>> out_real;
    dlib::matrix<complex<double>> out_imag;
    dlib::matrix<complex<double>> out;

    dlib::matrix<complex<double>> in1_real_f;
    dlib::matrix<complex<double>> in1_imag_f;
    dlib::matrix<complex<double>> in2_real_f;
    dlib::matrix<complex<double>> in2_imag_f;

    in1_mat.set_size(1, len);
  //  in1_imag_mat.set_size(1, len);
    in2_mat.set_size(1, len);
 //   in2_imag_mat.set_size(1, len);
//    out_real.set_size(1, len);
//    out_imag.set_size(1, len);
    out.set_size(1, len);

//    in1_real_f.set_size(1, len);
//    in1_imag_f.set_size(1, len);
//    in2_real_f.set_size(1, len);
//    in2_imag_f.set_size(1, len);

    for (size_t i = 0; i < len; ++i)
    {
        in1_mat(0, i) = std::real(in1[i]);
       // in1_imag_mat(0,i) = std::imag(in1[i]);
        in2_mat(0, i) = std::real(in2[i]);
       // in2_imag_mat(0,i) = std::imag(in2[i]);
    }

//     out_real = dlib::xcorr_same(in1_real_mat, in2_real_mat);
//     out_imag = dlib::xcorr_same(in1_imag_mat, in2_imag_mat);

    dlib::fft_inplace(in1_mat);
//    in1_imag_f = dlib::fft(in1_imag_mat);
//    in2_real_f = dlib::fft(in2_real_mat);
    dlib::fft_inplace(in2_mat);
    out = dlib::pointwise_multiply(in1_mat, in2_mat);
    out =dlib::ifft(out);
//    out_imag = dlib::pointwise_multiply(in1_imag_f, in2_imag_f);
//    out_imag = dlib::ifft(out_imag);

//    out = dlib::complex_matrix(dlib::real(out_real), dlib::imag(out_imag));
    return out;
}

int DelayEstimator::argmax(dlib::matrix<complex<double>> sig, size_t length) const
{
    float max_real = std::real(sig(0));
    int max_ind_real = 0;

    float max_imag = std::imag(sig(0));
    int max_ind_imag = 0;

    for (size_t i = 0; i < length; ++i)
    {

        if(std::real(sig(i)) > max_real){
            max_real = std::real(sig(i));
            max_ind_real = i;
        }

        if(std::imag(sig(i)) > max_imag){
            max_imag = std::imag(sig(i));
            max_ind_imag = i;
        }
    }

//    return max_ind_real;
    if (max_real > max_imag)
        return max_ind_real;
    else
        return max_ind_imag;
}
