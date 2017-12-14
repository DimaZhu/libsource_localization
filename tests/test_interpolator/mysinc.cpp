#include "mysinc.h"

MySinc::MySinc()
{

}

double MySinc::operator () (const parameters_vector& arg) const
{
    double shift[2] = {3, 4};
    double width[2] = {10, 10};
    double angle = 0;


    double x_rot = cos(angle) * arg(0) - sin(angle) * arg(1);
    double y_rot = sin(angle) * arg(1) + cos(angle) * arg(1);

    double sincx;
    double sincy;

    if (std::abs(x_rot - shift[0]) > 1e-3){
        sincx = std::sin(M_PI * (x_rot - shift[0]) / width[0]) / (M_PI * (x_rot - shift[0]) / width[0]);
    } else {
        sincx = 1;
    }

    if (std::abs(y_rot - shift[1]) > 1e-3){
        sincy = std::sin(M_PI * (y_rot - shift[1]) / width[1]) / (M_PI * (y_rot - shift[1]) / width[1]);
    } else {
        sincy = 1;
    }
    double z = - sincx * sincy;
    return z;
}


double MySinc::calculate(double alpha, double betta) const
{
    return 0;
}
