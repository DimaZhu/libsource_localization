#ifndef PELENGATOR_H
#define PELENGATOR_H

#include<antenna.h>
#include<specframe.h>
#include<interpolator.h>
#include<cmath>
#include<lhpel.h>
#include<gridpel.h>
#include<cassert>
#include <estimator.h>
#include <peleng.h>

using namespace std;
class Pelengator
{
public:

    Pelengator(const Antenna &ant, double f_res, double fs);
    ~Pelengator();

    //void set_signal_param(double central_frequency, double df, int i_carrier_samp);
    void set_signal_param(double carrier, int  i_samp_start, int i_samp_stop);
    Estimation estimate(SpecFrame const * const frame);
    void turn_on_interpolation(bool turn_on);
    bool is_active();
    void turn_on(bool);
    void set_verbose(bool );
private:

    Antenna antenna;
    vector<vector<vector<double>>> coord_grid;
    vector<vector<double>> lh_matrix;
    bool interpolation;
    bool verbose;
    double sampling_frequency;
    double frequency_resolution;
    bool active;
    int samp_start;
    int samp_stop;

};

#endif // PELENGATOR_H
