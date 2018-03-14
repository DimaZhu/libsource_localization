#include <QCoreApplication>
#include <specframeloader.h>
#include <delayestimator.h>
#include <iostream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SpecFrame frame;
    SpecFrameLoader loader;
    bool success = loader.open("/home/dima/Projects/Touchdown/Data/train_imp.bin");
    if (!success)
        printf("Enable to open the file");

    loader.read_title();
    loader.load(&frame);
    loader.close();

    DelayEstimator estimator;
    Estimation estimation = estimator.estimate(&frame);

    qDebug()<< "Estimated delays";
    for (int i = 0; i < frame.get_channels_total(); ++i)
        qDebug() << "ch " << i << ": " << estimation[i] * 1e6 << "мкс";


    return a.exec();
}
