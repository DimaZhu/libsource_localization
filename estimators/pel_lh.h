#ifndef PEL_LH_H
#define PEL_LH_H
#include<boost/python.hpp>
#include <dlib/optimization.h>
#include <cmath>
#include <complex.h>
#include <QtGlobal>
#define DIM 3

using namespace dlib;
using namespace std;


// In dlib, the general purpose solvers optimize functions that take a column
// vector as input and return a double.  So here we make a typedef for a
// variable length column vector of doubles.  This is the type we will use to
// represent the input to our objective functions which we will be minimizing.
typedef matrix<double,0,1> column_vector;

class Pel_Lh
{
public:
    Pel_Lh(const double * const * const amp, const double * const * const phase, int sig_length,  const double * const * const antenna_coordinates, int elemets_total, double f0);
    ~Pel_Lh();

    double operator() (const column_vector& arg) const;

private:
    double *pseudophase;
    double *mutual_amp;
    double **coord_delta;
    int ant_pairs;
    double ang_freq;
};


BOOST_PYTHON_MODULE(estimators) {
    using namespace boost::python;

    class_<Pel_Lh>("Pel_Lh",
                   init<const double * const * const, const double * const * const, int,  const double * const * const, int, double>())
                   .def("calculate", &Pel_Lh::operator())
            ;
}

#endif // PEL_LH_H
