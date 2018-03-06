#ifndef ESTIMATOR_H
#define ESTIMATOR_H

#include <vector>
#include <specframe.h>

typedef std::vector<double> Estimation;
class Estimator
{
public:
    Estimator();
    virtual Estimation estimate(SpecFrame *frame) const = 0;

};

#endif // ESTIMATOR_H
