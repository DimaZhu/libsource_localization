#ifndef PELENGATION_H
#define PELENGATION_H

#include <lh.h>


class  LhPel : public Lh
{
public:
    LhPel(Antenna *ant, SpecFrame const * const data, int samp_start, int samp_stop, bool verbose = false);

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
