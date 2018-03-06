#ifndef PHASE_ESTIMATOR_H
#define PHASE_ESTIMATOR_H

#include <estimator.h>
#include <complex>

/* Класс PhaseEstimator оценивает значения фаз.
 * Все значения фазы выдаются в диапазоне [-180; 180)
*/
class PhaseEstimator : public Estimator
{
public:
    PhaseEstimator();
    void set_signal_parameters(int i_samp);
    Estimation estimate(SpecFrame *frame) const;

private:
    int samp;
};


#endif // PHASE_ESTIMATOR_H
