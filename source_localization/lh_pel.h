#ifndef PELENGATION_H
#define PELENGATION_H


#include <dlib/dlib/optimization.h>
#include <cmath>
#include <complex.h>
#include <iostream>
#include <antenna.h>
#include <specframe.h>
#include <vector>
#define DIM 3


using namespace std;


// In dlib, the general purpose solvers optimize functions that take a column
// vector as input and return a double.  So here we make a typedef for a
// variable length column vector of doubles.  This is the type we will use to
// represent the input to our objective functions which we will be minimizing.
typedef dlib::matrix<double,0,1> parameters_vector;


class  Lh_Pel
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
