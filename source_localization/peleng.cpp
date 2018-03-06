#include "peleng.h"

Peleng::Peleng():
    azimuth(NAN),
    elevation(NAN),
    probability(NAN),
    time(0)
{
    std::vector<float> coord;
    coord.push_back(NAN);
    phase_center.push_back(coord);
    phase_center.push_back(coord);
    phase_center.push_back(coord);
}
