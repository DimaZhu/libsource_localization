#ifndef MYSINC_H
#define MYSINC_H

#include <lh.h>
#include <cmath>

class MySinc : public Lh
{
public:
    MySinc();
    double operator() (const parameters_vector& arg) const;
    double calculate(double alpha, double betta) const;
};

#endif // MYSINC_H
