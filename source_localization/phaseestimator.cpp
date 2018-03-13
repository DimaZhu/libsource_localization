#include "phaseestimator.h"

PhaseEstimator::PhaseEstimator()
{

}

void PhaseEstimator::set_signal_parameters(int i_samp)
{
    samp = i_samp;
}

Estimation PhaseEstimator::estimate(const SpecFrame * const frame) const
{
    if (samp < frame->get_bound())
        throw std::runtime_error("Sample is out of frame bounds");

    if (samp > (frame->get_bound() + frame->get_length()))
        throw std::runtime_error("Sample is out of frame bounds");

    Estimation estimation;
    FrameData data = frame->get_data();

    for (int ch = 0; ch < frame->get_channels_total(); ++ch){

        double phase = arg(data[ch][samp]);

        if (phase >= M_PI)
            phase -= 2 * M_PI;
        if (phase < - M_PI)
            phase += 2 * M_PI;

        estimation.push_back(phase);
    }
    return estimation;
}
