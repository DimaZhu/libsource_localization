#ifndef MYPOLY_H
#define MYPOLY_H

#include <lh.h>

class MyPoly : public Lh
{
public:
    MyPoly();
    double operator() (const parameters_vector& arg) const;
    double calculate(double alpha, double betta) const;
};

#endif // MYPOLY_H
