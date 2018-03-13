#include <QCoreApplication>
#include <specframeloader.h>
#include <delayestimator.h>
#include <iostream>

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

    cout << "Estimated delays";
    for (int i = 0; i < frame.get_channels_total(); ++i)
        cout << "ch " << i << ": " << estimation[i];


    return a.exec();
}
