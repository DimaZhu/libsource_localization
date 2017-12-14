#include "mypoly.h"

MyPoly::MyPoly()
{

}

double MyPoly::operator ()(const parameters_vector& arg) const
{
    double shift[2] = {3, 3};
    double width[2] = {1, 2};
    double angle = 0;


    double x_rot = cos(angle) * arg(0) - sin(angle) * arg(1);
    double y_rot = sin(angle) * arg(1) + cos(angle) * arg(1);

    double z = pow((x_rot - shift[0]),2) / width[0] + pow((y_rot - shift[1]), 2) / width[1];
    return z;
}

double MyPoly::calculate(double alpha, double betta) const
{
    return 0;
}
