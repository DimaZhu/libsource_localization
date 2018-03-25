#ifndef DELAYESTIMATOR_H
#define DELAYESTIMATOR_H

#include <estimator.h>
#include <pseudophaseestimator.h>

class DelayEstimator : public PseudoPhaseEstimator
{
public:
    DelayEstimator();
    Estimation estimate(SpecFrame const * const frame) const;


};

#endif // DELAYESTIMATOR_H
