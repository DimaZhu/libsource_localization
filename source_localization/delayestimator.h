#ifndef DELAYESTIMATOR_H
#define DELAYESTIMATOR_H

#include <estimator.h>

class DelayEstimator : public Estimator
{
public:
    DelayEstimator();
    void set_signal_parameters(int ref_ch);
    Estimation estimate(SpecFrame const * const frame);

private:
    void convolve(FrameChannel in1, size_t len_in1,
                  FrameChannel in2, size_t len_in2,
                  Complex1d out);

    int argmax(Complex1d sig, size_t length);

    int ref_ch;
};

#endif // DELAYESTIMATOR_H
