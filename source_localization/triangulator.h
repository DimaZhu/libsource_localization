#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include<antenna.h>
#include<peleng.h>
#include <dlib/dlib/optimization.h>

class Triangulator
{
public:
    Triangulator();
    void set_active(bool arg);
    bool is_active();
    coordinates estimate(vector<Peleng> pelengs);

private:

    dlib::matrix<double, 3,  1> calculate_vertex(Peleng &peleng0, Peleng &peleng1) const;
    bool active;
};

#endif // TRIANGULATOR_H
