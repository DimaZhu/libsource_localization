#ifndef PELENGATOR_H
#define PELENGATOR_H

#include <Python.h>
#include<antenna.h>
#include<specframe.h>
#include<interpolator.h>
#include<cmath>
#include<grid_api.h>
#include<lh_pel.h>

typedef vector<double> Peleng;

using namespace std;
class Pelengator
{
public:

    Pelengator(const Antenna &ant, double f_res, double fs);
    ~Pelengator();

    //void set_signal_param(double central_frequency, double df, int i_carrier_samp);
    void set_signal_param(double carrier, int  i_samp_start, int i_samp_stop);
    Peleng estimate(SpecFrame *frame);
    void turn_on_interpolation(bool turn_on);
    bool isActive();
    void turn_on(bool);

private:

    Antenna antenna;
    vector<vector<vector<double>>> coord_grid;
    vector<vector<double>> lh_matrix;
    PyObject * grid_func;
    bool interpolation;
    double sampling_frequency;
    double frequency_resolution;
    bool active;
    int samp_start;
    int samp_stop;

};


#endif // PELENGATOR_H
