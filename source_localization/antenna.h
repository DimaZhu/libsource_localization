#ifndef ANTENNA_H
#define ANTENNA_H

#include<vector>
#include <QString>
#include <QTextStream>
#include <QSettings>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <cmath>

#define DIM 3
using namespace std;
/* Класс Antenna предназначен для пространственного
 * моделирования фазированной антенной решетки
 */

class Antenna
{

public:
    explicit Antenna();
    explicit Antenna( QString &filename);
    explicit Antenna( char * str);
    Antenna(const Antenna &antenna);
    ~Antenna();


    friend bool operator==(const Antenna &antenna1, const Antenna &antenna2);
    friend bool operator!=(const Antenna &antenna1, const Antenna &antenna2) { return !(antenna1 == antenna2);}

        //seters
    void set_model_coordinates(vector< vector< float>> coordinates);
    void set_base_coordinates(vector< float>  coordinates);
    void set_model_name(QString i_model);
    void set_orientation(float i_orientation);

        //geters
    vector< vector< float>> get_model_coordinates() const;
    vector< vector< float>> get_elements_coordinates() const;
    vector<float> get_base_coordinates() const;

    vector<float> get_elements_on_stages() const;
    float get_orientation() const;
    QString get_model_name() const;
    int get_channels_total() const;

    bool adjust_elements_coordinates(vector<vector<float> > coorection);

    // Working with files
    void save_in_file(QTextStream &out) const;
    static Antenna read_from_file(QTextStream &in);

private:

    void initialization(QString &filename);
    void calculate_stages_number();
    void calculate_elements_coordinates();

    vector< vector< float>> modelCoordinates;
    vector< vector< float>> elementsCoordinates;
    vector< float>  baseCoordinates;
    vector< float>  elementsOnStages;
    float orientation;

    QString  model;
};

#endif // ANTENNA_H
