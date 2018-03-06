#ifndef AMP_ESTIMATOR_H
#define AMP_ESTIMATOR_H

#include <estimator.h>

class AmpEstimator : public Estimator
{
public:
    AmpEstimator();
    void set_signal_parameter(int i_samp_start, int i_samp_stop);
    Estimation estimate(SpecFrame *frame) const;

private:
    int samp_start;
    int samp_stop;
};

#endif // AMP_ESTIMATOR_H
