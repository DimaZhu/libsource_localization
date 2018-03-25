#include "pseudophaseestimator.h"

PseudoPhaseEstimator::PseudoPhaseEstimator():
    ref_ch(0),
    samp_start(0),
    samp_stop(0)
{

}

void PseudoPhaseEstimator::set_signal_parameters(int i_ref_ch, int i_samp_start, int i_samp_stop)
{
    ref_ch = i_ref_ch;
    samp_start = i_samp_start;
    samp_stop = i_samp_stop;
}

Estimation PseudoPhaseEstimator::estimate(SpecFrame const * const frame) const
{

    Estimation estimation;
    FrameData data = frame->get_data();

    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        Complex dot_product = 0;
        double pseudo_phase = 0;

        for (int s = samp_start; s <= samp_stop; ++s)
        {
            dot_product += data[ch][s] * conj(data[ref_ch][s]);

        }

        if ( samp_start >= frame->get_full_frame_length() / 2)
            pseudo_phase = arg(dot_product);
        else
            pseudo_phase = arg(conj(dot_product));

        estimation.push_back(pseudo_phase);
    }

    return estimation;
}
