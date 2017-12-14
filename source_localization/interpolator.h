#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <source_localization_global.h>
#include <vector>
#include <lh.h>
#include <dlib/dlib/optimization.h>
#include <exception>

using namespace  std;
class  Interpolator
{
public:
    Interpolator();
    vector<vector<double>> find_minimum2d(vector<vector<vector<double>>> x, vector<vector<double>> y, Lh *function);
};

#endif // INTERPOLATOR_H
