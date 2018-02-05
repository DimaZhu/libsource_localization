#ifndef ANTENNA_H
#define ANTENNA_H

#include<vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>

#define DIM 3
using namespace std;
/* Класс Antenna предназначен для пространственного
 * моделирования фазированной антенной решетки
 */

typedef vector<float> coordinate;
typedef vector<vector<float> > coordinates;
class Antenna
{

public:
    explicit Antenna();
    //explicit Antenna( QString &filename);
    explicit Antenna( char * str);
    Antenna(const Antenna &antenna);
    ~Antenna();


    friend bool operator==(const Antenna &antenna1, const Antenna &antenna2);
    friend bool operator!=(const Antenna &antenna1, const Antenna &antenna2) { return !(antenna1 == antenna2);}

        //seters
    void set_model(vector< vector< float> > coordinates);
    void set_base(vector< float>  coordinates);
    void set_name(string i_name);
    void set_orientation(float i_orientation);

        //geters
    vector< vector< float> > get_model() const;
    vector< vector< float> > get_elements() const;
    vector<float> get_base() const;

    vector<float> get_elements_on_stages() const;
    float get_orientation() const;
    string get_name() const;
    int get_channels_total() const;

    bool adjust_elements_coordinates(vector<vector<float> > coorection);

//    // Working with files
//    void save_in_file(QTextStream &out) const;
//    static Antenna read_from_file(QTextStream &in);

private:

    void initialization(string &filename);
    void calculate_stages_number();
    void calculate_elements_coordinates();

    vector< vector< float> > model;
    vector< vector< float> > elements;
    vector< float>  base;
    vector< float>  elementsOnStages;
    float orientation;

    string  name;
};

#endif // ANTENNA_H
