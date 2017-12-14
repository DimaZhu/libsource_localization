#ifndef PELENGATOR_H
#define PELENGATOR_H

#include <Python.h>
#include<antenna.h>
#include<specframe.h>
#include<interpolator.h>
#include<cmath>
#include<grid.h>
#include<lh_pel.h>

typedef vector<double> estimationT;

using namespace std;
class Pelengator
{
public:

    Pelengator(const Antenna &ant);
    ~Pelengator();

    void set_signal_param(double f0, double df, double f_res, double fs);
    estimationT estimate(SpecFrame frame);
    void turn_on_interpolation(bool turn_on);

private:

    Antenna antenna;
    vector<vector<vector<double>>> coord_grid;
    vector<vector<double>> lh_matrix;
    PyObject * grid_func;
    bool interpolation;

};


#endif // PELENGATOR_H
