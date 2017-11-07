#ifndef PELENGATOR_H
#define PELENGATOR_H

#include <Python.h>
#include<antenna.h>
#include<specframe.h>
#include<grid_synth.h>
#include<lh_pel.h>

class Pelengator
{
public:
    Pelengator(Antenna ant);
    void set_signal_param(double f0, double df, double f_res, double fs);
    void estimate(SpecFrame frame);

private:

    Lh_Pel lh;
    Antenna antenna;
    vector<vector<vector<float>>> grid;

};


#endif // PELENGATOR_H
