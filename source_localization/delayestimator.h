#ifndef DELAYESTIMATOR_H
#define DELAYESTIMATOR_H

#include <estimator.h>
#include <dlib/dlib/matrix.h>

class DelayEstimator : public Estimator
{
public:
    DelayEstimator();
    void set_signal_parameters(int ref_ch);
    Estimation estimate(SpecFrame const * const frame) const;

private:
    dlib::matrix<double>  convolve(FrameChannel in1,
                  FrameChannel in2, size_t len) const;

    int argmax(dlib::matrix<double> sig, size_t length) const;

    int ref_ch;
};

#endif // DELAYESTIMATOR_H
