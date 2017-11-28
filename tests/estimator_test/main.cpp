#include <Python.h>
#include <QCoreApplication>
#include<py_source_localization.h>
#include <antenna.h>

int main(int argc, char *argv[])
{

    Py_Initialize();
    PyInit_py_source_localization();

    QCoreApplication a(argc, argv);

    QString str = "/home/dima/.landing/AFS4.ini";
    Antenna ant(str);
    double f0 = 120e6;
    double fs = 100e6;
    double df = 10e3;
    double f_res = 10e3;

    std::vector<std::vector<std::vector<float>>> grid = pel_grid(ant.get_model_coordinates(), f0, df, f_res, fs);
    Py_Finalize();

    return a.exec();
}
