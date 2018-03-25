#include "delayestimator.h"

DelayEstimator::DelayEstimator()
{
}

Estimation DelayEstimator::estimate(SpecFrame const * const frame) const
{

    Estimation phases = PseudoPhaseEstimator::estimate(frame);
    Estimation delays;

    int carrier_samp = ((samp_stop + samp_start) / 2 + frame->get_bound() - frame->get_full_frame_length() / 2);
    double carrier_freq = frame->get_central_frequency() + carrier_samp * frame->get_frequency_resolution();


    if (phases.size() != frame->get_channels_total())
        printf("Warning! Number of pseudophase estimations doen't math number"
               "of frame channels");

    for (int ch = 0; ch < frame->get_channels_total(); ++ch)
    {
        delays.push_back(- phases[ch] / (2 * M_PI * carrier_freq));
    }

    return delays;
}
