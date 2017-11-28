#ifndef PELENGATOR_H
#define PELENGATOR_H

#include <Python.h>
#include<antenna.h>
#include<specframe.h>
#include<cmath>
#include<grid.h>
#include<lh_pel.h>

typedef vector<float> estimationT;

using namespace std;
class Pelengator
{
public:

    Pelengator(const Antenna &ant);
    ~Pelengator();

    void set_signal_param(double f0, double df, double f_res, double fs);
    estimationT estimate(SpecFrame frame);

private:

    Antenna antenna;
    vector<vector<vector<float>>> coord_grid;
    vector<vector<double>> lh_matrix;
    PyObject * grid_func;

};


#endif // PELENGATOR_H
