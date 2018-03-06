#ifndef ANTENNA_H
#define ANTENNA_H

#include<vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>

#define DIM 3
using namespace std;
/* Класс Antenna предназначен для пространственного
 * моделирования фазированной антенной решетки
 *
 * формат координат:
 * [ [x0, x1, x2, ...],
 *   [y0, y2, y2, ...],
 *   [z0, z1, z2, ...]]
 */

//typedef vector<float> coordinate;
typedef vector<vector<float> > coordinates;
class Antenna
{

public:
    explicit Antenna();
    explicit Antenna( string str);
    Antenna(const Antenna &antenna);
    ~Antenna();


    friend bool operator==(const Antenna &antenna1, const Antenna &antenna2);
    friend bool operator!=(const Antenna &antenna1, const Antenna &antenna2) { return !(antenna1 == antenna2);}

        //seters
    void set_model(vector< vector< float> > coordinates);
    void set_base(vector< float>  coordinates);
    void set_base(vector< vector<float> > coordinates);
    void set_name(string i_name);
    void set_orientation(float i_orientation);
    void set_phase_center(vector<float> center);
    void set_phase_center(vector< vector< float> > center);

        //geters
    vector< vector< float> > get_model() const;
    vector< vector< float> > get_elements() const;
    vector<vector<float>> get_base() const;
    vector<vector<float>> get_phase_center() const;

    vector<float> get_elements_on_stages() const;
    float get_orientation() const;
    string get_name() const;
    int get_channels_total() const;

    bool adjust_elements_coordinates(vector<vector<float> > coorection);

//    // Working with files
//    void save_in_file(QTextStream &out) const;
//    static Antenna read_from_file(QTextStream &in);

private:
    void calculate_stages_number();
    void calculate_elements_coordinates();

    vector< vector< float> > model;
    vector< vector< float> > elements;
    vector< float>  base;
    vector< float>  elementsOnStages;
    vector<float> phase_center_real;
    vector<float> phase_center_model;
    float orientation;

    string  name;
};

#endif // ANTENNA_H
