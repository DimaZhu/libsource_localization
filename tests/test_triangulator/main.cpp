#include <QCoreApplication>
#include <peleng.h>
#include <triangulator.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Peleng pel1;
    pel1.azimuth = M_PI * 45 / 180;
    pel1.elevation = 0;
    pel1.phase_center = vector< vector< float>> {vector<float> {0}, vector<float> {0}, vector<float> {0}};

    Peleng pel2;
    pel2.azimuth = M_PI * (-90) / 180;
    pel2.elevation = 0;
    pel2.phase_center = vector< vector< float>> {vector<float> {0.5}, vector<float> {1}, vector<float> {0}};

    Peleng pel3;
    pel3.azimuth = M_PI * 135 / 180;
    pel3.elevation = M_PI * (0) / 180;
    pel3.phase_center = vector< vector< float>> {vector<float> {1}, vector<float> {0}, vector<float> {0}};

    vector<Peleng> pelengs {pel1, pel2, pel3};
    Triangulator tr;
    coordinates coord =  tr.estimate(pelengs);

    return a.exec();
}
