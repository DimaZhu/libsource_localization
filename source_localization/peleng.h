#ifndef PELENG_H
#define PELENG_H

#include <vector>
#include <cmath>
#include <antenna.h>
class Peleng
{
public:
    Peleng();

    double azimuth;
    double elevation;
    double probability;
    coordinates phase_center;
    long int time;


};

#endif // PELENG_H

