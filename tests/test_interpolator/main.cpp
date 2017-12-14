#include <Python.h>
#include <QCoreApplication>
#include<lh_pel.h>
#include <vector>
#include <cmath>
#include <interpolator.h>
#include <mypoly.h>
#include <mysinc.h>
#include <specframe.h>
#include <QDebug>

using namespace  std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MySinc func;

    vector<vector<vector<double>>> x;
    int start = 0;
    int stop = 10;

    vector<vector<double>> buffer2d_arg1;
    vector<vector<double>> buffer2d_arg2;
    vector<vector<double>> y;
    for (int i = start; i < stop; ++i)
    {
        vector<double> buffer1d_arg1;
        vector<double> buffer1d_arg2;
        vector<double> buffer_y;
        for (int j = start; j < stop; ++j)
        {
            double arg1 = j;
            double arg2 = i;

            buffer1d_arg1.push_back(arg1);
            buffer1d_arg2.push_back(arg2);
            parameters_vector arg(2);
            arg = arg1, arg2;
            buffer_y.push_back(func(arg));
        }

        buffer2d_arg1.push_back(buffer1d_arg1);
        buffer2d_arg2.push_back(buffer1d_arg2);
        y.push_back(buffer_y);
    }

    x.push_back(buffer2d_arg1);
    x.push_back(buffer2d_arg2);

    SpecFrame specFrame;

    Interpolator interpolator;
    Lh * lh_ptr = &func;
    vector<vector<double>> output = interpolator.find_minimum2d(x, y, lh_ptr);
    qDebug() << "Min at: " + QString::number(output[0][0]) + ", " + QString::number(output[0][1]);
    qDebug() << "Fun value at min: " + QString::number(output[1][0]);

    return a.exec();
}
