#ifndef PELENGATION_H
#define PELENGATION_H

#include <lh.h>


class  Lh_Pel : public Lh
{
public:
    Lh_Pel(Antenna *ant, SpecFrame *data);

    double operator() (const parameters_vector& arg) const;
    double calculate(double alpha, double betta) const;



private:
    std::vector<double> pseudophase;
    std::vector<double> mutual_amp;
    std::vector<std::vector<float>>coord_delta;
    int ant_pairs;
    double ang_freq;
    double lh_max;
};

#endif // PELENGATION_H
