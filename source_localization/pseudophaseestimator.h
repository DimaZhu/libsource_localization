#ifndef PSEUDOPHASEESTIMATOR_H
#define PSEUDOPHASEESTIMATOR_H

#include <estimator.h>

class PseudoPhaseEstimator : public Estimator
{
public:
    PseudoPhaseEstimator();
    void set_signal_parameters(int ref_ch, int  i_samp_start, int i_samp_stop);
    virtual Estimation estimate(const SpecFrame * const frame) const;

protected:

    int ref_ch;
    int samp_start;
    int samp_stop;
};

#endif // PSEUDOPHASEESTIMATOR_H
