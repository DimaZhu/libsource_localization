#include <Python.h>
#include <QCoreApplication>
#include <pelengator.h>
#include <complex.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try{
        QString str = "/home/dima/.landing/AFS4.ini";
        Antenna ant(str.toLatin1().data());

        double f0 = 120e6;
        double fs = 260e6;
        double f_res = 10e3;
        double df = 10e3;
        complex2d data;

        for (int i = 0; i < ant.get_channels_total(); ++i)
        {
            vector<complex<double>> data_vec;
            complex<double> val = 0;
            data_vec.push_back(val);
            data.push_back(data_vec);
        }
        SpecFrame frame;
        frame.set_carrier(f0);
        frame.set_sampling_frequency(fs);
        frame.set_frequency_resolution(f_res);
        frame.set_data(data);
        frame.set_bound(f0 / f_res);

        Pelengator pel(ant);
        pel.set_signal_param(f0, df, f_res, fs);
        estimationT estimation = pel.estimate(frame);
        qDebug()<< "test finished succesfully.";
    }
    catch( std::exception &e){
        qDebug()<<e.what();
    }


    return a.exec();
}
